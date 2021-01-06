#include "fs.h"
extern size_t ramdisk_read(void*, size_t, size_t);
extern size_t ramdisk_write(const void*, size_t, size_t);

extern size_t events_read(void*, size_t, size_t);
extern size_t dispinfo_read(void*, size_t, size_t);
extern size_t serial_write(const void*, size_t, size_t);
extern size_t fb_write(const void*, size_t, size_t);
extern size_t fbsync_write(const void*, size_t, size_t);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
  {"/dev/tty", 0, 0, 0, invalid_read, serial_write},
  {"/proc/dispinfo", 0, 0, 0, dispinfo_read, invalid_write},
  {"/dev/fbsync", 0, 0, 0, invalid_read, fbsync_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  #include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static inline void fd_check(int fd) {
  assert(0 <= fd && fd < NR_FILES);
}

int fs_open(const char *pathname, int flags, int mode){
    for(int i = 0; i < NR_FILES;i++){
        if(strcmp(pathname, file_table[i].name) == 0){
            return i;
        }
    }
    assert(0 && "Can't find file");
}

size_t fs_read(int fd, void *buf, size_t len){
  fd_check(fd);
  size_t sz;
  if (file_table[fd].read == NULL) {
    sz = (file_table[fd].open_offset + len > file_table[fd].size) ? file_table[fd].open_offset : len;
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, sz);
  } else {
    sz = file_table[fd].read(buf, file_table[fd].open_offset, len);
  }

  file_table[fd].open_offset += sz; 
  return sz;
}

size_t fs_write(int fd, const void *buf, size_t len){
  fd_check(fd);
  size_t sz;
  if (file_table[fd].write == NULL) {
    sz = (file_table[fd].open_offset + len > file_table[fd].size) ? file_table[fd].open_offset : len;
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, sz);
  } else {
    sz = file_table[fd].write(buf, file_table[fd].open_offset, len);
  }
  file_table[fd].open_offset += sz;
  return sz;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  fd_check(fd);
    switch(whence){
        case SEEK_SET:
          if(0 <= offset && offset <= file_table[fd].size){
            file_table[fd].open_offset = offset;
          }
          break;
        case SEEK_CUR:
          if((offset + file_table[fd].open_offset >= 0) && (offset + file_table[fd].open_offset <= file_table[fd].size)){
              file_table[fd].open_offset += offset;
            }
            break;
        case SEEK_END:
            if((offset + file_table[fd].size >= 0) && (offset + file_table[fd].size <= file_table[fd].size)){
              file_table[fd].open_offset = file_table[fd].size + offset;
            }
            break;
        default:
          panic("lseek whence error!");
    }
    return file_table[fd].open_offset;
}

int fs_close(int fd){
    file_table[fd].open_offset = 0;
    return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int fb = fs_open("/dev/fb", 0, 0);
  file_table[fb].size = screen_width()*screen_height()*sizeof(uint32_t);
  fb = fs_open("/dev/fbsync", 0, 0);
  file_table[fb].size = 1;
  extern size_t get_dispinfo_size();
  fb = fs_open("/proc/dispinfo", 0, 0);
  file_table[fb].size = get_dispinfo_size();
}
