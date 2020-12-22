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
    if (keycode & 0x8000) {
      len = snprintf(buf, len, "kd %s\n", keyname[keycode & ~0x8000]);
    } else if(!((keycode & ~0x8000) == _KEY_NONE)){
      len = snprintf(buf, len, "ku %s\n", keyname[keycode & ~0x8000]);
    } else{
      len = snprintf(buf, len, "t %d\n", uptime());
    }
    return len;
}

static char dispinfo[128] __attribute__((used)) = {};


size_t get_dispinfo_size() {
  printf("dispinfo_len: %d\n", strlen(dispinfo));
  return strlen(dispinfo);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    len = snprintf(buf, len, dispinfo + offset);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int x = (offset/4) % screen_width();
    int y = (offset/4) / screen_width();
    draw_sync();
    draw_rect((void *)buf, x, y, len/4, 1);
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
