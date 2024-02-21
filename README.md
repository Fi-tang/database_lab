基于原型内存数据库系统AimDB 实现 select 查询处理功能。
完成数据库查询中的选择，投影，连接，以及分组聚集和排序

主要是按照面向对象的写法，明确本层需要从下层获得什么样的数据，同时本层又需要向上层提供什么样的数据。
限定一个大的抽象类 Operator, 分别具有 *lchild, *rchild, *parent 即左右孩子和父指针，
定义了虚函数 getNext()。
其余的选择，投影，连接，分组聚集和排序都需要继承这个 Operator 类，并完成各自的 getNext()。

其中 **AIMDB** 文件夹来自陈世敏老师团队的内存数据库系统，**OperatorTree** 文件夹则是自己实现的内容。
注释采用 doxygen 的格式编写和生成。