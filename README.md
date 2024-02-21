**写在最前**
本分支介绍了两个基础的大数据运算系统编程实验， 重点关注 1_hw2.java 和 1_hw2.cc 文件
其中 1_hw2.java 是典型的 MapReduce 编程，通过 Hadoop 运算系统，
分别传递 Map 阶段和 Reduce 阶段所需的参数， 重点关注
<inputKey, inputValue, outputKey, outputValue> 这个四元组的数据传递情况。
注意在重写 CompareTo 的对象比较时，需要把每一个 大于，小于，等于的返回值情况介绍清楚，
否则 Hadoop 在进行归并的时候会产生错误。

1_hw2.cc 是同步图运算系统
同步图运算实际只修改了 compute 函数，确定每个超步的变化值，得到最终收敛的结果。
