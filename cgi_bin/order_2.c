#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"
int main(int argc, char *argv[])
{
    PGconn* conn = PQconnectdb("host=127.0.0.1 dbname=train_12306 user=root password=123");

    char *data;
    char string[65536];
    char user[30];
    char train_id[20];
    char train_id2[20];
    int start_day;
    int start_day2;
    int start_time_1;
    int start_time1_1;
    int start_time_2;
    int start_time1_2;
    char start_station[20];
    char start_station2[20];
    int arrive_day;
    int arrive_day2;
    int arrive_time_1;
    int arrive_time1_1;
    int arrive_time_2;
    int arrive_time1_2;
    char arrive_station[20];
    char arrive_station2[20];
    char seat_type[20];
    char seat_type2[20];
    float seat_price;
    float seat_price2;
    float total_price;
    float total_price2;
    float sum_price;
    printf("Content-Type:text/html\n\n");
    printf("<HTML>\n");
    printf("<meta charset=\"UTF-8\">");
    printf("<HEAD>\n<TITLE >Get Method</TITLE>\n</HEAD>\n");
    printf("<BODY>\n");
    printf("<div style=\"font-size:12px\">\n");
    if (PQstatus(conn) != CONNECTION_OK) {
       printf("Connection to db failed: %s\n", PQerrorMessage(conn));
    }
    data = getenv("QUERY_STRING");
    urldecode(data);

    PGresult *res = PQexec(conn,"select * from count");

    if(sscanf(data,"user=%[^&]&train_id=%[^&]&start_day=%d&start_time=%d:%d&start_station=%[^&]&arrive_day=%d&arrive_time=%d:%d&arrive_station=%[^&]&seat_type=%[^&]&seat_price=%f&book_price=5&total_price=%f&train_id2=%[^&]&start_day2=%d&start_time2=%d:%d&start_station2=%[^&]&arrive_day2=%d&arrive_time2=%d:%d&arrive_station2=%[^&]&seat_type2=%[^&]&seat_price2=%f&book_price2=5&total_price2=%f&sum_price=%f", 
    user,
    train_id,
    &start_day,
    &start_time_1,&start_time_2,
    start_station,
    &arrive_day,
    &arrive_time_1,&arrive_time_2,
    arrive_station,
    seat_type,
    &seat_price,
    &total_price,
    train_id2,
    &start_day2,
    &start_time1_1,&start_time1_2,
    start_station2,
    &arrive_day2,
    &arrive_time1_1,&arrive_time1_2,
    arrive_station2,
    seat_type2,
    &seat_price2,
    &total_price2,
    &sum_price)!=26){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{
         sprintf(string,"update count \
            set count_num = count_num + 1; \
            insert into orders \
            values ( \
                '%s', \
                %s,\
                %d,\
                '%s',\
                %d,\
                '%s',\
                %d,\
                '%s',\
                '%s',\
                %f,\
                '%s',\
                %d,\
                '%s',\
                %d,\
                '%s',\
                '%s',\
                %f,\
                10,\
                %f,\
                true)",user, PQgetvalue(res, 0, 0) ,start_day,train_id,start_time_1 * 60 + start_time_2,start_station,
                arrive_time_1 * 60 + arrive_time_2,arrive_station,seat_type,seat_price,
                train_id2,start_time1_1*60 + start_time1_2,start_station2,
                arrive_time1_1*60 + arrive_time1_2,arrive_station2,seat_type2,seat_price2,
                seat_price + seat_price2 + 10);
         PQclear(res);
         res = PQexec(conn,string);
         ExecStatusType status= PQresultStatus(res);
         
        PQclear(res);
        if(strcmp(seat_type,"硬座")== 0){
            sprintf(string,"update seat \
                set se_hardseat = se_hardseat - 1 \
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
                res = PQexec(conn,string);
        }
        else if(strcmp(seat_type,"软座")== 0){
            sprintf(string,"update seat \
                se_softseat = se_softseat - 1 \
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
         else if(strcmp(seat_type,"硬卧上铺")== 0){
            sprintf(string,"update seat \
                se_hardsleeperup = se_hardsleeperup - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type,"硬卧中铺") == 0){
            sprintf(string,"update seat \
                se_hardsleepermid = se_hardsleepermid - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type,"硬卧下铺")== 0){
            sprintf(string,"update seat \
                se_hardsleeperdown = se_hardsleeperdown - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type,"软卧上铺")==0){
            sprintf(string,"update seat \
                se_softsleeperup = se_softsleeperup - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type,"软卧下铺")== 0){
            sprintf(string,"update seat \
                se_softsleeperdown = se_softsleeperdown - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id,start_day,train_id,start_time_1 * 60 + start_time_2,arrive_time_1 * 60 + arrive_time_2);
            res = PQexec(conn,string);
        }
        PQclear(res);
        if(strcmp(seat_type2,"硬座")== 0){
            sprintf(string,"update seat \
                set se_hardseat = se_hardseat - 1 \
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
                res = PQexec(conn,string);
        }
        else if(strcmp(seat_type2,"软座")== 0){
            sprintf(string,"update seat \
                se_softseat = se_softseat - 1 \
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
         else if(strcmp(seat_type2,"硬卧上铺")== 0){
            sprintf(string,"update seat \
                se_hardsleeperup = se_hardsleeperup - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type2,"硬卧中铺") == 0){
            sprintf(string,"update seat \
                se_hardsleepermid = se_hardsleepermid - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type2,"硬卧下铺")== 0){
            sprintf(string,"update seat \
                se_hardsleeperdown = se_hardsleeperdown - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type2,"软卧上铺")==0){
            sprintf(string,"update seat \
                se_softsleeperup = se_softsleeperup - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
        else if(strcmp(seat_type2,"软卧下铺")== 0){
            sprintf(string,"update seat \
                se_softsleeperdown = se_softsleeperdown - 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                    select pass_station \
                    from pass \
                    where pass_id = '%s' \
                    and pass_start > %d  \
                    and pass_arrive <= %d)",train_id2,start_day2,train_id2,start_time1_1 * 60 + start_time1_2,arrive_time1_1 * 60 + arrive_time1_2);
            res = PQexec(conn,string);
        }
        PQclear(res);
    printf("<script>window.setTimeout(\"location.href = 'query.cgi?user=%s'\", 10);</script>\n",user);
     }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
     PQfinish(conn);
    return 0;
}
