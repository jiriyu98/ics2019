#include "common.h"
#include <amdev.h>
#include <am.h>
#include <nemu.h>

extern int screen_width();
extern int screen_height();

size_t __am_input_read(uintptr_t reg, void *buf, size_t size);
size_t __am_timer_read(uintptr_t reg, void *buf, size_t size);

size_t serial_write(const void *buf, size_t offset, size_t len) {
  size_t success = 0;
  while (success < len) _putc(*((char *)buf + success++));
  return success;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
    int keycode = read_key();
    if(keycode != _KEY_NONE){
      if (keycode & 0x8000) {
        keycode ^= 0x8000;
        len = sprintf(buf, "kd %s\n", keyname[keycode]);
      } else if(!((keycode & ~0x8000) == _KEY_NONE)){
        len = sprintf(buf, "ku %s\n", keyname[keycode]);
      }
    } else{
      len = sprintf(buf, "t %u\n", uptime());
    }
    return len;
}

static char dispinfo[128] __attribute__((used)) = {};


size_t get_dispinfo_size() {
  return strlen(dispinfo);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  if (len + offset > 128) len = 128 - offset;
  strncpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int x = (offset/4) % screen_width();
    int y = (offset/4) / screen_width();
    // draw_sync();
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
