#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

void isa_reg_display();
void watchpoints_display();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_exp(char *args);
static int cmd_scan(char *args);
static int cmd_setWatchPoints(char *args);
static int cmd_deleteWatchPoints(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
	{ "si", "si [N] - run N steps and stop", cmd_si },
	{ "info", "info SUBCMD - print reg state & watchpoint info", cmd_info},
	{ "p", "p EXPR - get expression value", cmd_exp},
	{ "x", "x N EXPR - get expression value and set it as start memory address, then output N characters consequently", cmd_scan},
	{ "w", "w EXPR - setWatchPoint", cmd_setWatchPoints},
	{ "d", "d N - deleteWatchPoint", cmd_deleteWatchPoints}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
	char *arg = strtok(NULL, " ");

	if (arg == NULL){
		cpu_exec(1);
	}else{
		int n = 0;
		for (int i=0; i<strlen(args); i++){
			n = args[i] + n * 10;
		}
		cpu_exec(n);
	}
	return 0;
}

static int cmd_info(char *args){
	char *arg = strtok(NULL, " ");

	if(arg == NULL){
		return 0;
	}else if(strcmp(arg, "r") == 0){
		isa_reg_display();
	}else if(strcmp(arg, "w") == 0){
		watchpoints_display();
	}
	return 0;
}

static int cmd_exp(char *args){
	if(args == NULL){
		return 0;
	}
	bool success = true;
	uint32_t result = expr(args, &success);
	if(success){
    printf("\033[0;32m %s = %d(%#x) \033[0m;\n",args,result,result);
	}
  return 0;
}

static int cmd_setWatchPoints(char *args){
	if(args == NULL){
		return 0;
	}
	printf("%s", args);
	WP* wp = new_wp();
	strcpy(wp->exp, args);
	bool success = true;
	wp->old_value = expr(wp->exp, &success);
	if(!true){
		return -1;
	}
	printf("NO.%d watchpoint is setted.\n", wp->NO);
	return 0;
}

static int cmd_deleteWatchPoints(char *args){
	char *arg = strtok(NULL, " ");

	if(arg == NULL){
		return 0;
	}
	bool success = true;
	int n;
	/*
	n = expr(arg, &success);
	if(!success){
		printf("exp wrong!\n");
	}
	*/
	n = 2;
	free_wp(n);
	return 0;
}

static int cmd_scan(char *args){
	bool success = true;
	uint32_t result = expr(args, &success);
	if(success){
    printf("\033[0;32m %s = %d(%#x) \033[0m;\n",args,result,result);
	}
  return 0;
}


void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
