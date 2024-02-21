create view handle as

select p1.pass_id as first_id,
p1.pass_start as first_start,
p1.pass_station as start_station,
p3.pass_station as transfer_station1,
p3.pass_arrive as first_arrive,
p4.pass_station as transfer_station2,
p4.pass_start as second_start,
p2.pass_id as second_id, 
p2.pass_station as arrive_station,
p2.pass_arrive as second_arrive,

p3.pass_hardseat - p1.pass_hardseat as pass1_hardseat , 
p3.pass_softseat - p1.pass_softseat  as pass1_softseat,
p3.pass_hardsleeperup - p1.pass_hardsleeperup as pass1_hardsleeperup,
p3.pass_hardsleepermid - p1.pass_hardsleepermid as pass1_hardsleepermid,
p3.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass1_hardsleeperdown,
p3.pass_softsleeperup - p1.pass_softsleeperup as pass1_softsleeperup,
p3.pass_softsleeperdown - p1.pass_softsleeperdown as pass1_softsleeperdown,


p2.pass_hardseat - p4.pass_hardseat as pass2_hardseat, 
p2.pass_softseat - p4.pass_softseat  as pass2_softseat,
p2.pass_hardsleeperup - p4.pass_hardsleeperup as pass2_hardsleeperup,
p2.pass_hardsleepermid - p4.pass_hardsleepermid as pass2_hardsleepermid,
p2.pass_hardsleeperdown - p4.pass_hardsleeperdown as pass2_hardsleeperdown,
p2.pass_softsleeperup - p4.pass_softsleeperup as pass2_softsleeperup,
p2.pass_softsleeperdown - p4.pass_softsleeperdown as pass2_softsleeperdown,


p2.pass_arrive - p1.pass_start  as time_span



from pass as p1, pass as p2, pass as p3, pass as p4, 
station as s1, station as s2,station as s3, station as s4
where s1.st_city = '南京'
and s2.st_city = '苏州'
and p1.pass_start >= 0
and p1.pass_station = s1.st_station
and p2.pass_station = s2.st_station
and p1.pass_id != p2.pass_id
and p3.pass_id = p1.pass_id
and p4.pass_id = p2.pass_id
and s3.st_station = p3.pass_station
and s4.st_city = s3.st_city
and p4.pass_station = s4.st_station

and p4.pass_station != p3.pass_station
and p1.pass_start < p2.pass_arrive
and p3.pass_station != p1.pass_station
and p3.pass_station != p2.pass_station
and p4.pass_station != p1.pass_station
and p4.pass_station != p2.pass_station


and p1.pass_start < p3.pass_arrive
and p4.pass_start < p2.pass_arrive

and p4.pass_start - p3.pass_arrive >= 120
and p4.pass_start - p3.pass_arrive <= 240

union all


select p1.pass_id as first_id,
p1.pass_start as first_start,
p1.pass_station as start_station,
p3.pass_station as transfer_station1,
p3.pass_arrive as first_arrive,
p4.pass_station as transfer_station2,
p4.pass_start as second_start,
p2.pass_id as second_id,
p2.pass_station as arrive_station,
p2.pass_arrive as second_arrive,

p3.pass_hardseat - p1.pass_hardseat as pass1_hardseat , 
p3.pass_softseat - p1.pass_softseat  as pass1_softseat,
p3.pass_hardsleeperup - p1.pass_hardsleeperup as pass1_hardsleeperup,
p3.pass_hardsleepermid - p1.pass_hardsleepermid as pass1_hardsleepermid,
p3.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass1_hardsleeperdown,
p3.pass_softsleeperup - p1.pass_softsleeperup as pass1_softsleeperup,
p3.pass_softsleeperdown - p1.pass_softsleeperdown as pass1_softsleeperdown,


p2.pass_hardseat - p4.pass_hardseat as pass2_hardseat, 
p2.pass_softseat - p4.pass_softseat  as pass2_softseat,
p2.pass_hardsleeperup - p4.pass_hardsleeperup as pass2_hardsleeperup,
p2.pass_hardsleepermid - p4.pass_hardsleepermid as pass2_hardsleepermid,
p2.pass_hardsleeperdown - p4.pass_hardsleeperdown as pass2_hardsleeperdown,
p2.pass_softsleeperup - p4.pass_softsleeperup as pass2_softsleeperup,
p2.pass_softsleeperdown - p4.pass_softsleeperdown as pass2_softsleeperdown,


p2.pass_arrive - p1.pass_start  as time_span


from pass as p1, pass as p2, pass as p3, pass as p4, 
station as s1, station as s2
where s1.st_city = '南京'
and s2.st_city = '苏州'
and p1.pass_start >= 0
and p1.pass_station = s1.st_station
and p2.pass_station = s2.st_station
and p1.pass_id != p2.pass_id
and p3.pass_id = p1.pass_id
and p4.pass_id = p2.pass_id


and p4.pass_station = p3.pass_station
and p1.pass_start < p2.pass_arrive
and p3.pass_station != p1.pass_station
and p3.pass_station != p2.pass_station

and p1.pass_start < p3.pass_arrive
and p4.pass_start < p2.pass_arrive


and p4.pass_start - p3.pass_arrive >= 60
and p4.pass_start - p3.pass_arrive <= 240;


create view pass_union as

select handle.first_id as pass_union_first_id,
handle.start_station as pass_union_start_station,
handle.transfer_station1 as pass_union_transfer_station1,
handle.second_id as pass_union_second_id,
handle.transfer_station2 as pass_union_transfer_station2,
handle.arrive_station as pass_union_arrive_station,

seat1.se_hardseat as hardseat,
seat1.se_softseat as softseat,
seat1.se_hardsleeperup as hardsleeperup,
seat1.se_hardsleepermid as hardsleepermid,
seat1.se_hardsleeperdown as hardsleeperdown,
seat1.se_softsleeperup as softsleeperup,
seat1.se_softsleeperdown as softsleeperdown

from handle,pass as p5, seat as seat1

where p5.pass_id = handle.first_id
and p5.pass_start >= handle.first_start
and p5.pass_arrive <= handle.first_arrive
and seat1.se_id = p5.pass_id
and seat1.se_station = p5.pass_station
and seat1.se_date = 2

union all

select handle.first_id as pass_union_first_id,
handle.start_station as pass_union_start_station,
handle.transfer_station1 as pass_union_transfer_station1,
handle.second_id as pass_union_second_id,
handle.transfer_station2 as pass_union_transfer_station2,
handle.arrive_station as pass_union_arrive_station,

seat2.se_hardseat as hardseat,
seat2.se_softseat as softseat,
seat2.se_hardsleeperup as hardsleeperup,
seat2.se_hardsleepermid as hardsleepermid,
seat2.se_hardsleeperdown as hardsleeperdown,
seat2.se_softsleeperup as softsleeperup,
seat2.se_softsleeperdown as softsleeperdown

from handle,pass as p6, seat as seat2

where p6.pass_id = handle.second_id
and p6.pass_start >= handle.second_start
and p6.pass_arrive <= handle.second_arrive
and seat2.se_id = p6.pass_id
and seat2.se_station = p6.pass_station
and seat2.se_date = 2;


create view empty_union as

select u.pass_union_first_id as empty_union_first_id,
u.pass_union_start_station as empty_union_start_station,
u.pass_union_transfer_station1 as empty_union_transfer_station1,
u.pass_union_second_id as empty_union_second_id,
u.pass_union_transfer_station2 as empty_union_transfer_station2,
u.pass_union_arrive_station as empty_union_arrive_station,

min(u.hardseat) as hardseat,
min(u.softseat) as softseat ,
min(u.hardsleeperup) as hardsleeperup,
min(u.hardsleepermid) as hardsleepermid,
min(u.hardsleeperdown) as hardsleeperdown,
min(u.softsleeperup) as softsleeperup,
min(u.softsleeperdown) as softsleeperdown

from pass_union as u
group by u.pass_union_first_id,
u.pass_union_start_station,
u.pass_union_transfer_station1,
u.pass_union_second_id,
u.pass_union_transfer_station2,
u.pass_union_arrive_station;



select h.first_id as first_id,
h.first_start as first_start,
h.start_station as start_station,
h.transfer_station1 as transfer_station1,
h.first_arrive as first_arrive,
h.second_id as second_id,

h.second_start as second_start,
h.transfer_station2 as transfer_station2,

h.arrive_station as arrive_station,
h.second_arrive as second_arrive,
h.time_span as time_span,

least(h.pass1_hardseat,
h.pass1_softseat,
h.pass1_hardsleeperup,
h.pass1_hardsleepermid,
h.pass1_hardsleeperdown,
h.pass1_softsleeperup,
h.pass1_softsleeperdown) +

least(h.pass2_hardseat,
h.pass2_softseat,
h.pass2_hardsleeperup,
h.pass2_hardsleepermid,
h.pass2_hardsleeperdown,
h.pass2_softsleeperup,
h.pass2_softsleeperdown) as pass2_least,

least(u.hardseat,
u.softseat,
u.hardsleeperup,
u.hardsleepermid,
u.hardsleeperdown,
u.softsleeperup,
u.softsleeperdown) as seat_least
 

from handle as h ,empty_union as u


where  u.empty_union_first_id = h.first_id
and u.empty_union_start_station = h.start_station
and u.empty_union_transfer_station1 = h.transfer_station1
and u.empty_union_second_id = h.second_id
and u.empty_union_transfer_station2= h.transfer_station2
and u.empty_union_arrive_station = h.arrive_station

order by pass2_least,
time_span,
first_start;



drop view empty_union;
drop view pass_union;
drop view handle;