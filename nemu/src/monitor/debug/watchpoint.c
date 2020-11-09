#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp();
void free_wp(WP *wp);

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
	
}

WP* new_wp(){
	if(free == NULL){
		assert(0);
	}
	WP* newwp = free_;
	free_ = free_->next;
	newwp->next = head;
	head = newwp;
	return newwp;
}

void free_wp(WP *wp){
	if(wp == NULL || head == NULL){
		return;
	}
	if(wp == head){
		WP* temp = head;
		head = head->next;
		temp->next = free_;
		free_ = temp;
		return;
	}
	
	WP *front=head, *temp=head->next;
	while(temp!=NULL){
		if(wp == temp){
			front->next = temp->next;
			temp->next = free_;
			free_ = temp;
			break;
		}
		front = temp;
		temp = temp->next;
	}
}
