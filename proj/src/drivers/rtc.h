#include <lcom/lcf.h>
#include <stdint.h>

#include "utils.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS_REG         0x00
#define RTC_SECONDS_ALARM_REG   0x01
#define RTC_MINUTES_REG         0x02
#define RTC_MINUTES_ALARM_REG   0x03
#define RTC_HOURS_REG           0x04
#define RTC_HOURS_ALARM_REG     0x05
#define RTC_DAY_WEEK_REG        0x06
#define RTC_DAY_MONTH_REG       0x07
#define RTC_MONTH_REG           0x08
#define RTC_YEAR_REG            0x09
#define RTC_STATUS_A_REG        0x0A
#define RTC_STATUS_B_REG        0x0B
#define RTC_STATUS_C_REG        0x0C
#define RTC_STATUS_D_REG        0x0D

#define RTC_UPDATING            1

struct rtc_date_time {
    uint8_t rtc_seconds;
    uint8_t rtc_minutes;
    uint8_t rtc_hours;
    uint8_t rtc_day_week; // 1 -> Sunday 
    uint8_t rtc_day; // 1 - 31
    uint8_t rtc_month;
    uint16_t rtc_year; // TODO: Might need adjustment
};

struct rtc_alarm {
    uint8_t rtc_seconds;
    uint8_t rtc_minutes;
    uint8_t rtc_hours;
};



int rtc_read_register(uint8_t register_addr, uint8_t* register_data);
int rtc_read_date_time(struct rtc_date_time* rtc_dt);
void rtc_display_date_time(struct rtc_date_time* rtc_dt);

/** AUX **/
int rtc_update_in_progress();
uint8_t bcd_to_dec(uint8_t bcd);
time_t rtc_date_time_to_unix(struct rtc_date_time* rtc_dt);
void unix_to_rtc_date_time(time_t unix_time, struct rtc_date_time* rtc_dt); 


/** TESTS **/
int rtc_test_conf(void);
