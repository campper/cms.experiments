//---------------------------------------------------------------------------

#define GetPixel4096(data, x, y)	\
	(PARGBQuad)((LPBYTE)data->Scan0 + (y >> 12) * data->Stride + ((x >> 12) << 2))
//---------------------------------------------------------------------------
#include "BmpData.h"

// 获取临近插值颜色
FORCEINLINE
ARGBQuad GetNearColor(CONST BitmapData *data, UINT x, UINT y)
{
	return *GetPixel4096(data, x, y);
}
//---------------------------------------------------------------------------

// 获取线性插值颜色
FORCEINLINE
ARGBQuad GetBilinearColor(CONST BitmapData *data, UINT x, UINT y)
{
	UINT u = (x & 0xfff) >> 4;	// u = (x % 0x1000) / 16
	UINT v = (y & 0xfff) >> 4;	// v = (y % 0x1000) / 16
	UINT u0 = u ^ 255;			// u0 = 255 - u
	UINT v0 = v ^ 255;			// v0 = 255 - v
	UINT m0 = v0 * u0;
	UINT m1 = v * u0;
	UINT m2 = v0 * u;
	UINT m3 = v * u;
	PARGBQuad p0 = GetPixel4096(data, x, y);
	PARGBQuad p1 = (PARGBQuad)((LPBYTE)p0 + data->Stride);
	PARGBQuad p2 = p0 + 1;
	PARGBQuad p3 = p1 + 1;
	ARGBQuad color;
	// 如果不要求很高精度，/ (255 * 255)可改为 >> 16，能提高速度
	color.Blue = (p0->Blue * m0 + p1->Blue * m1 + p0->Blue * m2 + p3->Blue * m3) / (255 * 255);
	color.Green = (p0->Green * m0 + p1->Green * m1 + p2->Green * m2 + p3->Green * m3) / (255 * 255);
	color.Red = (p0->Red * m0 + p1->Red * m1 + p2->Red * m2 + p3->Red * m3) / (255 * 255);
	color.Alpha = (p0->Alpha * m0 + p1->Alpha * m1 + p2->Alpha * m2 + p3->Alpha * m3) / (255 * 255);
	return color;
}
//---------------------------------------------------------------------------

static INT uvTable[513];

// 获取双立方插值颜色
FORCEINLINE
ARGBQuad GetBicubicColor(CONST BitmapData *data, UINT x, UINT y)
{
	INT us[4], vs[4];
	UINT u = (x & 0xfff) >> 4;	// u = (x % 0x1000) / 16
	UINT v = (y & 0xfff) >> 4;	// v = (y % 0x1000) / 16
	us[0] = uvTable[256 + u];
	us[1] = uvTable[u];
	us[2] = uvTable[256 - u];
	us[3] = uvTable[512 - u];
	vs[0] = uvTable[256 + v];
	vs[1] = uvTable[v];
	vs[2] = uvTable[256 - v];
	vs[3] = uvTable[512 - v];

	PARGBQuad p = GetPixel4096(data, x, y);
	INT pixOffset = data->Stride >> 2;
	INT sA, sR, sG, sB;
	sA = sR = sG = sB = 0;

	for (INT i = 0; i < 4; i++, p += pixOffset)
	{
		sB += ((us[0] * p[0].Blue + us[1] * p[1].Blue +
			us[2] * p[2].Blue + us[3] * p[3].Blue) * vs[i]);
		sG += ((us[0] * p[0].Green + us[1] * p[1].Green +
			us[2] * p[2].Green + us[3] * p[3].Green) * vs[i]);
		sR += ((us[0] * p[0].Red + us[1] * p[1].Red +
			us[2] * p[2].Red + us[3] * p[3].Red) * vs[i]);
		sA += ((us[0] * p[0].Alpha + us[1] * p[1].Alpha +
			us[2] * p[2].Alpha + us[3] * p[3].Alpha) * vs[i]);
	}
	sB >>= 16;
	sG >>= 16;
	sR >>= 16;
	sA >>= 16;
	ARGBQuad color;
	sA = sA < 0 ? 0 : sA > 255 ? 255 : sA;
	// 因像素格式为PARGB，上限必须为sA(Alpha)而非255
	color.Blue = sB < 0 ? 0 : sB > sA ? sA : sB;
	color.Green = sG < 0 ? 0 : sG > sA ? sA : sG;
	color.Red = sR < 0 ? 0 : sR > sA ? sA : sR;
	color.Alpha = sA;
	return color;
}
//---------------------------------------------------------------------------

VOID InitBicubicUVTable(FLOAT slope = -0.75)
{
	static FLOAT _slope = 0;
	DOUBLE x, x2, x3;
	if (!(slope < 0)) slope = -0.75;
	if (_slope != slope)
	{
		_slope = slope;
		for (INT i = 0; i <= 512; i++)
		{
			x = i * (1.0 / 256);
			x2 = x * x;
			x3 = x * x2;
			if (x > 2)
				uvTable[i] = 0;
			else if (x > 1)
				uvTable[i] = (INT)(slope * (x3 - 5 * x2 + 8 * x - 4) * 256);
			else
				uvTable[i] = (INT)(((slope + 2) * x3 - (slope + 3) * x2 + 1) * 256);
		}
	}
}
//---------------------------------------------------------------------------

// 目标像素pd和颜色cs合成函数。
FORCEINLINE
VOID MixerColor(PARGBQuad pd, ARGBQuad cs)
{
	if (cs.Alpha == 255)		// 如果源像素不透明度为255，直接拷贝
		pd->Color = cs.Color;
	else if (cs.Alpha != 0)		// 否则，如果源像素不透明度大于0
	{
		if (pd->Alpha == 255)	// 如果目标像素不透明度为255，ARGB合成
		{
			pd->Blue += (cs.Blue - (pd->Blue * cs.Alpha + 127) / 255);
			pd->Green += (cs.Green - (pd->Green * cs.Alpha + 127) / 255);
			pd->Red += (cs.Red - (pd->Red * cs.Alpha + 127) / 255);
		}
		else  					// 否则，PARGB合成
		{
			// pd转换为PARGB，cs已经是PARGB格式
			pd->Blue = (pd->Blue * pd->Alpha + 127) / 255;
			pd->Green = (pd->Green * pd->Alpha + 127) / 255;
			pd->Red = (pd->Red * pd->Alpha + 127) / 255;
			// pd与cs合成
			pd->Blue += (cs.Blue - (pd->Blue * cs.Alpha + 127) / 255);
			pd->Green += (cs.Green - (pd->Green * cs.Alpha + 127) / 255);
			pd->Red += (cs.Red - (pd->Red * cs.Alpha + 127) / 255);
			pd->Alpha += (cs.Alpha - (pd->Alpha * cs.Alpha + 127) / 255);
			// 重新转换为ARGB
			pd->Blue = pd->Blue * 255 / pd->Alpha;
			pd->Green = pd->Green * 255 / pd->Alpha;
			pd->Red = pd->Red * 255 / pd->Alpha;
		}
	}
}
//---------------------------------------------------------------------------

typedef ARGBQuad(*InterpolateProc)(CONST BitmapData*, UINT, UINT);

// 获取插值过程和扩展半径
INT GetInterpolateProc(InterpolateMode mode, InterpolateProc &proc)
{
	INT radius[] = { 2, 1, 2, 4 };
	InterpolateProc procs[] = { GetBilinearColor, GetNearColor,
		GetBilinearColor, GetBicubicColor };
	proc = procs[mode];
	return radius[mode];
}
//---------------------------------------------------------------------------

VOID CopyInterpolateData(BitmapData *dest, CONST BitmapData *source, INT alpha)
{
	//	SetAlphaFlag(dest, HasAlphaFlag(source));
	PARGBQuad pd, ps;
	UINT width, height;
	INT dstOffset, srcOffset;
	GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);
	UINT x, y;
	// 如果alpha < 255或者源数据含Alpha，转换为PARGB像素格式
	if (alpha < 255 || HasAlphaFlag(source))
	{
		for (y = 0; y < height; y++, pd += dstOffset, ps += srcOffset)
		{
			for (x = 0; x < width; x++, pd++, ps++)
			{
				pd->Alpha = (alpha * ps->Alpha + 127) / 255;
				pd->Blue = (ps->Blue * pd->Alpha + 127) / 255;
				pd->Green = (ps->Green * pd->Alpha + 127) / 255;
				pd->Red = (ps->Red * pd->Alpha + 127) / 255;
			}
		}
	}
	// 否则, 直接像素拷贝
	else
	{
		for (y = 0; y < height; y++, pd += dstOffset, ps += srcOffset)
		{
			for (x = 0; x < width; *pd++ = *ps++, x++);
		}
	}
}
//---------------------------------------------------------------------------

VOID FillBorder(BitmapData *data, UINT radius, BOOL fillX, BOOL fillY)
{
	UINT height = data->Height - (radius << 1);
	UINT x, y;
	PARGBQuad pd, ps;
	if (fillX)
	{
		UINT width = data->Width - (radius << 1);
		pd = (PARGBQuad)data->Scan0 + radius * data->Width;
		for (y = 0; y < height; y++)
		{
			for (x = 0, ps = pd + radius; x < radius; *pd++ = *ps, x++);
			for (x = 0, pd += width, ps = pd - 1; x < radius; *pd++ = *ps, x++);
		}
	}
	if (fillY)
	{
		pd = (PARGBQuad)data->Scan0;
		ps = pd + radius * data->Width;
		PARGBQuad pd2 = ps + height * data->Width;
		PARGBQuad ps2 = pd2 - data->Width;
		for (y = 0; y < radius; y++)
		{
			for (x = 0; x < data->Width; *pd++ = ps[x], *pd2++ = ps2[x], x++);
		}
	}
}
//---------------------------------------------------------------------------

BOOL CanTransform(INT width, INT height, RECT &r)
{
	r.right += r.left;
	r.bottom += r.top;
	if (r.right > width) r.right = width;
	if (r.bottom > height) r.bottom = height;
	if (r.left > 0) r.right -= r.left; else r.left = 0;
	if (r.top > 0) r.bottom -= r.top; else r.top = 0;
	return r.right > 0 && r.bottom > 0;
}

BOOL GetTransformParams(INT dstWidth, INT dstHeight,
	INT srcWidth, INT srcHeight, TransformMatrix &matrix, RECT &dst, RECT &src)
{
	FLOAT fx, fy, fwidth, fheight;
	matrix.GetTransformSize(srcWidth, srcHeight, fx, fy, fwidth, fheight);
	matrix.Invert();
	dst.left = (LONG)fx;
	dst.top = (LONG)fy;
	dst.right = (LONG)(fwidth + fx + 0.999999f);
	dst.bottom = (LONG)(fheight + fy + 0.999999f);
	if (!CanTransform(dstWidth, dstHeight, dst))
		return FALSE;
	if (fx > 0 || fy > 0)
	{
		if (fx < 0) fx = 0;
		else if (fy < 0) fy = 0;
		matrix.Translate(fx, fy);
	}
	matrix.GetTransformSize(dst.right, dst.bottom, fx, fy, fwidth, fheight);
	src.left = (LONG)fx;
	src.top = (LONG)fy;
	src.right = (LONG)(fwidth + fx + 0.999999f);
	src.bottom = (LONG)(fheight + fy + 0.999999f);
	if (!CanTransform(srcWidth, srcHeight, src))
		return FALSE;
	if (fx > 0) matrix.GetElements().dx -= fx;
	if (fy > 0) matrix.GetElements().dy -= fy;
	return TRUE;
}
//---------------------------------------------------------------------------

// 执行图像数据几何变换
VOID ImageTransform(BitmapData *dest, INT x, INT y,
	CONST BitmapData *source, TransformMatrix *matrix, FLOAT alpha = 1.0f)
{
	INT alphaI = (INT)(alpha * 255);
	if (alphaI <= 0) return;
	if (alphaI > 255) alphaI = 255;

	// 复制几何变换矩阵对象
	TransformMatrix m(matrix);

	// 几何变换矩阵绝对增加平移量x, y
	m.GetElements().dx += x;
	m.GetElements().dy += y;

	// 逆转几何变换矩阵，计算并分别返回目标和源图像实际大小dstR和srcR
	RECT dstR, srcR;
	if (GetTransformParams(dest->Width, dest->Height,
		source->Width, source->Height, m, dstR, srcR) == FALSE)
		return;

	// 将浮点数扩大4096倍，采用定点数运算
	INT im11 = (INT)(m.GetElements().m11 * 4096.0f);
	INT im12 = (INT)(m.GetElements().m12 * 4096.0f);
	INT im21 = (INT)(m.GetElements().m21 * 4096.0f);
	INT im22 = (INT)(m.GetElements().m22 * 4096.0f);

	// 根据mode获取插值过程及边框扩展半径
	InterpolateMode mode = GetInterpolateMode(source);
	InterpolateProc ColorProc;
	INT radius = GetInterpolateProc(mode, ColorProc);

	BitmapData dst, src, exp, tmp;
	// 按dstR和srcR分别获取目标和源图像子图到dst和src
	GetBitmapData(dest, dstR.left, dstR.top, dstR.right, dstR.bottom, &dst);
	GetBitmapData(source, srcR.left, srcR.top, srcR.right, srcR.bottom, &src);
	// 建立扩展半径为radius新的图像数据对象exp
	GetBitmapData(src.Width + radius * 2, src.Height + radius * 2, &exp);
	// src图像数据拷贝到exp
	GetBitmapData(&exp, radius, radius, src.Width, src.Height, &tmp);
	CopyInterpolateData(&tmp, &src, alphaI);
	// 填充exp边框像素。如果im21或im12的12位尾数不为0，说明x或y向为斜边，不填充
	BOOL fillX = (im21 & 0xfff) == 0;
	BOOL fillY = (im12 & 0xfff) == 0;
	FillBorder(&exp, radius, fillX, fillY);
	if (fillX && fillY && alphaI == 255 && !HasAlphaFlag(source) &&
		dest->Width == dst.Width && dest->Height == dst.Height)
		SetAlphaFlag(dest, FALSE);
	// 确定源图像边界界限
	INT up = radius * 0x800;
	INT xDown = (exp.Width - radius) * 0x1000;
	INT yDown = (exp.Height - radius) * 0x1000;
	// 几何变换逆矩阵的平移量为与子图原点对应的源图起始坐标点
	INT xs = (INT)(m.GetElements().dx * 4096.0f) + up + 0x800;
	INT ys = (INT)(m.GetElements().dy * 4096.0f) + up + 0x800;
	INT width = (INT)dst.Width;
	INT height = (INT)dst.Height;

	PARGBQuad pd = (PARGBQuad)dst.Scan0;
	INT dstOffset = (dst.Stride >> 2) - dst.Width;
	// 如果插值方式为双立方卷积，初始化UV表
	if (mode == InterpolateModeBicubic)
		InitBicubicUVTable(-0.75);

	// 按目标子图逐点复制源子图几何变换后的数据
	for (y = 0; y < height; y++, ys += im22, xs += im21, pd += dstOffset)
	{
		INT y0 = ys;
		INT x0 = xs;
		for (x = 0; x < width; x++, x0 += im11, y0 += im12, pd++)
		{
			if (y0 >= up && y0 < yDown && x0 >= up && x0 < xDown)
			{
				MixerColor(pd, ColorProc(&exp, x0, y0));
			}
		}
	}
	FreeBitmapData(&exp);
}
//---------------------------------------------------------------------------
