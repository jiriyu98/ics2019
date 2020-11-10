#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp();
void free_wp(int NO);
bool check_watchpoints();

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

void watchpoints_display(){
  WP* node =head;
  printf("\033[0;34m");
  printf("NUM                    VALUE                    EXPR\n");
  while(node!=NULL)
  {
    uint32_t num = node->old_value;
    int len=0;
    while(num>0){
      num/=10;
      len++;
    }
    printf("%-16d%u(0x%-8x)%*s%s\n",
          node->NO,node->old_value,node->old_value,(44-16-8-len)," ",node->exp);
    node=node->next;
  }
  printf("\033[0m");
}

WP* new_wp(char *exp){
	if(free_ == NULL){
		assert(0);
	}
	WP* newwp = free_;
	free_ = free_->next;
	newwp->next = head;
	head = newwp;
	return newwp;
}

void free_wp(int NO){
	if(NO == NULL || head == NULL || NO <0 || NO >=31){
		return;
	}
	if(NO == head->NO){
		WP* temp = head;
		head = head->next;
		temp->next = free_;
		free_ = temp;
		return;
	}
	
	WP *front=head, *temp=head->next;
	while(temp!=NULL){
		if(NO == temp->NO){
			front->next = temp->next;
			temp->next = free_;
			free_ = temp;
			break;
		}
		front = temp;
		temp = temp->next;
	}
}

bool check_watchpoints(){
  WP* node = head; 
  bool result=true;
  while(node!=NULL){
		bool success = true;
		uint32_t newValue = expr(node->exp, &success);
			if(!success){
				assert(0);
				//Assert(0,"\033[0;31mThe watchpoints expression got some touble.\n\033[0m");
			}
			if(newValue!=node->old_value){
				  printf("\033[0;33m");
				  printf("watchpoints %d : %s\n",node->NO, node->exp);
				  printf("Old Value : %d(0x%x)\n",node->old_value, node->old_value); 
				  printf("New Value : %d(0x%x)\n",newValue, newValue); 
				  printf("\033[0m");
				  node->old_value=newValue;
				  result=false;
			}
			node=node->next;
  }
  return result;
}
