#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include<stdint.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);

uint32_t expr(char *e, bool *success);


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  // int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);
	
	/* test expr() */
	int ans, result;
	char buf[65536];
	bool success;
	FILE * fp = fopen("tools/gen-expr/input", "r");
	while(fscanf(fp, "%d %s", &ans, buf) != -1){
		result = expr(buf, &success);
		if(!success){
			printf("Wrong expr");
		}else{
			if(result == ans){
				printf("True! Cong!");
			}else{
				printf("False! Please debug!");
				break;
			}
		}
		result = 0;
		memset(buf, 0, sizeof(buf));
	}


  return 0;
}
