需要补全代码的位置,在目录build_in_progress\HL\dynamic\下:
1.WeightDecreaseMaintenance_improv.h#DIFFUSE函数的TODO部分
2.WeightIncreaseMaintenance_improv.h#SPREAD1函数的TODO部分
3.WeightIncreaseMaintenance_improv.h#SPREAD2函数的TODO部分
4.WeightIncreaseMaintenance_improv.h#SPREAD3函数的TODO部分


除build_in_progress\HL\dynamic文件夹外, 其余文件均为辅助实现的部分代码, 可以不重点关注
下面对build_in_progress\HL\dynamic文件夹内文件及内部函数做简要释义

build_in_progress\HL\dynamic\test_dynamic.h
    --test_dynamic:
        测试函数, 整个算法的调用入口在此, 开头的注释部分注明了如何调用函数, 以及编译运行的参数
        根据参数随机生成图,改变图的边权并执行索引动态维护算法,然后执行正确性检查
        其中含有多个控制参数便于debug测试,请详细读代码
    --check_correctness_dynamic: 检查生成的索引正确性的函数,会被 test_dynamic 调用
    --graph_change_and_label_maintenance: 改变图边权并动态维护索引的函数.随机改变一条边的权重,然后调用索引维护的函数


build_in_progress\HL\dynamic\PLL_dynamic.h
    结合PPR的PLL算法的实现文件
    --PLL_thread_function_dij_mixed: 从点v_k执行剪枝的Dijkstra算法
    --PLL_dynamic: PLL实现函数,分为初始化和索引生成部分,函数作用是生成索引和PPR
    对应的伪代码参考阅读材料


build_in_progress\HL\dynamic\PPR.h
    --PPR_type: 自定义数据类型,为PPR的数据类型
    一些PPR的操作函数（插入、删除等）


build_in_progress\HL\dynamic\two_hop_labels_base.h
    --two_hop_label_v1: 索引结构
    --graph_hash_of_mixed_weighted_two_hop_case_info_v1: 存储PLL相关参数和用时用量信息,主要用于传参; 同时存储生成的索引和PPR,因此想要查看生成的索引只需要调用print_L函数或print_PPR函数即可
    --insert_sorted_two_hop_label: 向有序索引中插入元素的函数
    --search_sorted_two_hop_label和search_sorted_two_hop_label2: 在索引中进行查找的函数 
    --affected_label: 边权变化后受影响的索引
    --graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc/2/3/4: 最短距离查询函数,函数参数和返回值略有不同  

build_in_progress\HL\dynamic\WeightDecreaseMaintenance_improv.h
    是需要补全代码的文件,包含一个需要补全的函数
    --WeightDecreaseMaintenance_improv_step1: 边权减小时的第一步操作,对应的伪代码参考阅读材料
    --DIFFUSE: 需要补全的函数,对应的伪代码参考阅读材料
    --WeightDecreaseMaintenance_improv: 边权减小的索引维护函数,会调用以上两个子函数；被 graph_change_and_label_maintenance 调用


build_in_progress\HL\dynamic\WeightIncreaseMaintenance_improv.h
    是需要补全代码的文件,包含三个需要补全的函数
    --SPREAD1, SPREAD2, SPREAD3: 需要补全的函数,对应的伪代码参考阅读材料
    --WeightIncreaseMaintenance_improv: 边权增大的索引维护函数,会调用以上三个SPREAD函数；被 graph_change_and_label_maintenance 调用




