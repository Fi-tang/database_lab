/*
乘客可以查询历史订单
给定出发日期范围，显示订单列表
订单信息：订单号，日期、出发到达站、总票价、订单状
态（正常/取消）
提供链接，点击显示订单具体信息，当订单包含2个车次
时，显示每个车次的信息
提供链接，点击取消订单
– 取消的订单，在订单列表中仍将显示，但注明取消
*/

select ord_id,
ord_day,
ord_trid1,
ord_departure1,
ord_destation1,

ord_trid2,
ord_departure2,
ord_destation2,

ord_sum,
ord_state

from orders

where ord_day >= 2
and ord_day <= 3
and ord_user = '票';

update orders
set ord_state = 0
where ord_id = 123;