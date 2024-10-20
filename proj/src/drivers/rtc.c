#include "rtc.h"

int rtc_update_in_progress() {
    uint8_t rtc_status;
    if (rtc_read_register(RTC_STATUS_A_REG, &rtc_status)) return 1;
    return (rtc_status & BIT(7)) ? RTC_UPDATING : !RTC_UPDATING;
}

uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

time_t rtc_date_time_to_unix(struct rtc_date_time* rtc_dt) {
    struct tm t;

    t.tm_sec = rtc_dt->rtc_seconds;
    t.tm_min = rtc_dt->rtc_minutes;
    t.tm_hour = rtc_dt->rtc_hours;
    t.tm_mday = rtc_dt->rtc_day;
    t.tm_mon = rtc_dt->rtc_month - 1;
    t.tm_year = rtc_dt->rtc_year + 100;
    t.tm_isdst = -1;

    return mktime(&t);
}

void unix_to_rtc_date_time(time_t unix_time, struct rtc_date_time* rtc_dt) {
    struct tm* t = gmtime(&unix_time);

    rtc_dt->rtc_seconds = t->tm_sec;
    rtc_dt->rtc_minutes = t->tm_min;
    rtc_dt->rtc_hours = t->tm_hour;
    rtc_dt->rtc_day = t->tm_mday;
    rtc_dt->rtc_month = t->tm_mon + 1;
    rtc_dt->rtc_year = t->tm_year + 1900;
}


int rtc_read_register(uint8_t register_addr, uint8_t* register_data) {
    if (sys_outb(RTC_ADDR_REG, register_addr)) return 1;
    if (util_sys_inb(RTC_DATA_REG, register_data)) return 1;
    return 0;
}

int rtc_read_date_time(struct rtc_date_time* rtc_dt) {
    if (rtc_update_in_progress() == RTC_UPDATING) return 1;
    if (rtc_dt == NULL) return 1;
    
    uint8_t seconds, minutes, hours, day_of_week, day, month, year;

    if (rtc_read_register(RTC_SECONDS_REG, &seconds)) return 1;
    if (rtc_read_register(RTC_MINUTES_REG, &minutes)) return 1;
    if (rtc_read_register(RTC_HOURS_REG, &hours)) return 1;
    if (rtc_read_register(RTC_DAY_WEEK_REG, &day_of_week)) return 1;
    if (rtc_read_register(RTC_DAY_MONTH_REG, &day)) return 1;
    if (rtc_read_register(RTC_MONTH_REG, &month)) return 1;
    if (rtc_read_register(RTC_YEAR_REG, &year)) return 1;

    rtc_dt->rtc_seconds = bcd_to_dec(seconds);
    rtc_dt->rtc_minutes = bcd_to_dec(minutes);
    rtc_dt->rtc_hours = bcd_to_dec(hours);
    rtc_dt->rtc_day_week = bcd_to_dec(day_of_week);
    rtc_dt->rtc_day = bcd_to_dec(day);
    rtc_dt->rtc_month = bcd_to_dec(month);
    rtc_dt->rtc_year = bcd_to_dec(year);

    return 0;
}

void rtc_display_date_time(struct rtc_date_time* rtc_dt) {
    if (rtc_dt == NULL) {
        printf("Invalid date/time data.\n");
        return;
    }

    const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    printf("%02u:%02u:%02u %s, %02u/%02u/20%02u\n",
           rtc_dt->rtc_hours,
           rtc_dt->rtc_minutes,
           rtc_dt->rtc_seconds,
           weekdays[rtc_dt->rtc_day_week],
           rtc_dt->rtc_day,
           rtc_dt->rtc_month,
           rtc_dt->rtc_year
    );
}

int rtc_test_conf(void) {
    uint8_t reg_a, reg_b, reg_c, reg_d;

    if (rtc_read_register(RTC_STATUS_A_REG, &reg_a)) {
        printf("Failed to read Register A.\n");
        return 1;
    }
    if (rtc_read_register(RTC_STATUS_B_REG, &reg_b)) {
        printf("Failed to read Register B.\n");
        return 1;
    }
    if (rtc_read_register(RTC_STATUS_C_REG, &reg_c)) {
        printf("Failed to read Register C.\n");
        return 1;
    }
    if (rtc_read_register(RTC_STATUS_D_REG, &reg_d)) {
        printf("Failed to read Register D.\n");
        return 1;
    }

    printf("Register A: 0x%02x\n", reg_a);
    printf("  UIP - Update In Progress: %d\n", reg_a & BIT(7));
    printf("  RS3-RS0: %d%d%d%d\n", (reg_a & 0x08) >> 3, (reg_a & 0x04) >> 2, (reg_a & 0x02) >> 1, (reg_a & 0x01));

    printf("Register B: 0x%02x\n", reg_b);
    printf("  SET - Inhibiting updates of time/date registers: %d\n", reg_b & BIT(7));
    printf("  PIE - Interrupt source: %d\n", reg_b & BIT(6));
    printf("  AIE - Interrupt source: %d\n", reg_b & BIT(5));
    printf("  UIE - Interrupt source: %d\n", reg_b & BIT(4));
    printf("  SQWE - Square-wave generation: %d\n", reg_b & BIT(3));
    printf("  DM - Registers in binary (otherwise BCD): %d\n", reg_b & BIT(2));
    printf("  24/12 - 24h Format: %d\n", reg_b & BIT(1));
    printf("  DSE - Daylight Savings Time: %d\n", reg_b & BIT(0));

    printf("Register C: 0x%02x\n", reg_c);
    printf("  IRQF - IRQ line active: %d\n", reg_c & BIT(7));
    printf("  PF - Periodic interrupt pending: %d\n", reg_c & BIT(6));
    printf("  AF - Alarm interrupt pending: %d\n", reg_c & BIT(5));
    printf("  UF - Update interrupt pending: %d\n", reg_c & BIT(4));

    printf("Register D: 0x%02x\n", reg_d);
    printf("  VRT - Valid RAM/time: %d\n", (reg_d & 0x80) >> 7);

    return 0;
}
