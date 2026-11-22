#ifndef CIRCULAR_Q_H
#define CIRCULAR_Q_H



#include "common.h"



#ifndef CQ_SIZE
#define CQ_SIZE 1024
#endif



typedef struct {
	State* content;

	unsigned int head;
	unsigned int tail;
	
	unsigned int size;
	unsigned int capacity;
} CQueue;



void init(CQueue* cq);

void resize(CQueue* cq);

void push(CQueue* cq, State x);

// size > 0
State pop(CQueue* cq);

// size > 0
State get_tail(CQueue* cq);

// size > 0
State get_head(CQueue* cq);

void print_cq(CQueue* cq);

void destroy(CQueue* cq);



#endif // CIRCULAR_Q_H

