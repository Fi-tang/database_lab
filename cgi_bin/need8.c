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
    char day_start[20];
    char day_end[20];
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

    if(sscanf(data,"user=%[^&]&day_start=%[^&]&day_end=%s",user,day_start,day_end)!=3){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{

         sprintf(string,"select *\
         from orders\
         where ord_user='%s' \
         and ord_day >= %d \
         and ord_day <=%d",user,atoi(day_start),atoi(day_end));
         PGresult *res = PQexec(conn,string);
        

         ExecStatusType status= PQresultStatus(res);
         if (status!=PGRES_TUPLES_OK){
           printf("failed execution: %s\n", PQresultErrorMessage(res));
         }
        printf("<table ><caption align=\"top\">Retrieved %d rows * 20 fields</caption>",PQntuples(res));
        printf("<tr><th align=\"left\">用户</th><th align=\"right\">订单号</th><th align=\"left\">日期</th><th align=\"left\">第一列列车车次</th><th align=\"right\">第一列出发时间</th><th align=\"right\">第一列出发车站</th><th align=\"right\">第一列到达时间</th><th align=\"right\">第一列到达车站</th><th align=\"right\">第一列座位类型</th><th align=\"right\">第一列票价</th><th align=\"right\">第二列列车车次</th><th align=\"right\">第二列出发时间</th><th align=\"right\">第二列出发车站</th><th align=\"right\">第二列到达时间</th><th align=\"right\">第二列到达车站</th><th align=\"right\">第二列座位类型</th><th align=\"right\">第二列票价</th><th align=\"right\">订单价格</th><th align=\"right\">总价格</th><th align=\"right\">订单状态</th></tr>");
        int i,j;
        for(i = 0; i < PQntuples(res);i++){
            printf("<tr>");
            for(j = 0; j < 20;j++){
                if(j == 4 || j == 6 || j == 11 || j == 13){
                    printf("<th align=\"right\">%d:%d</th>",atoi(PQgetvalue(res,i,j))/60, atoi(PQgetvalue(res,i,j) )%60);
                }
                else{
                    printf("<th align=\"right\">%s</th>",PQgetvalue(res,i,j));
                }
            }
            if(*(PQgetvalue(res,i,10))!='\0'){
                printf("<th align=\"right\"><a href=\"cancel_2.cgi?user=%s&ord_id=%s&day=%d&train_id=%s&train_id2=%s&seat_type=%s&seat_type2=%s&start_time1=%d&arrive_time1=%d&start_time2=%d&arrive_time2=%d&data=%s\">取消</a></th>",PQgetvalue(res,i,0),PQgetvalue(res,i,1),atoi(PQgetvalue(res,i,2)),PQgetvalue(res,i,3),PQgetvalue(res,i,10),PQgetvalue(res,i,8),PQgetvalue(res,i,15),atoi(PQgetvalue(res,i,4)),atoi(PQgetvalue(res,i,6)),atoi(PQgetvalue(res,i,11)),atoi(PQgetvalue(res,i,13)),data);
            }
            else{
                printf("<th align=\"right\"><a href=\"cancel_1.cgi?user=%s&ord_id=%s&day=%d&train_id=%s&seat_type=%s&start_time1=%d&arrive_time1=%d&data=%s\">取消</a></th>",PQgetvalue(res,i,0),PQgetvalue(res,i,1),atoi(PQgetvalue(res,i,2)),PQgetvalue(res,i,3),PQgetvalue(res,i,8),atoi(PQgetvalue(res,i,4)),atoi(PQgetvalue(res,i,6)),data);
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
