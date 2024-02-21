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
    printf("<div style=\"font-weight:bold; font-size:15px\">需求5测试</div>");
    printf("<div>请输入出发地城市名，到达地城市名，出发日期，出发时间:<div>");
    printf("<form action=\"\" name=\"form1\">");
    printf("<input type=\"hidden\" name=\"user\" value=\"%s\">",user);
    printf("出发地城市名:<input type=\"txt\" size=\"20\" name=\"start\"> ");
    printf("<br>");
    printf("<br>");
    printf("到达地城市名:<input type=\"txt\" size=\"20\" name=\"arrive\"> ");
    printf("<br>");
    printf("出发日期:<input type=\"txt\" size=\"2\" name=\"day\">");
    printf("<br>");
    printf("<br>");
    printf("出发时间:<input type=\"txt\" size=\"6\" name=\"time\">");
    printf("<input type=\"button\" value=\"直达列车\" type=\"submit\" onclick=\"form1.action='/cgi-bin/need5_1.cgi';form1.submit();\"/>");
    printf("<br>");
    printf("<input type=\"button\" value=\"换乘列车\" type=\"submit\" onclick=\"form1.action='/cgi-bin/need5_2.cgi';form1.submit();\"/>");
    printf("<br>");
    printf("<br>");
    printf("<input type=\"button\" value=\"返程直达列车\" type=\"submit\" onclick=\"form1.action='/cgi-bin/need6_1.cgi';form1.submit();\"/>");
    printf("<input type=\"button\" value=\"返程换乘列车\" type=\"submit\" onclick=\"form1.action='/cgi-bin/need6_2.cgi';form1.submit();\" />");
    printf("</form>");
    printf("</td></tr>");
    printf("<tr><td>");
    printf("</table>");
    }
    printf("</body>");
    printf("</html>");
    return 0;
}

