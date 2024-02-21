#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"

int main(int argc, char *argv[])
{
    char *data;
    char user[30];
    printf("Content-Type:text/html\n\n");
    printf("<HTML>\n");
    printf("<meta charset=\"UTF-8\">");
    printf("<HEAD>\n<TITLE >Get Method</TITLE>\n</HEAD>\n");
    printf("<BODY>\n");
    data = getenv("QUERY_STRING");
    urldecode(data);
    
    if(sscanf(data,"user=%s",user)!=1){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{
    printf("<h2>查询首页</h2>");
    printf("<form action=\"\" name=\"Query\">");
    printf("<input type=\"hidden\" name=\"user\" value=\"%s\">",user);
    printf("<input type=\"button\" value=\"查询具体车次\" type=\"submit\" onclick=\"Query.action='need4_front.cgi';Query.submit();\"/>");
    printf("<br>");
    printf("<input type=\"button\" value=\"查询两地之间的车次\" type=\"submit\" onclick=\"Query.action='need5_front.cgi';Query.submit();\" />");
    printf("<br>");
    printf("<input type=\"button\" value=\"查询订单和删除订单\" type=\"submit\" onclick=\"Query.action='need8_front.cgi';Query.submit();\"/>");
    printf("</form>");
    }
    printf("</body>");
    printf("</html>");
    return 0;
}
