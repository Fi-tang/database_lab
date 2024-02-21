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
    
    printf("<table width=\"200\" height=\"180\" border=\"0\" style=\"font-size:12px\">");
    printf("<tr><td>");
    printf("<div style=\"font-weight:bold; font-size:15px\">需求4测试</div>");
    printf("<div>请输入列车信息和查询天数，天数需要从0~30计数:<div>");
    printf("<form method=\"get\" action=\"/cgi-bin/need4.cgi?user=%s\">",user);
    printf("<input type=\"hidden\" name=\"user\" value=\"%s\">",user);
    printf("列车号:<input type=\"txt\" size=\"20\" name=\"id\"> ");
    printf("<br>");
    printf("日期:<input type=\"txt\" size=\"2\" name=\"day\">"); 
    printf("<br>");
    printf("<input type=\"submit\" value=\"result\">");
    printf("</form>");
    printf("</td></tr>");
    printf("<tr><td>");
    printf("</table>");
    }
    printf("</body>");
    printf("</html>");
    return 0;
}

