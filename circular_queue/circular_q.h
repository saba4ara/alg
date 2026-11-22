#ifndef CIRCULAR_Q_H
#define CIRCULAR_Q_H



#ifndef CQ_SIZE
#define CQ_SIZE 4
#endif



typedef struct {
	int* content;

	unsigned int head;
	unsigned int tail;
	
	unsigned int size;
	unsigned int capacity;
} CQueue;



void init(CQueue* cq);

void resize(CQueue* cq);

void push(CQueue* cq, int x);

// size > 0
int pop(CQueue* cq);

// size > 0
int get_tail(CQueue* cq);

// size > 0
int get_head(CQueue* cq);

void print_cq(CQueue* cq);

void destroy(CQueue* cq);



#endif // CIRCULAR_Q_H

