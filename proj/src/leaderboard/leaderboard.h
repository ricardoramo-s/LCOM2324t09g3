#pragma once

#include <stdint.h>
#include "drivers/rtc.h"

typedef struct {
    char name[4];
    struct rtc_date_time dt_of_entry;
    time_t time_entry;
} LeaderboardEntry;

typedef struct {
    LeaderboardEntry entries[3];
    uint8_t entry_count;
} Leaderboard;


int Leaderboard_read_file(Leaderboard* leaderboard);
int Leaderboard_write_file(Leaderboard* leaderboard);
void Leaderboard_add_entry(Leaderboard *leaderboard, char name[4], struct rtc_date_time dt_of_entry, time_t time_entry);

