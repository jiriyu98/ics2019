#include <am.h>
#include <amdev.h>
#include <nemu.h>

#include <klib.h>
static uint32_t *const fb = (uint32_t *)(uintptr_t)FB_ADDR;

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info->width = inl(SCREEN_ADDR);
      info->height = info->width & 0xffff;
    info->width = info->width >> 16;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
    int width = screen_width(), height = screen_height(), i;
    assert(ctl->x + ctl->w <= width && ctl->y + ctl->h <= height);
    for (i = 0; i < ctl->h; i++)
    memcpy(fb + (i + ctl->y) * width + ctl->x, ctl->pixels + i * ctl->w, ctl->w * sizeof(uint32_t));

      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  
  int i, size = screen_width() * screen_height();
  for (i = 0; i < size; i++) fb[i] = i;
  draw_sync();
  
}