/*需求5：查询两地之间的车次
• 网页输入
 出发地城市名、到达地城市名
 出发日期，默认为查询时间的第二天
 出发时间，默认为00:00分 • 显示
 表格1：两地之间的直达列车和余票信息
 表格2：两地之间换乘一次的列车组合和余票信息
– 换乘地必须是同一城市
– 如果换乘地是同一车站，那么 1小时<= 换乘经停时间 <= 4小时
– 如果换乘地是同城的不同车站，那么 2小时<= 换乘经停时间 <= 4小时
– 显示两个列车的信息和总余票信息（两个列车余票的最小值）
 发车时间>=给定的出发时间
• 要求
 先显式直达表格，后显示换乘一次表格
 每个表格，按照先票价、再行程总时间、最后起始时间排序
 每个表格最多显示10行 • 余票上有链接，点击跳转到需求7网页
*/

create view emptyseat as

select p3.pass_id,
min(se_hardseat) as hardseat,
min(se_softseat) as softseat ,
min(se_hardsleeperup) as hardsleeperup,
min(se_hardsleepermid) as hardsleepermid,
min(se_hardsleeperdown) as hardsleeperdown,
min(se_softsleeperup) as softsleeperup,
min(se_softsleeperdown) as softsleeperdown

from pass as p1, pass as p2 ,pass as p3,seat,station as s1,station as s2
where s1.st_city = '泰安'
and s2.st_city = '苏州'
and p1.pass_station = s1.st_station
and p2.pass_station = s2.st_station 
and p1.pass_id = p2.pass_id
and p1.pass_start < p2.pass_start
and p3.pass_id = p1.pass_id
and p3.pass_start > p1.pass_start
and p3.pass_arrive <= p2.pass_arrive
and se_id = p1.pass_id
and se_date = 2
group by p3.pass_id;


select p1.pass_id, p1.pass_start,
p1.pass_station,p2.pass_station,
p2.pass_hardseat - p1.pass_hardseat as pass_hardseat , 
p2.pass_softseat - p1.pass_softseat  as pass_softseat,
p2.pass_hardsleeperup - p1.pass_hardsleeperup as pass_hardsleeperup,
p2.pass_hardsleepermid - p1.pass_hardsleepermid as pass_hardsleepermid,
p2.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass_hardsleeperdown,
p2.pass_softsleeperup - p1.pass_softsleeperup as pass_softsleeperup,
p2.pass_softsleeperdown - p1.pass_softsleeperdown as pass_softsleeperdown,
p2.pass_arrive - p1.pass_start as time_span,

(case
    when p2.pass_hardseat - p1.pass_hardseat  is NULL then 0
    when p2.pass_hardseat - p1.pass_hardseat  is not NULL then emptyseat.hardseat
    end
) as hardseat,

(case
    when p2.pass_softseat - p1.pass_softseat is NULL then 0
    when p2.pass_softseat - p1.pass_softseat is not NULL then emptyseat.softseat
    end
) as softseat,

(case
    when p2.pass_hardsleeperup - p1.pass_hardsleeperup is NULL then 0
    when p2.pass_hardsleeperup - p1.pass_hardsleeperup is not NULL then emptyseat.hardsleeperup
    end
) as hardsleeperup,

(case
    when p2.pass_hardsleepermid - p1.pass_hardsleepermid is NULL then 0
    when p2.pass_hardsleepermid - p1.pass_hardsleepermid is not NULL then emptyseat.hardsleepermid
    end
) as hardsleepermid,

(case
    when p2.pass_hardsleeperdown - p1.pass_hardsleeperdown is NULL then 0
    when p2.pass_hardsleeperdown - p1.pass_hardsleeperdown is not NULL then emptyseat.hardsleeperdown
    end
) as hardsleeperdown,

(case
    when p2.pass_softsleeperup - p1.pass_softsleeperup is NULL then 0
    when p2.pass_softsleeperup - p1.pass_softsleeperup is not NULL then emptyseat.hardsleeperdown
    end   
)as softsleeperup,

(case
    when p2.pass_softsleeperdown - p1.pass_softsleeperdown is NULL then 0
    when p2.pass_softsleeperdown - p1.pass_softsleeperdown is not NULL then emptyseat.softsleeperdown
    end
)as softsleeperdown


from pass as p1, pass as p2,seat,emptyseat,station as s1,station as s2
where s1.st_city = '泰安'
and s2.st_city = '苏州'
and p1.pass_station = s1.st_station
and p2.pass_station = s2.st_station 
and p1.pass_id = p2.pass_id  
and se_date = 2
and se_id = p1.pass_id
and se_station = p1.pass_station
and p1.pass_start < p2.pass_start
and p1.pass_id = emptyseat.pass_id
order by pass_hardseat,pass_softseat,pass_hardsleeperup,pass_hardsleepermid,pass_hardsleeperdown,
pass_softsleeperup,pass_softsleeperdown,time_span,p1.pass_start;

drop view emptyseat;