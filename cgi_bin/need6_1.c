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

        sprintf(string," create view emptyseat as \
        select p3.pass_id,\
        min(se_hardseat) as hardseat,\
        min(se_softseat) as softseat ,\
        min(se_hardsleeperup) as hardsleeperup,\
        min(se_hardsleepermid) as hardsleepermid,\
        min(se_hardsleeperdown) as hardsleeperdown,\
        min(se_softsleeperup) as softsleeperup,\
        min(se_softsleeperdown) as softsleeperdown\
\
       from pass as p1, pass as p2 ,pass as p3,seat,station as s1,station as s2 \
       where s1.st_city = '%s' \
       and s2.st_city = '%s'\
       and p1.pass_station = s1.st_station \
        and p2.pass_station = s2.st_station \
        and p1.pass_id = p2.pass_id\
        and p1.pass_start < p2.pass_start\
        and p3.pass_id = p1.pass_id\
        and p3.pass_start > p1.pass_start\
        and p3.pass_arrive <= p2.pass_arrive\
        and se_id = p1.pass_id\
        and se_date = %d\
        and p1.pass_start >= %d\
        group by p3.pass_id",arrive,start,day + 1,time);
        PGresult *res = PQexec(conn,string);

        ExecStatusType status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));

        PQclear(res);

        sprintf(string," select p1.pass_id, p1.pass_start,\
        p1.pass_station,p2.pass_station,\
        p2.pass_hardseat - p1.pass_hardseat as pass_hardseat , \
        p2.pass_softseat - p1.pass_softseat  as pass_softseat,\
        p2.pass_hardsleeperup - p1.pass_hardsleeperup as pass_hardsleeperup,\
        p2.pass_hardsleepermid - p1.pass_hardsleepermid as pass_hardsleepermid,\
        p2.pass_hardsleeperdown - p1.pass_hardsleeperdown as pass_hardsleeperdown,\
        p2.pass_softsleeperup - p1.pass_softsleeperup as pass_softsleeperup,\
        p2.pass_softsleeperdown - p1.pass_softsleeperdown as pass_softsleeperdown,\
        p2.pass_arrive - p1.pass_start as time_span,\
\
        (case\
            when p2.pass_hardseat - p1.pass_hardseat  is NULL then 0\
            when p2.pass_hardseat - p1.pass_hardseat  is not NULL then emptyseat.hardseat\
            end\
        ) as hardseat,\
\
        (case\
            when p2.pass_softseat - p1.pass_softseat is NULL then 0\
            when p2.pass_softseat - p1.pass_softseat is not NULL then emptyseat.softseat\
            end\
        ) as softseat,\
\
        (case\
            when p2.pass_hardsleeperup - p1.pass_hardsleeperup is NULL then 0\
            when p2.pass_hardsleeperup - p1.pass_hardsleeperup is not NULL then emptyseat.hardsleeperup\
            end\
        ) as hardsleeperup,\
\
        (case\
            when p2.pass_hardsleepermid - p1.pass_hardsleepermid is NULL then 0\
            when p2.pass_hardsleepermid - p1.pass_hardsleepermid is not NULL then emptyseat.hardsleepermid\
            end\
        ) as hardsleepermid,\
\
        (case\
            when p2.pass_hardsleeperdown - p1.pass_hardsleeperdown is NULL then 0\
            when p2.pass_hardsleeperdown - p1.pass_hardsleeperdown is not NULL then emptyseat.hardsleeperdown\
            end\
        ) as hardsleeperdown,\
\
        (case\
            when p2.pass_softsleeperup - p1.pass_softsleeperup is NULL then 0\
            when p2.pass_softsleeperup - p1.pass_softsleeperup is not NULL then emptyseat.softsleeperup\
            end   \
        )as softsleeperup,\
\
        (case\
            when p2.pass_softsleeperdown - p1.pass_softsleeperdown is NULL then 0\
            when p2.pass_softsleeperdown - p1.pass_softsleeperdown is not NULL then emptyseat.softsleeperdown\
            end\
        )as softsleeperdown\
        from pass as p1, pass as p2,seat,emptyseat,station as s1,station as s2\
        where s1.st_city = '%s'\
        and s2.st_city = '%s'\
        and p1.pass_station = s1.st_station\
        and p2.pass_station = s2.st_station \
        and p1.pass_id = p2.pass_id  \
        and se_date = %d\
        and p1.pass_start >= %d\
        and se_id = p1.pass_id\
        and se_station = p1.pass_station\
        and p1.pass_start < p2.pass_start\
        and p1.pass_id = emptyseat.pass_id\
        order by pass_hardseat,pass_softseat,pass_hardsleeperup,pass_hardsleepermid,pass_hardsleeperdown,\
        pass_softsleeperup,pass_softsleeperdown,time_span,p1.pass_start limit 10",arrive,start,day + 1,time);


        res = PQexec(conn,string);

        status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));



        printf("<table ><caption align=\"top\">Retrieved %d rows * 19 fields</caption>",PQntuples(res));
            printf("<tr><th align=\"left\">直达列车车次</th><th align=\"right\">出发时间</th><th align=\"left\">出发车站</th><th align=\"left\">到达车站</th><th align=\"right\">硬座价格</th><th align=\"right\">软座价格</th><th align=\"right\">硬卧上铺价格</th><th align=\"right\">硬卧中铺价格</th><th align=\"right\">硬卧下铺价格</th><th align=\"right\">软卧上铺价格</th><th align=\"right\">软卧下铺价格</th><th align=\"right\">列车总时长</th><th align=\"right\">硬座余票</th><th align=\"right\">软座余票</th><th align=\"right\">硬卧上铺余票</th><th align=\"right\">硬卧中铺余票</th><th align=\"right\">硬卧下铺余票</th><th align=\"right\">软卧上铺余票</th><th align=\"right\">软卧下铺余票</th></tr>");   
          int i,j;
        for(i = 0; i < PQntuples(res);i++){
            printf("<tr>");
            for(j = 0; j < 19;j++){
                if(j == 0){
                    printf("<th align=\"left\">%s</th>",PQgetvalue(res,i,j));
                }
                else if(j == 1 || j == 11){
                    printf("<th align=\"right\">%d:%d</th>",atoi(PQgetvalue(res,i,j))/60, atoi(PQgetvalue(res,i,j) )%60);
                }
                else if(j > 1 && j < 11){
                    printf("<th align=\"right\">%s</th>",PQgetvalue(res,i,j));
                }
                else{
                     if(*(PQgetvalue(res,i,j-8)) != '\0' && *(PQgetvalue(res,i,j-8)) != '0'){
                        printf("<th align=\"right\"><a href=\"need7_1.cgi?user=%s&train_id=%s&start_day=%d&start_time=%d:%d&start_station=%s"
                            "&arrive_day=%d&arrive_time=%d:%d&arrive_station=%s&seat_type=%s&seat_price=%s&book_price=5&total_price=%.2f"
                            "\" target=\"_blank\">%s</a></th>",
                            user,
                            PQgetvalue(res,i,0),
                            day + 1,
                            (atoi(PQgetvalue(res,i,1))/60),
                            (atoi(PQgetvalue(res,i,1) )%60),
                            PQgetvalue(res,i,2),
                            day + 1,
                            ((atoi(PQgetvalue(res,i,1)) + atoi(PQgetvalue(res,i,11))) /60 ),
                            ((atoi(PQgetvalue(res,i,1)) + atoi(PQgetvalue(res,i,11))) %60),
                            PQgetvalue(res,i,3),
                            (j == 12)?"硬座":(j == 13)?"软座":(j == 14)?"硬卧上铺":(j == 15)?"硬卧中铺":(j == 16)?"硬卧下铺":(j == 17)?"软卧上铺":"软卧下铺",
                            PQgetvalue(res,i,j-8),
                            (atof(PQgetvalue(res,i,j-8)) + 5),
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

        sprintf(string,"drop view emptyseat");
        res = PQexec(conn,string);

     }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
     PQfinish(conn);
    return 0;
}

