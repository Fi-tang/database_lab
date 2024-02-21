基于数据库编写一个典型的 Web 应用：
Web 应用的三层结构中，
客户端展示层：采用html
应用逻辑中间层：采用 cgi
数据库系统的数据管理层：采用 SQL 进行查询。

应用的需求是仿照 12306 设计和实现一个火车订票系统，能够将数据存储在数据库中，提供基本的 Web 界面，
实现列车信息记录，列车座位记录，乘客信息记录，具体车次查询，两地之间车次查询，返程信息查询，
车次座位预订，查询订单和删除订单，管理员的9个需求。


数据库编程采用 libpq 编程库。

其中 cgi_bin文件夹下 采用嵌入式 sql 语句，通过 libpq 编程完成前端交付的查询任务。
html文件夹下负责展示用户查询界面，
sql文件夹负责将爬取的 .csv 文件中的数据进行表，视图的建立。
数据处理文件夹 利用 python 脚本，负责读取车辆信息，并整理成 sql 语句查询所需的格式。

一个简图说明完成9个查询任务需要的 html 页面和 cgi 应用逻辑中间层
![relation](https://github.com/Fi-tang/database_lab/blob/12306_web_application/12306.PNG)
