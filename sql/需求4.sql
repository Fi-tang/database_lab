/*•网页输入
车次序号，例如G101
日期，例如：2022‐5‐1，默认为查询时间的第二天
•显示该车次所有信息
有静态信息
– 始发站，中间经停站，终点站
– 每站的发车时间和到达时间
– 票价
也有动态信息：每站余票（从始发站到当前站）
•每站余票上有超链接，点击跳转到需求7网页
预定始发站被点击的当前站的票*/


select pass_station,pass_arrive,pass_start,
pass_hardseat,pass_softseat,pass_hardsleeperup,
pass_hardsleepermid,pass_hardsleeperdown,
pass_softsleeperup,pass_softsleeperdown,se_date,
se_hardseat,se_softseat,se_hardsleeperup,se_hardsleepermid,
se_hardsleeperdown,se_softsleeperup,se_softsleeperdown

from pass,seat
where pass_id ='1095' and se_date=2 and se_id = pass_id and se_station = pass_station
order by pass_start;