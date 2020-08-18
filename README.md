# cms.experiments
基于OPTICS密度聚类的变电设备状态监测异常点检测算法
网上很多关于OPTICS的介绍，这里做一个整理和自己的理解。
OPTICS是 Ordering Point To Idenfy the Cluster Structure 缩写。它是DBSCAN算法的一种改进算法，与DBSCAN算法相比，OPTICS算法对输入参数不敏感。
OPTICS算法不显式生成数据聚类，它对数据对象集合进行排序，得到一个有序的对象列表，但其中包含足够的信息来提取聚类 Cluster。所以针对它的这一特性，还可以对数据的分布和关联做进一步分析，它的排序队列可以应用到多领域的数据挖掘和分析中。
##  1.基本概念
ε：给定对象半径 ε 内的邻域称为该对象的 ε 邻域
minPts：如果对象的 ε 领域内至少包含最小数据minPts个对象，则该对象成为 核心对象。
直接密度可达距离(reachable-distance)： 1) max(core-distance,distance(p,q)),即p到q的距离小于  ε，这里的距离一般指欧式距离。
                                                           2) p 的 ε 邻域的对象数量>=minPts
### 公式：
    reachability-distance(p,q) = _| UNDEFINED |Nε(p)| < minPts
                                              | max{core-distance(p), distance(p,q)} |Nε(p)| >= minPts
    核心距离(core-distance)： p 为核心对象的 min( ε ) 使得 p 成为核心对象的最小邻域半径为核心距离
### 公式：                                     
    core-distance(p) = _| UNDEFINED  |Nε(p)| < minPts 
                                 | min(p, Nε(p)) |Nε(p)| >= minPts
    OPTICS算法生成一个有序对象列表，每个对象拥有两个属性，核心距离和可达距离。利用这两个列表可以获得任何邻域半径<  ε 的聚类

##  2.算法描述
OPTICS 算法的目标是输出数据集合datasets各元素的有序排列和每个元素的两个属性：core-distance, reachability-distance
伪代码：
```
struct datapoint{
     double* data;
     double reachable_distance;
     double core_distance;
};
```
核心流程：
datasets = set{ datapoint };
seedList 按可达距离排序队列
dpQue 核心对象队列
step 1.找到数据集中的核心对象
```
isCoreObject(datapoint, dataset)
{
for(dp : datasets)
{
    double distance_ = distance(datapoint,dp);
    if(distance_ <= ε)
    {
         distances.push_back(distance_);
         reacharrivableObject.push_back(dp);
    }
}

if(reacharrivableObject.size() >= minPts)
{
    for(item : reacharrivableObject)
    {
        item->setCoredistance(min(distance));
        item->setReachabledistance(max(coredistance,distances));
    }
}
}
```
step 2. 从核心对象的数据点出发，按照可达距离寻找非核心对象点,形成 seedList. 此过程中，dpQue在UNDEFINED标签datapoint加入后要进行从新排序
```
while(dpQue.IsNotEmpty())
{
    currentCorePoint = dpQue[0]; //取出第一个核心对象
    dpQue.remove(currentCorePoint); 
    seedList_.pushback(currentCorePoint);
    unlabeleddatalist = IsCoreObject(currentPoint, !dpQue); //在此非核心对象外找数据对象
    if(unlabeleddatalist.size() > 0)
    {
        for(unlabelItem : unlabeleddatalist)
        { 
            if(!IsContainedInSeedList(unlabelItem,seedList))
            {
                if( unlabelItem->reachability-distance > currentCorePoint->reachability-distance )
                     unlabelItem->setReachabledistance(currentCorePoint->reachability-distance);
            }
            else
            {
                dpQue.push_back(unlabelItem);
            }
            sort(dpQue);
        }
    }
}
```
step 3. 整个遍历后的dpQue，输出按可达距离排序的 seedList

##  3. 概念理解
 
    1.可达距离相对更重要，从某种意义上来说，可认为核心距离是为了定义可达距离而定义的。
    在待聚类的数据集中固定一个核心对象 x，则按照定义，数据集中的其它点关于 x 都有一个可达距离。注意，x 周边的点的分布通常是不均匀的，有的地方稠密，有的地方稀疏，现在我们希望从 x 出发，沿着稠密的方向做聚集操作，那么哪些点应该优先被考虑呢？答案是那些关于 x 的可达距离较小的点应该优先被考虑。为什么呢？
    参数 eps 和 minpts 给定后，核心距离 coredistance(x) 的值是确定的，且由定义可知，coredistance(x) 将 x 外的其它点分成了两类，即与 x 的距离小于等于 coredistance(x) 的点和 大于 coredistance(x) 的点，对于第一类点，它们的可达距离不作区分，都取为 coredistance(x)，因为它们都与 x 离得足够近，而对于第二类点，它们的可达距离就取为其与 x 的真实距离，显然，在这些点中，与 x 离得越近的点对应的可达距离越小。
    综上可见，可达距离本质上是对一个点与某个核心点之间的距离的一种刻画，但它不同于通常的距离，它引入了核心距离的概念，从而使得其与密度挂钩
    2.可达距离是不可能小于核心距离的，满足核心距离的条件是：只要核心对象在ε 领域内的对象数量多于minPts时，满足条件的距离即是核心距离，它不一定比给定的ε 大，可能刚好等于ε ，也可能小于ε 。这取决于minPts参数的选定。
    3.我们在选取核心对象的时候，只要满足minPts条件即可，不是必须找到核心对象内所有数量的对象再计算最大距离，那样的结果是核心距离=ε 。所以核心距离在OPTICS聚类中不是决定对象归属哪个簇的决定性因素，而是决定核心对象数量的参数。重要的是核心对象的可达距离才是决定其他对象归属于核心对象簇的决定性因素。
    4.形象的说：核心距离更像是密度特征（即我内核元素必须满足的条件或我成为核心对象的条件），可达距离更像是基于此密度的延伸（从内核分裂出去找寻属于自己对象簇元素的抓手）。所以OPTICS可以发掘任意形状簇，但对于每个形状簇的解释还需要进一步分析。
