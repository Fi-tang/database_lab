**写在最前**
本次的内容比较简单，主要考察 5_hw1.java 文件
实验目标是熟悉大数据存储系统编程，
从 HDFS (Hadoop FileSystem) 这个文件系统中读取文件，实现 distinct 的中间处理，并且将
结果写入到 HBase 的大数据存储系统中。
这里的思路和 Operator_Tree 分支中的操作树很像，
从每个连接中读取一序列的数据，分别定义 distinct所需的中间对象，采用 getNext() 操作分别接收来自下层的数据，
并传递本层数据给上层。
基本思路一致，增加了对于大数据系统的 java API 的熟悉。

实验的设计步骤如下:

![producer](https://github.com/Fi-tang/database_lab/blob/Big_data_storage_programming/producer.PNG)
