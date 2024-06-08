# 文件说明


`WeightDecreaseMaintenance_improv.h` ：边权减小情况下的标签索引维护


`WeightIncreaseMaintenance_improv.h` ：边权增加情况下的标签索引维护


`DuQueue.h` ：本组自行实现的特殊优先队列，在上面两个文件中调用；具体而言，用于 `DIFFUSE` 和 `SPREAD3` ，对应论文算法中的 $Q = \{(u\enspace|\enspace d_u)\}$ 
