create table station(
    st_id integer,
    st_station varchar(20),
    st_city varchar(20),
    primary key(st_station)
);

create table train(
    tr_id varchar,
    tr_departure varchar(20),
    tr_destination varchar(20),
    tr_start int,
    tr_arrive int,
    primary key(tr_id),
    foreign key(tr_departure) references station(st_station),
    foreign key(tr_destination) references station(st_station)
);

create table pass(
    pass_id varchar(20),
    pass_station varchar(20),
    pass_arrive int DEFAULT NULL,
    pass_start int DEFAULT NULL, 
    pass_hardseat float DEFAULT NULL,
    pass_softseat float DEFAULT NULL,
    pass_hardsleeperup float DEFAULT NULL,
    pass_hardsleepermid float DEFAULT NULL,
    pass_hardsleeperdown float DEFAULT NULL,
    pass_softsleeperup float DEFAULT NULL,
    pass_softsleeperdown float DEFAULT NULL,
    primary key(pass_id,pass_station),
    foreign key (pass_id) references train(tr_id),
    foreign key(pass_station) references station(st_station)
);


create table seat(
    se_id varchar(20),
    se_date int,
    se_station varchar(20),
    se_hardseat float DEFAULT NULL,
    se_softseat float DEFAULT NULL,
    se_hardsleeperup float DEFAULT NULL,
    se_hardsleepermid float DEFAULT NULL,
    se_hardsleeperdown float DEFAULT NULL,
    se_softsleeperup float DEFAULT NULL,
    se_softsleeperdown float DEFAULT NULL,
    primary key(se_id,se_date,se_station),
    foreign key(se_id,se_station) references pass(pass_id,pass_station)
);

/*需要更新*/
create table passenger(
    pa_name varchar(20),
    pa_tele varchar(11),
    pa_user varchar(30),
    pa_password varchar(30),
    primary key(pa_user)
);


create table orders(
    ord_user varchar(30),
    ord_id integer,
    ord_day integer,
    ord_trid1 varchar(20),
    ord_start1 integer,
    ord_departure1 varchar(20),
    ord_arrive1 integer,
    ord_destation1 varchar(20),
    ord_type1 varchar(20),
    ord_price1 float,

    ord_trid2 varchar(20),
    ord_start2 integer,
    ord_departure2 varchar(20),
    ord_arrive2 integer,
    ord_destation2 varchar(20),
    ord_type2 varchar(20),
    ord_price2 float,

    ord_bookprice float,
    ord_sum float,
    ord_state bool,
    primary key(ord_id),
    foreign key(ord_user) references passenger(pa_user) 
);

create table count(
    count_num integer,
    primary key(count_num)
);