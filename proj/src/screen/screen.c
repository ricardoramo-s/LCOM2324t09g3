#include "screen.h"

static double pos_x; //center absolute position in grid
static double pos_y; //center absolute position in grid

static int grid_pos_x; //center position in grid
static int grid_pos_y; //center position in grid

static int mid_x;
static int mid_y;
static int min_x;
static int min_y;
static int max_x;
static int max_y;

static struct GRID_INFO* grid_info_;

int screen_init(double x, double y) {

	pos_x = x;
    pos_y = y;

	struct GPU_RES_INFO gpu_res_info = *gpu_get_gpu_res_info();
    grid_info_ = grid_get_grid_info();

	screen_info.x_size = gpu_res_info.x_res / GRID_SQUARE_SIZE + 1;
    screen_info.y_size = gpu_res_info.y_res / GRID_SQUARE_SIZE + 1;

	screen_info.objects = (char **) malloc(screen_info.y_size * sizeof(char *));

	for (int i = 0; i < screen_info.y_size; i++) {
        screen_info.objects[i] = (char *) malloc(screen_info.x_size * sizeof(char));
        memset(screen_info.objects[i], ' ', screen_info.x_size);
    }

	mid_x = screen_info.x_size / 2;
    mid_y = screen_info.y_size / 2;

    min_x = 0 - mid_x;
    min_y = 0 - mid_y;

    max_x = 0 + mid_x;
    max_y = 0 + mid_y;

    return 0;
}

void screen_cleanup() {

    for (int y = 0; y < screen_info.y_size; y++) {
        for (int x = 0; x < screen_info.x_size; x++) {
            screen_info.objects[y][x] = ' ';
        }
    }
}

void screen_update() {

    grid_pos_x = (int) pos_x;
    grid_pos_y = (int) pos_y;


    int i_min_x = grid_pos_x + min_x < 0 ? 0 : grid_pos_x + min_x;
    int i_min_y = grid_pos_y + min_y < 0 ? 0 : grid_pos_y + min_y;


    int i_max_x = grid_pos_x + max_x >= grid_info_->x_size ? grid_info_->x_size - 1 : grid_pos_x + max_x;
    int i_max_y = grid_pos_y + max_y >= grid_info_->y_size ? grid_info_->y_size - 1 : grid_pos_y + max_y;


    printf("\n(%d, %d)", i_min_x, i_max_x);
    printf("\n(%d, %d)\n", i_min_y, i_max_y);

    for (int y = i_min_y; y <= i_max_y; y++) {
        for (int x = i_min_x; x <= i_max_x; x++) {
            screen_info.objects[y - i_min_y][x - i_min_x] = grid_info_->objects[y][x];
        }
    }
}

struct SCREEN_INFO* screen_get_screen_info() {
    return &screen_info;
}

