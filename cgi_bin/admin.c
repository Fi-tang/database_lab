#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"

int main(int argc, char *argv[])
{
    PGconn* conn = PQconnectdb("host=127.0.0.1 dbname=train_12306 user=root password=123");

    char string[65536];
    printf("Content-Type:text/html\n\n");
    printf("<HTML>\n");
    printf("<meta charset=\"UTF-8\">");
    printf("<HEAD>\n<TITLE >Get Method</TITLE>\n</HEAD>\n");
    printf("<BODY>\n");
    if (PQstatus(conn) != CONNECTION_OK) {
       printf("Connection to db failed: %s\n", PQerrorMessage(conn));
    }
    
    sprintf(string,"create  view order_trainid as \
        select ord_trid1 as order_trainid_id\
        \
        from orders\
        \
        where ord_state = true\
        and ord_trid1 is not null\
        \
        union all\
        \
        select ord_trid2 as order_trainid_id\
        from orders\
        where ord_state = true\
        and ord_trid2 is not null;\
        \
        select order_trainid_id, count(order_trainid_id) \
        from order_trainid\
        group by order_trainid_id  \
        order by count(order_trainid_id) desc limit 10");
    PGresult *res = PQexec(conn,string);

    ExecStatusType status= PQresultStatus(res);
    printf("%s", PQresultErrorMessage(res));

    PQprintOpt po = {0};
    po.header=1; po.align=1; po.fieldSep="|";
    po.html3 = 1;
    PQprint(stdout, res, &po);
    PQclear(res); 


    sprintf(string,"\
        select count(ord_id),\
        sum(ord_sum) \
        from orders \
        where ord_state = true");
    res = PQexec(conn,string);
    status = PQresultStatus(res);
    printf("%s",PQresultErrorMessage(res));

    PQprintOpt po1 = {0};
    po1.header=1; po1.align=1; po1.fieldSep="|";
    po1.html3 = 1;
    PQprint(stdout, res, &po1);

    PQclear(res); 

    sprintf(string,"select pa_user as current_registeruser, \
                    pa_name as name,\
                    pa_tele as telephone,\
                    pa_password as password \
            from passenger");
    res = PQexec(conn,string);
    status = PQresultStatus(res);
    printf("%s",PQresultErrorMessage(res));

    PQprintOpt po2 = {0};
    po2.header=1; po2.align=1; po2.fieldSep="|";
    po2.html3 = 1;
    PQprint(stdout, res, &po2);
    PQclear(res); 


    sprintf(string,"drop view order_trainid");
    res = PQexec(conn,string);


    printf("<table width=\"200\" height=\"180\" border=\"0\" style=\"font-size:12px\">\n");
    printf("<tr><td>\n");
    printf("<div style=\"font-weight:bold; font-size:15px\">Admin界面</div>\n");
    printf("<div>请输入需要查询的用户名:<div>\n");
    printf("<form method=\"get\" action=\"/cgi-bin/admin_find.cgi\">\n");
    printf("用户名:<input type=\"txt\" size=\"30\" name=\"user\">\n"); 
    printf("<br>\n");
    printf("<input type=\"submit\" value=\"用户订单查询结果\">\n");
    printf("</form>\n");
    printf("</td></tr>\n");
    printf("<tr><td>\n");
    printf("</table>\n");

    printf("</div>\n");
    printf("</BODY>\n");
    printf("</HTML>\n");
    PQfinish(conn);
    return 0;
}
