/*Admin登录后显示不同的登录首页
• Admin可以看到下述信息
总订单数（不包括已经取消订单）
总票价（不包括已经取消订单）
最热点车次排序，排名前10的车次（不包括已经取消订单）
当前注册用户列表
查看每个用户的订单
*/

create  view as order_trainid
select ord_trid1 as order_trainid_id

from orders

where ord_state = 1

union

select ord_trid2 as order_trainid_id
from orders
where ord_state = 1

select order_trainid_id
from order_trainid
group by order_trainid_id  desc limit 10

drop view order_trainid;

/*创建最热点车次排序*/

select count(ord_id),
sum(ord_sum)
from orders
where ord_state = 1;

select *
from orders
where ord_user = 'defalut name';

select pa_user
from passenger;