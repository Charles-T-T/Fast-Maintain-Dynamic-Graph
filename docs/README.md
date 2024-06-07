# 补全代码任务

参考 $\mathrm{Fast \enspace exact \enspace shortest-path \enspace distance \enspace queries \enspace on \enspace large \enspace networks \enspace by \enspace pruned \enspace landmark \enspace labeling}$ 和 $\mathrm{Fast \enspace Maintenance \enspace of \enspace 2-hop \enspace Labels \enspace for \enspace Shortest \enspace Distance \enspace Queries \enspace on \enspace Fully \enspace Dynamic \enspace Graphs}$ 两篇论文，补全第二篇论文中，在边权减小、增加情况下快速维护最短路径索引的算法（即 $\textbf{Algorithm 3} \enspace \mathrm{The \enspace FastDeM \enspace algorithm} $ 和 $\textbf{Algorithm 4} \enspace \mathrm{The \enspace FastInM \enspace algorithm}$ ）对应的代码。

**需要补全代码的位置,在目录 `build_in_progress\HL\dynamic\` 下：**

1. `WeightDecreaseMaintenance_improv.h#DIFFUSE` 函数的TODO部分
2. `WeightIncreaseMaintenance_improv.h#SPREAD1` 函数的TODO部分
3. `WeightIncreaseMaintenance_improv.h#SPREAD2` 函数的TODO部分
4. `WeightIncreaseMaintenance_improv.h#SPREAD3` 函数的TODO部分

**除 `build_in_progress\HL\dynamic` 文件夹外, 其余文件均为辅助实现的部分代码, 可以不重点关注**。

下面对 `build_in_progress\HL\dynamic` 文件夹内文件及内部函数做简要释义。

## `test_dynamic.h` 

-  `test_dynamic` 
   - 测试函数，整个算法的调用入口在此，开头的注释部分注明了如何调用函数，以及编译运行的参数
   - 根据参数随机生成图，改变图的边权并执行索引动态维护算法，然后执行正确性检查
   - 其中含有多个控制参数便于debug测试，请详细读代码
-  `check_correctness_dynamic` 
   -  检查生成的索引正确性的函数，会被 `test_dynamic` 调用


-  `graph_change_and_label_maintenance` 
   -  改变图边权并动态维护索引的函数
   -  随机改变一条边的权重，然后调用索引维护的函数

## `PLL_dynamic.h` 

结合 $PPR$ 的 $PLL$ 算法的实现文件 

- `PLL_thread_function_dij_mixed` 
  - 从点 $v_k$ 执行剪枝的 $Dijkstra$ 算法

- `PLL_dynamic` 
  -  $PPL$ 实现函数，分为初始化和索引生成部分
  -  函数作用是生成索引和 $PPR$​ ，对应的伪代码参考阅读材料

## `PPR.h` 

-  `PPR_type` 
   - 自定义数据类型，为 $PPR$ 的数据类型
   - 一些 $PPR$ 的操作函数（插入、删除等）

## `two_hop_labels_base.h` 

- `two_hop_label_v1`
  - 索引结构
- `graph_hash_of_mixed_weighted_two_hop_case_info_v1` 
  - 存储 $PLL$ 相关参数和用时用量信息,主要用于传参
  - 同时存储生成的索引和PPR，因此想要查看生成的索引只需要调用 `print_L` 函数或 `print_PPR` 函数即可
- `insert_sorted_two_hop_label`
  - 向有序索引中插入元素的函数
- `search_sorted_two_hop_label` 和 `search_sorted_two_hop_label2`
  - 在索引中进行查找的函数 
- `affected_label` 
  - 边权变化后受影响的索引
- `graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc/2/3/4` 
  - 最短距离查询函数
  - 函数参数和返回值略有不同  

## `WeightDecreaseMaintenance_improv.h` 

是需要补全代码的文件，包含一个需要补全的函数。

- `WeightDecreaseMaintenance_improv_step1` 
  - 边权减小时的第一步操作，对应的伪代码参考阅读材料
- `DIFFUSE` 
  - 需要补全的函数，对应的伪代码参考阅读材料
- `WeightDecreaseMaintenance_improv`  
  - 边权减小的索引维护函数，会调用以上两个子函数
  - 被 `graph_change_and_label_maintenance` 调用

## `WeightIncreaseMaintenance_improv.h` 

是需要补全代码的文件，包含三个需要补全的函数。

- `SPREAD1` ,  `SPREAD2` ,  `SPREAD3` 
  - 需要补全的函数,对应的伪代码参考阅读材料
- `WeightIncreaseMaintenance_improv` 
  - 边权增大的索引维护函数，会调用以上三个 `SPREAD` 函数
  - 被 `graph_change_and_label_maintenance` 调用