#include "leaderboard.h"
#include <stdio.h>

int Leaderboard_read_file(Leaderboard *leaderboard) {
    FILE *file = fopen("/home/lcom/labs/g3/proj/src/leaderboard/leaderboard.txt", "r");
    if (!file) {
        return 1;
    }

    leaderboard->entry_count = 0;
    time_t dt_entry;
    while (fscanf(file, "%s %lld %lld", 
                    leaderboard->entries[leaderboard->entry_count].name, 
                    &dt_entry,
                    &leaderboard->entries[leaderboard->entry_count].time_entry) == 3) {
        unix_to_rtc_date_time(dt_entry, &leaderboard->entries[leaderboard->entry_count].dt_of_entry);

        leaderboard->entry_count++;
        if (leaderboard->entry_count >= 3) {
            break;
        }
    }

    fclose(file);
    return 0;
}

int Leaderboard_write_file(Leaderboard *leaderboard) {
    FILE *file = fopen("/home/lcom/labs/g3/proj/src/leaderboard/leaderboard.txt", "w");
    if (!file) {
        return 1;
    }

    for (int i = 0; i < 3; i++) {
        time_t timestamp = rtc_date_time_to_unix(&leaderboard->entries[i].dt_of_entry);
        fprintf(file, "%s %lld %lld\n", leaderboard->entries[i].name, timestamp, leaderboard->entries[i].time_entry);
    }

    fclose(file);
    return 0;
}

void Leaderboard_add_entry(Leaderboard *leaderboard, char name[4], struct rtc_date_time dt_of_entry, time_t time_entry) {
    LeaderboardEntry new_entry;
    strcpy(new_entry.name, name);
    new_entry.dt_of_entry = dt_of_entry;
    new_entry.time_entry = time_entry;

    uint8_t insert_position = leaderboard->entry_count;
    for (uint8_t i = 0; i < leaderboard->entry_count; i++) {
        if (new_entry.time_entry < leaderboard->entries[i].time_entry) {
            insert_position = i;
            break;
        }
    }

    for (int i = leaderboard->entry_count; i > insert_position; i--) {
        if (i < 3) {
            leaderboard->entries[i] = leaderboard->entries[i - 1];
        }
    }

    if (insert_position < 3) {
        leaderboard->entries[insert_position] = new_entry;
        if (leaderboard->entry_count < 3) {
            leaderboard->entry_count++;
        }
    }
}
