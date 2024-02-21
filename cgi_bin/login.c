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
    char password[30];
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
    
    if(sscanf(data,"user=%[^&]&password=%s",user,password)!=2){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{

        sprintf(string,"select count(*) \
                     from passenger \
                     where pa_user = '%s' \
                     and pa_password = '%s'",user,password);
        PGresult *res = PQexec(conn,string);

        ExecStatusType status= PQresultStatus(res);
        printf("%s", PQresultErrorMessage(res));




        if(*(PQgetvalue(res,0,0)) != '0'){
            if(strcmp(user,"admin")){
                printf("<script>window.setTimeout(\"location.href = 'query.cgi?user=%s'\", 1000);</script>\n", user);
               
                printf("</div>\n");
                printf("</BODY>\n");
                printf("</HTML>\n");
                PQfinish(conn);
            }
            else{
                printf("<script>window.setTimeout(\"location.href = 'admin.cgi'\", 10);</script>\n");
                printf("</div>\n");
                printf("</BODY>\n");
                printf("</HTML>\n");
                PQfinish(conn);

            }        


        }
        else{
            printf("<script type=\"text/javascript\">alert('登录失败，用户名或密码不存在！');</script>\n");
            printf("<script>window.setTimeout(\"location.href = '../login.html'\", 10);</script>\n");
            
            printf("</div>\n");
            printf("</BODY>\n");
            printf("</HTML>\n");
            PQfinish(conn);

        }
        PQclear(res);
    }
    return 0;
}
