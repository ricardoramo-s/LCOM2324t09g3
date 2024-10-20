#include "gpu.h"

#define INDEX_MODE 0
#define DIRECT_MODE 1

static struct minix_mem_range mem_range;
static uint32_t vram_size;
static uint8_t *video_mem;
static char *backbuffer;

static uint16_t x_res;
static uint16_t y_res;

static uint8_t bits_per_pixel;
static uint8_t bytes_per_pixel;

static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;

static uint8_t red_mask_location;
static uint8_t green_mask_location;
static uint8_t blue_mask_location;

uint8_t color_mode;


void gpu_r86_err_hand(reg86_t *r86) {

	switch (r86->al) {

		case (0x4F):

			switch (r86->ah) {

				case (0x02): 
					printf("Function is not supported in the current hardware configuration");
                    return;

				case (0x03):
                    printf("Function call invalid in current video mode");
                    return;

				default: 
					printf(" Function call failed");
                    return;
			}

		default:
			printf("Function is not supported");
            return;
	}

}

int gpu_set_mode(uint16_t mode) {

	reg86_t r86;

	memset(&r86, 0, sizeof(r86));

	r86.ax = 0x4F02;          
	r86.bx = BIT(14) | mode; 
	r86.intno = 0x10;

	if (sys_int86(&r86) != OK) {
        gpu_r86_err_hand(&r86);
		return 1;
	}

	return 0;
}

int gpu_init_mem_indexation(uint16_t mode) {

	vbe_mode_info_t vmi_p;

	memset(&vmi_p, 0, sizeof(vmi_p));

	if (vbe_get_mode_info(mode, &vmi_p)) return 1;

	x_res = vmi_p.XResolution;
    y_res = vmi_p.YResolution;

	bits_per_pixel = vmi_p.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + 7) / 8;

	red_mask_size = vmi_p.RedMaskSize;
    green_mask_size = vmi_p.GreenMaskSize;
    blue_mask_size = vmi_p.BlueMaskSize;

	red_mask_location = vmi_p.RedFieldPosition;
    green_mask_location = vmi_p.GreenFieldPosition;
    blue_mask_location = vmi_p.BlueFieldPosition;

	color_mode = (vmi_p.MemoryModel == 0x06) ? DIRECT_MODE : INDEX_MODE;

	uint32_t vram_base = (phys_bytes) vmi_p.PhysBasePtr;
    vram_size = x_res * y_res * bytes_per_pixel;

	mem_range.mr_base = vram_base;
    mem_range.mr_limit =  mem_range.mr_base + vram_size;

	int r;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	video_mem = vm_map_phys(SELF, (void *) mem_range.mr_base, vram_size);

    if (video_mem == MAP_FAILED) panic("couldn't map video memory");

	backbuffer = (char*) malloc(vram_size * sizeof(char));

	if (backbuffer == NULL) panic("couldn't map video memory");


	return 0;
}

int gpu_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {

	if (x >= x_res || y >= y_res) return 0;
    if (x < 0 || y < 0) return 0;

	uint32_t location = (y * x_res + x) * bytes_per_pixel;

	memcpy(&backbuffer[location], &color, bytes_per_pixel);

	return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

	for (int n = 0; n < len; n++) {
        if (gpu_draw_pixel(x + n, y, color)) return 1;
	}

	return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

	for (int n = 0; n < height; n++) {
        if (vg_draw_hline(x, y + n, width, color)) return 1;
	}

	return 0;
}


int aux_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step) {

	uint8_t size_x = x_res / no_rectangles;
    uint8_t size_y = y_res / no_rectangles;

	for (uint8_t ny = 0; ny < no_rectangles; ny++) {
		for (uint8_t nx = 0; nx < no_rectangles; nx++) {

			uint16_t y = ny * size_y;
            uint16_t x = nx * size_x;

			switch (color_mode) {
                
				case INDEX_MODE: {
                    uint32_t color = (first + (ny * no_rectangles + nx) * step) % (1 << bits_per_pixel);
                    if (vg_draw_rectangle(x, y, size_x, size_y, color)) return 1;
                    break;
                }

				case DIRECT_MODE: {
                    uint32_t red = ((first & ((red_mask_location << (red_mask_size)) - 1)) + nx * step) % (1 << red_mask_size);
                    uint32_t green = ((first & ((green_mask_location << (green_mask_size)) - 1)) + ny * step) % (1 << green_mask_size);
                    uint32_t blue = ((first & ((blue_mask_location << (blue_mask_size)) - 1)) + (nx + ny) * step) % (1 << blue_mask_size);
                    uint32_t color = (red << red_mask_location) | (green << green_mask_location) | (blue << blue_mask_location);
                    if (vg_draw_rectangle(x, y, size_x, size_y, color)) return 1;
                    break;
                }
            }

		}
	}

	return 0;
}

int aux_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y) {
	
	enum xpm_image_type type = XPM_INDEXED;
    xpm_image_t img;

    uint8_t *colors = xpm_load(xpm, type, &img);

	for (int iy = 0; iy < img.height; iy++) {
		for (int ix = 0; ix < img.width; ix++) {
			if (gpu_draw_pixel(x + ix, y + iy, *colors)) return 1;
            colors++;
		}
	}

	return 0;

}


void aux_vg_commit() {
    memcpy(video_mem, backbuffer, vram_size);
}

int aux_clear() {

	if (vg_draw_rectangle(0, 0, x_res, y_res, 0x0)) return 1;

	return 0;
}
