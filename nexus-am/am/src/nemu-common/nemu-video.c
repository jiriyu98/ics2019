#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>

static inline int min(int x, int y) {
  return (x < y) ? x : y;
}

#define W 400
#define H 300

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info->width = W;
      info->height = H;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
        _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
        int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
        
      /* fb: the whole data that will be painted on the screen, 
       * but we only need to fill part of data (that is from (x, y) to (x + w, y + h)) by copy from pixels
       */
      uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR; 
      
      /* pixels: the data that will be painted. we show copy the data in pixels to fb. */
      uint32_t* pixels = ctl->pixels;
      int copy_size = sizeof(uint32_t) * ( W - x < w ? W-x : w);
      for(int i = 0; i < h && y + i < H; ++ i) {
        memcpy(&fb[(i + y) * W + x], pixels + i * w, copy_size);
      }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    } 
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  draw_sync();
}
