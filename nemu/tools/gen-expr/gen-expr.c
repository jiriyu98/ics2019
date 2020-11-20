#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define NR_TOKENS 32
// this should be enough
static char buf[65536];
static int strSub =0;
static int nr_tokens = 0;

static inline uint32_t choose(uint32_t n){
	return rand() % n ;
}

static inline void gen_num()
{
  uint32_t num =rand()%1000;
  char buffer[40];
  snprintf(buffer,sizeof(buffer),"%d",num);
  int len = strlen(buffer);
  strcpy(&buf[strSub],buffer);
  strSub+=len;
  nr_tokens++;
  return ;
}
static inline void gen(char c)
{
  if(c=='('){
  	buf[strSub] = '(';
	}
  if(c==')'){
  	buf[strSub] = ')';
	}
  strSub++;
  nr_tokens++;
  return ;
}

static inline void gen_rand_op(){
  switch (choose(4)){
    case 0 : buf[strSub]='+';break;
    case 1 : buf[strSub]='-';break;
    case 2 : buf[strSub]='*';break;
    case 3 : buf[strSub]='/';break;
  }
  strSub++;
  nr_tokens++;
  return;
}
static inline void gen_rand_expr() {
  switch (choose(3)){
    case 0 :
    if(nr_tokens+1>=NR_TOKENS)
      break;
    gen_num();break;
    case 1 : 
    if(nr_tokens+3>=NR_TOKENS)
      break;
    gen('(');gen_rand_expr();gen(')');break;
    default: 
    if(nr_tokens+3>=NR_TOKENS)
      break;
    gen_rand_expr();gen_rand_op();gen_rand_expr();break;
  }
  switch (choose(2)){
		case 1:
    if(nr_tokens+1>=NR_TOKENS)
      break;
    buf[strSub]=' ';strSub++;nr_tokens++;break;
    default: break;
  }
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
		strSub=0;
    nr_tokens=0;
		memset(buf,0,sizeof(buf));
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
