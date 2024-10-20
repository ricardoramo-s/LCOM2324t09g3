#include "gpu.h"
#include "vec2/vec2.h"
#include <math.h>


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

int gpu_set_mode(uint16_t mode) {

  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.ax = 0x4F02;
  r86.bx = BIT(14) | mode;
  r86.intno = 0x10;

  if (sys_int86(&r86) != OK) return 1;

  return 0;
}

int gpu_init_mem_indexation(uint16_t mode) {

  vbe_mode_info_t vmi_p;

  memset(&vmi_p, 0, sizeof(vmi_p));

  if (vbe_get_mode_info(mode, &vmi_p)) return 1;

  gpu_res_info.x_res = vmi_p.XResolution;
  gpu_res_info.y_res = vmi_p.YResolution;

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
  vram_size = gpu_res_info.x_res * gpu_res_info.y_res * bytes_per_pixel;

  mem_range.mr_base = vram_base;
  mem_range.mr_limit = mem_range.mr_base + vram_size;

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *) mem_range.mr_base, vram_size);

  if (video_mem == MAP_FAILED) panic("couldn't map video memory");

  backbuffer = (char *) malloc(vram_size * sizeof(char));

  if (backbuffer == NULL) panic("couldn't map video memory");

  return 0;
}

int gpu_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {

  if (x >= gpu_res_info.x_res || y >= gpu_res_info.y_res) return 0;
  if (x < 0 || y < 0) return 0;

  uint32_t location = (y * gpu_res_info.x_res + x) * bytes_per_pixel;

  memcpy(backbuffer + location, &color, bytes_per_pixel);

  return 0;
}

int gpu_draw_line(vec2 position, uint8_t *bytes, int pixelsc) {
  if (position.x > (int) gpu_res_info.x_res) return 0;
  if (position.x + pixelsc < 0) return 0;
  if (position.y < 0 || position.y >= (int) gpu_res_info.y_res) return 0;

  int start = MAX(position.x, 0);
  int end = MIN(gpu_res_info.x_res - 1, position.x + pixelsc - 1);
  int count = end - start + 1;

  int location = (position.y * gpu_res_info.x_res + start) * bytes_per_pixel;

  // printf("[gpu_draw_line] x: %d, y: %d, start: %d, end: %d, count: %d\n", position.x, position.y, start, end, count);

  if (position.x < 0) bytes -= position.x;
  memcpy(backbuffer + location, bytes, count * bytes_per_pixel);

  return 0;
}

int gpu_draw_rect(vec2 position, vec2 size, uint8_t color) {
  if (color_mode == INDEX_MODE) { // indexed
    for (int y = position.y; y < position.y + size.y; y++) {
      uint8_t bytes[size.x];
      memset(bytes, color, size.x);
      gpu_draw_line(VEC2(position.x, y), bytes, size.x);
    }
  }
  else {
    // TODO: direct color
  }

  return 0;
}

int gpu_draw_transparent_xpm(xpm_image_t *xpm, vec2 position, vec2 offset, vec2 size) {
  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      uint8_t byte = xpm->bytes[((y + offset.y) * xpm->width + (x + offset.x))];
      if (byte != 99) {
        gpu_draw_pixel(position.x + x, position.y + y, byte);
      }
    }
  }

  return 0;
}

int gpu_draw_xpm(xpm_image_t *xpm, vec2 position, vec2 offset, vec2 size) {
  // printf("[gpu_draw_xpm] x: %d, y: %d, offset.x: %d, offset.y: %d, size.x: %d, size.y: %d\n", position.x, position.y, offset.x, offset.y, size.x, size.y);
  uint8_t *bytes = malloc(size.x * bytes_per_pixel);

  for (int y = offset.y; y < offset.y + size.y; y++) {
    memcpy(bytes, xpm->bytes + (y * xpm->width + offset.x) * bytes_per_pixel, size.x * bytes_per_pixel);

    gpu_draw_line(VEC2(position.x, position.y + y - offset.y), bytes, size.x);
  }

  free(bytes);
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

void gpu_buffer_commit() {
  memcpy(video_mem, backbuffer, vram_size);
}

void gpu_buffer_clear() {
  memset(backbuffer, 0, vram_size);
}

int aux_clear() {

  if (vg_draw_rectangle(0, 0, x_res, y_res, 0x0)) return 1;

  return 0;
}

struct GPU_RES_INFO *gpu_get_gpu_res_info() {
  return &gpu_res_info;
}
