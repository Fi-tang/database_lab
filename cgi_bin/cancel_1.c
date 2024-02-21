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
    int ord_id;
    int day;
    char train_id[20];
    char seat_type[20];
    int start_time1;
    int arrive_time1;
    char data1[1024];
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
                   
    if(sscanf(data,"user=%[^&]&ord_id=%d&day=%d&train_id=%[^&]&seat_type=%[^&]&start_time1=%d&arrive_time1=%d&data=%s",user,&ord_id,&day,train_id,seat_type,&start_time1,
    &arrive_time1,data1)!=8){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{

         sprintf(string,"update orders\
         set ord_state = false \
         where ord_user = '%s' \
         and ord_id = %d",user,ord_id);
        PGresult *res = PQexec(conn,string);
        PQclear(res);


        if(strcmp(seat_type,"硬座")== 0){
            sprintf(string,"update seat \
                set se_hardseat = se_hardseat + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"软座")== 0){
            sprintf(string,"update seat \
                set se_softseat = se_softseat + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"硬卧上铺")== 0){
            sprintf(string,"update seat \
                set se_hardsleeperup = se_hardsleeperup + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"硬卧中铺")== 0){
            sprintf(string,"update seat \
                set se_hardsleepermid = se_hardsleepermid + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"硬卧下铺")== 0){
            sprintf(string,"update seat \
                set se_hardsleeperdown = se_hardsleeperdown + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"软卧上铺")== 0){
            sprintf(string,"update seat \
                set se_softsleeperup = se_softsleeperup + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            printf("%s",string);
            PQclear(res);
        }
        else if(strcmp(seat_type,"软卧下铺")== 0){
            sprintf(string,"update seat \
                set se_softsleeperdown = se_softsleeperdown + 1\
                where se_id = '%s'\
                and se_date = %d \
                and se_station in ( \
                        select pass_station \
                        from pass \
                        where pass_id = '%s' \
                        and pass_start > %d \
                        and pass_arrive <= %d)",train_id,day,train_id,start_time1,arrive_time1);
            res = PQexec(conn,string);
            PQclear(res);
        }



        printf("<script>window.setTimeout(\"location.href = 'need8.cgi?%s'\", 10);</script>\n", data1);

     }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
     PQfinish(conn);
    return 0;
}
