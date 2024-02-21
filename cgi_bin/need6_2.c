#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"

int main(int argc, char *argv[])
{
    PGconn* conn = PQconnectdb("host=127.0.0.1 dbname=train_12306 user=root password=123");

    char *data;
    char string[65536];
    char start[20];
    char arrive[20];
    char user[30];
    int day;
    int time;
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
    
    if(sscanf(data,"user=%[^&]&start=%[^&]&arrive=%[^&]&day=%d&time=%d",user,start,arrive,&day,&time)!=5){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{

        sprintf(string," create view handle as \
        select p1.pass_id as first_id,\
        p1.pass_start as first_start,\
        p1.pass_station as start_station,\
        p3.pass_station as transfer_station1,\
        p3.pass_arrive as first_arrive,\
        p4.pass_station as transfer_station2,\
        p4.pass_start as second_start,\
        p2.pass_id as second_id, \
        p2.pass_station as arrive_station,\
        p2.pass_arrive as second_arrive,\
        \
        p3.pass_hardseat - p1.pass_hardseat as pass1_hardseat , \
        p3.pass_softseat - p1.pass_softseat  as pass1_softseat,\
        p3.pass_hardsleeperup - p1.pass_hardsleeperup as pass1_hardsleeperup,\
        p3.pass_hardsleepermid - p1.pass_hardsleepermid as pass1_hardsleepermid,\
        p3.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass1_hardsleeperdown,\
        p3.pass_softsleeperup - p1.pass_softsleeperup as pass1_softsleeperup,\
        p3.pass_softsleeperdown - p1.pass_softsleeperdown as pass1_softsleeperdown,\
        \
        \
        p2.pass_hardseat - p4.pass_hardseat as pass2_hardseat, \
        p2.pass_softseat - p4.pass_softseat  as pass2_softseat,\
        p2.pass_hardsleeperup - p4.pass_hardsleeperup as pass2_hardsleeperup,\
        p2.pass_hardsleepermid - p4.pass_hardsleepermid as pass2_hardsleepermid,\
        p2.pass_hardsleeperdown - p4.pass_hardsleeperdown as pass2_hardsleeperdown,\
        p2.pass_softsleeperup - p4.pass_softsleeperup as pass2_softsleeperup,\
        p2.pass_softsleeperdown - p4.pass_softsleeperdown as pass2_softsleeperdown,\
        \
        \
        p2.pass_arrive - p1.pass_start  as time_span\
        \
        \
        \
        from pass as p1, pass as p2, pass as p3, pass as p4, \
        station as s1, station as s2,station as s3, station as s4\
        where s1.st_city = '%s'\
        and s2.st_city = '%s'\
        and p1.pass_start >= %d\
        and p1.pass_station = s1.st_station\
        and p2.pass_station = s2.st_station\
        and p1.pass_id != p2.pass_id\
        and p3.pass_id = p1.pass_id\
        and p4.pass_id = p2.pass_id\
        and s3.st_station = p3.pass_station\
        and s4.st_city = s3.st_city\
        and p4.pass_station = s4.st_station\
        \
        and p4.pass_station != p3.pass_station\
        and p1.pass_start < p2.pass_arrive\
        and p3.pass_station != p1.pass_station\
        and p3.pass_station != p2.pass_station\
        and p4.pass_station != p1.pass_station\
        and p4.pass_station != p2.pass_station\
        \
        \
        and p1.pass_start < p3.pass_arrive\
        and p4.pass_start < p2.pass_arrive\
        \
        and p4.pass_start - p3.pass_arrive >= 120\
        and p4.pass_start - p3.pass_arrive <= 240\
        \
        union all\
        \
        \
        select p1.pass_id as first_id,\
        p1.pass_start as first_start,\
        p1.pass_station as start_station,\
        p3.pass_station as transfer_station1,\
        p3.pass_arrive as first_arrive,\
        p4.pass_station as transfer_station2,\
        p4.pass_start as second_start,\
        p2.pass_id as second_id,\
        p2.pass_station as arrive_station,\
        p2.pass_arrive as second_arrive,\
        \
        p3.pass_hardseat - p1.pass_hardseat as pass1_hardseat , \
        p3.pass_softseat - p1.pass_softseat  as pass1_softseat,\
        p3.pass_hardsleeperup - p1.pass_hardsleeperup as pass1_hardsleeperup,\
        p3.pass_hardsleepermid - p1.pass_hardsleepermid as pass1_hardsleepermid,\
        p3.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass1_hardsleeperdown,\
        p3.pass_softsleeperup - p1.pass_softsleeperup as pass1_softsleeperup,\
        p3.pass_softsleeperdown - p1.pass_softsleeperdown as pass1_softsleeperdown,\
        \
        \
        p2.pass_hardseat - p4.pass_hardseat as pass2_hardseat, \
        p2.pass_softseat - p4.pass_softseat  as pass2_softseat,\
        p2.pass_hardsleeperup - p4.pass_hardsleeperup as pass2_hardsleeperup,\
        p2.pass_hardsleepermid - p4.pass_hardsleepermid as pass2_hardsleepermid,\
        p2.pass_hardsleeperdown - p4.pass_hardsleeperdown as pass2_hardsleeperdown,\
        p2.pass_softsleeperup - p4.pass_softsleeperup as pass2_softsleeperup,\
        p2.pass_softsleeperdown - p4.pass_softsleeperdown as pass2_softsleeperdown,\
        \
        \
        p2.pass_arrive - p1.pass_start  as time_span\
        \
        \
        from pass as p1, pass as p2, pass as p3, pass as p4, \
        station as s1, station as s2\
        where s1.st_city = '%s'\
        and s2.st_city = '%s'\
        and p1.pass_start >= %d\
        and p1.pass_station = s1.st_station\
        and p2.pass_station = s2.st_station\
        and p1.pass_id != p2.pass_id\
        and p3.pass_id = p1.pass_id\
        and p4.pass_id = p2.pass_id\
        \
        \
        and p4.pass_station = p3.pass_station\
        and p1.pass_start < p2.pass_arrive\
        and p3.pass_station != p1.pass_station\
        and p3.pass_station != p2.pass_station\
        \
        and p1.pass_start < p3.pass_arrive\
        and p4.pass_start < p2.pass_arrive\
        \
        \
        and p4.pass_start - p3.pass_arrive >= 60\
        and p4.pass_start - p3.pass_arrive <= 240",arrive,start,day + 1,arrive,start,day + 1);
        PGresult *res = PQexec(conn,string);

        ExecStatusType status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));

        PQclear(res);

       sprintf(string," create view pass_union as\
        \
        select handle.first_id as pass_union_first_id,\
        handle.start_station as pass_union_start_station,\
        handle.transfer_station1 as pass_union_transfer_station1,\
        handle.second_id as pass_union_second_id,\
        handle.transfer_station2 as pass_union_transfer_station2,\
        handle.arrive_station as pass_union_arrive_station,\
        \
        seat1.se_hardseat as hardseat,\
        seat1.se_softseat as softseat,\
        seat1.se_hardsleeperup as hardsleeperup,\
        seat1.se_hardsleepermid as hardsleepermid,\
        seat1.se_hardsleeperdown as hardsleeperdown,\
        seat1.se_softsleeperup as softsleeperup,\
        seat1.se_softsleeperdown as softsleeperdown\
        \
        from handle,pass as p5, seat as seat1\
        \
        where p5.pass_id = handle.first_id\
        and p5.pass_start >= handle.first_start\
        and p5.pass_arrive <= handle.first_arrive\
        and seat1.se_id = p5.pass_id\
        and seat1.se_station = p5.pass_station\
        and seat1.se_date = %d\
        \
        union all\
        \
        select handle.first_id as pass_union_first_id,\
        handle.start_station as pass_union_start_station,\
        handle.transfer_station1 as pass_union_transfer_station1,\
        handle.second_id as pass_union_second_id,\
        handle.transfer_station2 as pass_union_transfer_station2,\
        handle.arrive_station as pass_union_arrive_station,\
        \
        seat2.se_hardseat as hardseat,\
        seat2.se_softseat as softseat,\
        seat2.se_hardsleeperup as hardsleeperup,\
        seat2.se_hardsleepermid as hardsleepermid,\
        seat2.se_hardsleeperdown as hardsleeperdown,\
        seat2.se_softsleeperup as softsleeperup,\
        seat2.se_softsleeperdown as softsleeperdown\
        \
        from handle,pass as p6, seat as seat2\
        \
        where p6.pass_id = handle.second_id\
        and p6.pass_start >= handle.second_start\
        and p6.pass_arrive <= handle.second_arrive\
        and seat2.se_id = p6.pass_id\
        and seat2.se_station = p6.pass_station\
        and seat2.se_date = %d",day + 1, day + 1);
        res = PQexec(conn,string);
        status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));

        PQclear(res);


        sprintf(string,"create view empty_union as\
        \
        select u.pass_union_first_id as empty_union_first_id,\
        u.pass_union_start_station as empty_union_start_station,\
        u.pass_union_transfer_station1 as empty_union_transfer_station1,\
        u.pass_union_second_id as empty_union_second_id,\
        u.pass_union_transfer_station2 as empty_union_transfer_station2,\
        u.pass_union_arrive_station as empty_union_arrive_station,\
        \
        min(u.hardseat) as hardseat,\
        min(u.softseat) as softseat ,\
        min(u.hardsleeperup) as hardsleeperup,\
        min(u.hardsleepermid) as hardsleepermid,\
        min(u.hardsleeperdown) as hardsleeperdown,\
        min(u.softsleeperup) as softsleeperup,\
        min(u.softsleeperdown) as softsleeperdown\
        \
        from pass_union as u\
        group by u.pass_union_first_id,\
        u.pass_union_start_station,\
        u.pass_union_transfer_station1,\
        u.pass_union_second_id,\
        u.pass_union_transfer_station2,\
        u.pass_union_arrive_station");

        res = PQexec(conn,string);
        status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));

        PQclear(res);

        sprintf(string,"select h.first_id as first_id,\
        h.first_start as first_start,\
        h.start_station as start_station,\
        h.transfer_station1 as transfer_station1,\
        h.first_arrive as first_arrive,\
        h.second_id as second_id,\
\
        h.second_start as second_start,\
        h.transfer_station2 as transfer_station2,\
\
        h.arrive_station as arrive_station,\
        h.second_arrive as second_arrive,\
        h.time_span as time_span,\
\
        h.pass1_hardseat,\
        h.pass1_softseat,\
        h.pass1_hardsleeperup,\
        h.pass1_hardsleepermid,\
        h.pass1_hardsleeperdown,\
        h.pass1_softsleeperup,\
        h.pass1_softsleeperdown,\
\
        h.pass2_hardseat,\
        h.pass2_softseat,\
        h.pass2_hardsleeperup,\
        h.pass2_hardsleepermid,\
        h.pass2_hardsleeperdown,\
        h.pass2_softsleeperup,\
        h.pass2_softsleeperdown,\
\
        least(h.pass1_hardseat,\
        h.pass1_softseat,\
        h.pass1_hardsleeperup,\
        h.pass1_hardsleepermid,\
        h.pass1_hardsleeperdown,\
        h.pass1_softsleeperup,\
        h.pass1_softsleeperdown) as pass1_least,\
\
        least(h.pass2_hardseat,\
        h.pass2_softseat,\
        h.pass2_hardsleeperup,\
        h.pass2_hardsleepermid,\
        h.pass2_hardsleeperdown,\
        h.pass2_softsleeperup,\
        h.pass2_softsleeperdown) as pass2_least,\
\
        least(h.pass1_hardseat,\
        h.pass1_softseat,\
        h.pass1_hardsleeperup,\
        h.pass1_hardsleepermid,\
        h.pass1_hardsleeperdown,\
        h.pass1_softsleeperup,\
        h.pass1_softsleeperdown)\
        +\
        least(h.pass2_hardseat,\
        h.pass2_softseat,\
        h.pass2_hardsleeperup,\
        h.pass2_hardsleepermid,\
        h.pass2_hardsleeperdown,\
        h.pass2_softsleeperup,\
        h.pass2_softsleeperdown) as pass_least,\
\
        least(u.hardseat,\
        u.softseat,\
        u.hardsleeperup,\
        u.hardsleepermid,\
        u.hardsleeperdown,\
        u.softsleeperup,\
        u.softsleeperdown) as seat_least\
        \
\
        from handle as h ,empty_union as u\
\
\
        where  u.empty_union_first_id = h.first_id\
        and u.empty_union_start_station = h.start_station\
        and u.empty_union_transfer_station1 = h.transfer_station1\
        and u.empty_union_second_id = h.second_id\
        and u.empty_union_transfer_station2= h.transfer_station2\
        and u.empty_union_arrive_station = h.arrive_station\
\
        order by pass_least,\
        time_span,\
        first_start limit 10");
        res = PQexec(conn,string);
        status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));

       printf("<table ><caption align=\"top\">Retrieved %d rows * 29 fields</caption>",PQntuples(res));
        printf("<tr><th align=\"left\">第一列列车车次</th><th align=\"right\">第一列出发时间</th><th align=\"left\">第一列出发车站</th><th align=\"left\">第一列到达车站</th><th align=\"right\">第一列到达时间</th><th align=\"right\">第二列列车车次</th><th align=\"right\">第二列出发时间</th><th align=\"right\">第二列出发车站</th><th align=\"right\">第二列到达车站</th><th align=\"right\">第二列到达时间</th><th align=\"right\">总时长</th><th align=\"right\">第一列硬座票价</th><th align=\"right\">第一列软座票价</th><th align=\"right\">第一列硬卧上铺票价</th><th align=\"right\">第一列硬卧中铺票价</th><th align=\"right\">第一列硬卧下铺票价</th><th align=\"right\">第一列软卧上铺票价</th><th align=\"right\">第一列软卧下铺票价</th><th align=\"right\">第二列硬座票价</th><th align=\"right\">第二列软座票价</th><th align=\"right\">第二列硬卧上铺票价</th><th align=\"right\">第二列硬卧中铺票价</th><th align=\"right\">第二列硬卧下铺票价</th><th align=\"right\">第二列软卧上铺票价</th><th align=\"right\">第二列软卧下铺票价</th><th align=\"right\">第一列最低票价</th><th align=\"right\">第二列最低票价</th><th align=\"right\">最低票价总和</th><th align=\"right\">余票座位数</th></tr>");   
        int i,j;
        for(i = 0; i < PQntuples(res);i++){
            printf("<tr>");
            for(j = 0; j < 29;j++){
                if(j == 0){
                    printf("<th align=\"left\">%s</th>",PQgetvalue(res,i,j));
                }
                else if(j == 1 || j == 4 || j == 6 || j == 9 || j == 10){
                    printf("<th align=\"right\">%d:%d</th>",atoi(PQgetvalue(res,i,j))/60, atoi(PQgetvalue(res,i,j) )%60);
                }
                else if(j!= 0 && j!= 1 && j!= 4 && j!= 6 && j!= 9 && j!= 10 && j!= 28){
                    printf("<th align=\"right\">%s</th>",PQgetvalue(res,i,j));
                }
                else{
                    if(*(PQgetvalue(res,i,j)) != '\0' && *(PQgetvalue(res,i,j)) != '0'){
                        printf("<th align=\"right\"><a href=\"need7_2.cgi?user=%s&train_id=%s&start_day=%d&start_time=%d:%d&start_station=%s"
                            "&arrive_day=%d&arrive_time=%d:%d&arrive_station=%s&seat_type=%s&seat_price=%s&book_price=5&total_price=%.2f"
                            "&train_id2=%s&start_day2=%d&start_time2=%d:%d&start_station2=%s"
                            "&arrive_day2=%d&arrive_time2=%d:%d&arrive_station2=%s&seat_type2=%s&seat_price2=%s&book_price2=5&total_price2=%.2f"
                            "&sum_price=%.2f"
                            "\" target=\"_blank\">%s</a></th>",
                            user,
                            PQgetvalue(res,i,0),
                            day + 1,
                             (atoi(PQgetvalue(res,i,1))/60 ),
                            (atoi(PQgetvalue(res,i,1) )%60),
                            PQgetvalue(res,i,2),
                             day + 1,
                            (atoi(PQgetvalue(res,i,4))/60 ),
                            (atoi(PQgetvalue(res,i,4) )%60),
                            PQgetvalue(res,i,3),
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,11)) )?"硬座":
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,12)))?"软座":
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,13)))?"硬卧上铺":
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,14)))?"硬卧中铺":
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,15)))?"硬卧下铺":
                            (*(PQgetvalue(res,i,25)) ==*(PQgetvalue(res,i,16)))?"软卧上铺":"软卧下铺",
                            PQgetvalue(res,i,25),
                            (atof(PQgetvalue(res,i,25)) + 5),
                            PQgetvalue(res,i,5),
                            day + 1,
                            (atoi(PQgetvalue(res,i,6))/60),
                            (atoi(PQgetvalue(res,i,6) )%60),
                            PQgetvalue(res,i,7),
                            day + 1,
                            (atoi(PQgetvalue(res,i,9))/60 ),
                            (atoi(PQgetvalue(res,i,9) )%60),
                            PQgetvalue(res,i,8),
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,18)) )?"硬座":
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,19)))?"软座":
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,20)))?"硬卧上铺":
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,21)))?"硬卧中铺":
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,22)))?"硬卧下铺":
                            (*(PQgetvalue(res,i,26)) ==*(PQgetvalue(res,i,23)))?"软卧上铺":"软卧下铺",
                            PQgetvalue(res,i,26),
                            (atof(PQgetvalue(res,i,26)) + 5),
                            (atof(PQgetvalue(res,i,25)) + atof(PQgetvalue(res,i,26)) + 10),
                            PQgetvalue(res,i,j)
                            );
                    }
                    else{
                        printf("<th align=\"right\">%s</th>",PQgetvalue(res,i,j));
                    }
                }
            }
            printf("</tr>");
        }

        printf("</table>");

        PQclear(res); 

        sprintf(string,"drop view empty_union");
        res = PQexec(conn,string);
        
        sprintf(string,"drop view pass_union");
        res = PQexec(conn,string);
        
        sprintf(string,"drop view handle");
        res = PQexec(conn,string);

     }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
     PQfinish(conn);
    return 0;
}

