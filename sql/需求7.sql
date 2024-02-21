/*• 显示
 每个车次显示（换乘需要同时显示2个车次）
– 车次
– 出发日期、出发时间、出发车站
– 到达日期、到达时间、到达车站
– 座位类型、本次车票价
 订票费：5元*车次数
 总票价
 注：通常有1组信息；对于换乘一次，有2组车次信息
• 用户点击确认，就生成订单
 记录到用户的历史订单中，修改车次对应的座位信息
 订单包含：订单号、上述车次、出发、到达、座位类型、票价、日期
和时间
• 用户点击取消，返回登录首页
*/
/*
插入完整记录
insert into 表名 values(值以逗号隔开，按create table 顺序 );
insert into Student
values (131234,'张飞',1995/1/1,M,'计算机',2013,85);

插入记录特定的列，其它列为空NULL 或默认值
insert into 表名(列名1，列名2,...) */

select 
ord_id,
ord_day,
ord_trid1,
ord_start1,
ord_departure1,
ord_arrive1,
ord_destation1,
ord_type1,
ord_price1,

ord_trid2,
ord_start2,
ord_departure2,
ord_arrive2,
ord_destation2,
ord_type2,
ord_price2,
ord_sum,
ord_state

from orders
where  ord_user = '票'
order by ord_id;

update count
set count_num = count_num + 1;

insert into order
values (
    '',
select * from count,
    20,
    '',
    0,
    '',
    0,
    '',
    '',
    0.0,
    '',
    0,
    '',
    0,
    '',
    '',
    0.0,
    0.0,
    0.0,
    1
);

update seat
set se_hardseat = se_hardseat - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);


update seat
set se_softseat = se_softseat - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleeperup = se_hardsleeperup - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleepermid = se_hardsleepermid - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleeperdown = se_hardsleeperdown - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_softsleeperup = se_softsleeperup - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_softsleeperdown = se_softsleeperdown - 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

delete from order
where ord_id = 1;


update seat
set se_hardseat = se_hardseat + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);


update seat
set se_softseat = se_softseat + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleeperup = se_hardsleeperup + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleepermid = se_hardsleepermid + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_hardsleeperdown = se_hardsleeperdown + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_softsleeperup = se_softsleeperup + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);

update seat
set se_softsleeperdown = se_softsleeperdown + 1
where se_id = '1'
and se_date = '2'
and se_station in (
    select pass_station
    from pass
    where pass_id = ''
    and pass_start > 0
    and pass_arrive <= 100
);