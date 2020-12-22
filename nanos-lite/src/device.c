#include "common.h"
#include <amdev.h>
#include <am.h>
#include <nemu.h>

extern int screen_width();
extern int screen_height();

size_t __am_input_read(uintptr_t reg, void *buf, size_t size);
size_t __am_timer_read(uintptr_t reg, void *buf, size_t size);

size_t serial_write(const void *buf, size_t offset, size_t len) {
    for(int i = 0; i < len;i++){
        _putc(((char*)buf)[i]);
    }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
    int keycode = read_key();

    printf("keycode = 0x%x\n", keycode);
    printf("keycode & ~0x8000 = 0x%x\n", keycode & ~0x8000);
    printf("keycode & 0x8000 = 0x%x\n", keycode & 0x8000);
    if((keycode & ~0x8000) == _KEY_NONE){
      printf("_KEY_NONE = %d\n", uptime());
        len = sprintf(buf,"t %d\n", uptime());
    }
    else if (keycode & 0x8000) {
      len = sprintf(buf, "kd %s\n", keyname[keycode & ~0x8000]);
    } else {
      len = sprintf(buf, "ku %s\n", keyname[keycode & ~0x8000]);
    }
    return len;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    len = sprintf(buf, dispinfo + offset);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int x = (offset/4) % screen_width();
    int y = (offset/4) / screen_width();
    draw_sync();
    draw_rect((uint32_t*)buf, x, y, len/4, 1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
    draw_sync();
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
}
