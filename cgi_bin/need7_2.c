#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include"input_standard.h"

int main(int argc, char *argv[])
{
    char *data;
    char user[30];
    char train_id[20];
    char train_id2[20];
    int start_day;
    int start_day2;
    int start_time_1;
    int start_time1_1;
    int start_time_2;
    int start_time1_2;
    char start_station[20];
    char start_station2[20];
    int arrive_day;
    int arrive_day2;
    int arrive_time_1;
    int arrive_time1_1;
    int arrive_time_2;
    int arrive_time1_2;
    char arrive_station[20];
    char arrive_station2[20];
    char seat_type[20];
    char seat_type2[20];
    float seat_price;
    float seat_price2;
    float total_price;
    float total_price2;
    float sum_price;
    printf("Content-Type:text/html\n\n");
    printf("<HTML>\n");
    printf("<meta charset=\"UTF-8\">");
    printf("<HEAD>\n<TITLE >Get Method</TITLE>\n</HEAD>\n");
    printf("<BODY>\n");
    printf("<div style=\"font-size:12px\">\n");
    
    data = getenv("QUERY_STRING");
    urldecode(data);
    
    if(sscanf(data,"user=%[^&]&train_id=%[^&]&start_day=%d&start_time=%d:%d&start_station=%[^&]&arrive_day=%d&arrive_time=%d:%d&arrive_station=%[^&]&seat_type=%[^&]&seat_price=%f&book_price=5&total_price=%f&train_id2=%[^&]&start_day2=%d&start_time2=%d:%d&start_station2=%[^&]&arrive_day2=%d&arrive_time2=%d:%d&arrive_station2=%[^&]&seat_type2=%[^&]&seat_price2=%f&book_price2=5&total_price2=%f&sum_price=%f", 
    user,
    train_id,
    &start_day,
    &start_time_1,&start_time_2,
    start_station,
    &arrive_day,
    &arrive_time_1,&arrive_time_2,
    arrive_station,
    seat_type,
    &seat_price,
    &total_price,
    train_id2,
    &start_day2,
    &start_time1_1,&start_time1_2,
    start_station2,
    &arrive_day2,
    &arrive_time1_1,&arrive_time1_2,
    arrive_station2,
    seat_type2,
    &seat_price2,
    &total_price2,
    &sum_price)!=26){
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }
    else{
    printf("<table ><caption align=\"top\">Retrieved 2 rows * 12 fields</caption>");
    printf("<tr><th align=\"left\">用户</th><th align=\"right\">列车车次</th><th align=\"right\">出发日期</th><th align=\"right\">出发时间</th><th align=\"right\">出发车站</th><th align=\"right\">到达日期</th><th align=\"right\">到达时间</th><th align=\"right\">到达车站</th><th align=\"right\">座位类型</th><th align=\"right\">座位票价</th><th align=\"right\">订票费用</th><th align=\"right\">总费用</th><th align=\"right\">两列车总费用</th></tr>");
    printf("<tr>");
    printf("<th align=\"left\">%s</th>",user);
    printf("<th align=\"right\">%s</th>",train_id);
    printf("<th align=\"right\">%d</th>",start_day);
    printf("<th align=\"right\">%d:%d</th>",start_time_1,start_time_2);
    printf("<th align=\"right\">%s</th>",start_station);
    printf("<th align=\"right\">%d</th>",arrive_day);
    printf("<th align=\"right\">%d:%d</th>",arrive_time_1,arrive_time_2);
    printf("<th align=\"right\">%s</th>",arrive_station);
    printf("<th align=\"right\">%s</th>",seat_type);
    printf("<th align=\"right\">%f</th>",seat_price);
    printf("<th align=\"right\">5</th>");
    printf("<th align=\"right\">%f</th>",total_price);
    printf("<th align=\"right\">%f</th>",sum_price);
               
    printf("</tr>");
    printf("<tr>");
    printf("<th align=\"left\">%s</th>",user);
    printf("<th align=\"right\">%s</th>",train_id2);
    printf("<th align=\"right\">%d</th>",start_day2);
    printf("<th align=\"right\">%d:%d</th>",start_time1_1,start_time1_2);
    printf("<th align=\"right\">%s</th>",start_station2);
    printf("<th align=\"right\">%d</th>",arrive_day2);
    printf("<th align=\"right\">%d:%d</th>",arrive_time1_1,arrive_time1_2);
    printf("<th align=\"right\">%s</th>",arrive_station2);
    printf("<th align=\"right\">%s</th>",seat_type2);
    printf("<th align=\"right\">%f</th>",seat_price2);
    printf("<th align=\"right\">5</th>");
    printf("<th align=\"right\">%f</th>",total_price2);
    printf("<th align=\"right\">%f</th>",sum_price);
               
    printf("</tr>");

    printf("<th align=\"right\"><a href=\"order_2.cgi?user=%s&train_id=%s&start_day=%d&start_time=%d:%d&start_station=%s"
                            "&arrive_day=%d&arrive_time=%d:%d&arrive_station=%s&seat_type=%s&seat_price=%f&book_price=5&total_price=%f"
                            "&train_id2=%s&start_day2=%d&start_time2=%d:%d&start_station2=%s"
                            "&arrive_day2=%d&arrive_time2=%d:%d&arrive_station2=%s&seat_type2=%s&seat_price2=%f&book_price2=5&total_price2=%f"
                            "&sum_price=%f\">确认</a></th>",user,train_id,start_day,start_time_1,start_time_2,start_station,arrive_day,arrive_time_1,arrive_time_2,arrive_station,
                            seat_type,seat_price,total_price,
                            train_id2,start_day2,start_time1_1,start_time1_2,start_station2,
                            arrive_day2,arrive_time1_1,arrive_time1_2,arrive_station2,seat_type2,seat_price2,total_price2,sum_price);
    printf("<th align=\"right\"><a href=\"query.cgi?user=%s\">取消</a></th>",user);
    printf("</table>");
    }
     printf("</div>\n");
     printf("</BODY>\n");
     printf("</HTML>\n");
    return 0;
}
