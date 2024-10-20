#include "grid.h"

int grid_init(char* level) {

    FILE *file;

    char level_path[100];

    strcpy(level_path, "/home/lcom/labs/shared/proj/src/assets/levels/");
    strcat(level_path, level);

    char line[100];

    file = fopen(level_path, "r");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }


    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading dimensions");
        fclose(file);
        return 1;
    }

    if (sscanf(line, "%hhu %hhu", &grid_info.x_size, &grid_info.y_size) != 2) {
        printf("Error parsing dimensions");
        fclose(file);
        return 1;
    }

    grid_info.objects = (char **) malloc(grid_info.y_size * sizeof(char *));

    for (int i = 0; i < grid_info.y_size; i++) {
        grid_info.objects[i] = (char *) malloc(grid_info.x_size * sizeof(char));
        memset(grid_info.objects[i], ' ', grid_info.x_size);
    }

    uint8_t y = 0;
    while (fgets(line, sizeof(line), file) != NULL && y < grid_info.y_size) {
        for (int i = 0; line[i] != '\0'; i++) {
            grid_info.objects[y][i] = line[i];
        }
        y++;
    }

    fclose(file);

    return 0;
}


void grid_cleanup() {

    if (grid_info.objects != NULL) {
        for (int i = 0; i < grid_info.y_size; i++) {
            free(grid_info.objects[i]);
        }
        free(grid_info.objects);
        grid_info.objects = NULL;
    }
}
    

struct GRID_INFO* grid_get_grid_info() {
    return &grid_info;
}
