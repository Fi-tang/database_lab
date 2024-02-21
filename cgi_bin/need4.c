#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"
int main(int argc, char *argv[])
{
    PGconn* conn = PQconnectdb("host=127.0.0.1 dbname=train_12306 user=root password=123");

    char *data;
    char string[65536];
    char id[20];
    char user[30];
    int day;
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
    if(sscanf(data,"user=%[^&]&id=%[^&]&day=%d",user,id,&day)!=3){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{

         sprintf(string,"select pass_station,pass_arrive,pass_start,\
         pass_hardseat,pass_softseat,pass_hardsleeperup,\
         pass_hardsleepermid,pass_hardsleeperdown,\
        pass_softsleeperup,pass_softsleeperdown,se_date,\
         se_hardseat,se_softseat,se_hardsleeperup,se_hardsleepermid,\
         se_hardsleeperdown,se_softsleeperup,se_softsleeperdown\
         \
         from pass,seat\
         where pass_id ='%s' and se_date=%d and se_id = pass_id and se_station = pass_station\
         order by pass_start",id,day);
         PGresult *res = PQexec(conn,string);
         ExecStatusType status= PQresultStatus(res);
         if (status!=PGRES_TUPLES_OK){
           printf("failed execution: %s\n", PQresultErrorMessage(res));
         }

    printf("<table ><caption align=\"top\">Retrieved %d rows * 18 fields</caption>",PQntuples(res));
    printf("<tr><th align=\"left\">出发车站</th><th align=\"right\">到达时间</th><th align=\"right\">出发时间</th><th align=\"right\">硬座票价</th><th align=\"right\">软座票价</th><th align=\"right\">硬卧上铺票价</th><th align=\"right\">硬卧中铺票价</th><th align=\"right\">硬卧下铺票价</th><th align=\"right\">软卧上铺票价</th><th align=\"right\">软卧下铺票价</th><th align=\"right\">出发日期</th><th align=\"right\">硬座余票</th><th align=\"right\">软座余票</th><th align=\"right\">硬卧上铺余票</th><th align=\"right\">硬卧中铺余票</th><th align=\"right\">硬卧下铺余票</th><th align=\"right\">软卧上铺余票</th><th align=\"right\">软卧下铺余票</th></tr>");
    int i,j;
    for(i = 0; i < PQntuples(res);i++){
        printf("<tr>");
        for(j = 0; j < 18;j++){
            if(j == 0){
                printf("<th align=\"left\">%s</th>",PQgetvalue(res,i,j));
            }
            else if(j == 1 || j == 2){
                printf("<th align=\"right\">%d:%d</th>",atoi(PQgetvalue(res,i,j))/60, atoi(PQgetvalue(res,i,j) )%60);
            }
            else if(j >= 3 && j <= 10){
                printf("<th align=\"right\">%s</th>",PQgetvalue(res,i,j));
            }
            else{
                if(*(PQgetvalue(res,i,j-8)) != '\0' && *(PQgetvalue(res,i,j-8)) != '0'){
                    printf("<th align=\"right\"><a href=\"need7_1.cgi?user=%s&train_id=%s&start_day=%d&start_time=%d:%d&start_station=%s"
                    "&arrive_day=%d&arrive_time=%d:%d&arrive_station=%s&seat_type=%s&seat_price=%s&book_price=5&total_price=%f"
                    "\" target=\"_blank\">%s</a></th>",
                    user,
                    id,
                    day,
                    (atoi(PQgetvalue(res,0,2))/60),
                    (atoi(PQgetvalue(res,0,2) )%60),
                    PQgetvalue(res,0,0),
                    day,
                    (atoi(PQgetvalue(res,i,1))/60),
                    (atoi(PQgetvalue(res,i,1) )%60),
                    PQgetvalue(res,i,0),
                    (j == 11)?"硬座":(j == 12)?"软座":(j == 13)?"硬卧上铺":(j == 14)?"硬卧中铺":(j == 15)?"硬卧下铺":(j == 16)?"软卧上铺":"软卧下铺",
                    PQgetvalue(res,i,j-8),
                    (atof(PQgetvalue(res,i,j-8)) + 5),
                    PQgetvalue(res,i,j)
                    );
                }
                else{
                    printf("<th align=\"right\">%s</th>","0");
                }
            }
        }
        printf("</tr>");
    }

    printf("</table>");
    PQclear(res);

     }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
     PQfinish(conn);
    return 0;
}
