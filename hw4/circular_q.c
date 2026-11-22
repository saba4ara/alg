#include "circular_q.h"

#include <stdio.h>
#include <stdlib.h>



void init(CQueue* cq) {
	cq->head = cq->tail = cq->size = 0;
	cq->capacity = CQ_SIZE;	

	cq->content = (State*)malloc(cq->capacity * sizeof(State));
	if (!cq->content) exit(ALLOC_ERROR);
}

void resize(CQueue* cq) {
	State* tmp = (State*)malloc(2 * cq->capacity * sizeof(State));
	if (!tmp) exit(ALLOC_ERROR);
	
	// 2x memset
	for (unsigned int i = 0; i < cq->size; ++i) {
		tmp[i] = cq->content[(cq->head + i) % cq->capacity];
	}

	cq->capacity *= 2;
	
	cq->head = 0;
	cq->tail = cq->size;
	free(cq->content);
	cq->content = tmp;
}

void push(CQueue* cq, State x) {
	if (cq->size == cq->capacity) resize(cq);
	
	cq->content[cq->tail] = x;
	
	cq->tail = (cq->tail + 1) % cq->capacity;
	cq->size++;
}

// size > 0
State pop(CQueue* cq) {
	State tmp = cq->content[cq->head];
	// cq->content[cq->head] = 0;
	cq->head = (cq->head + 1) % cq->capacity;
	cq->size--;
	
	return tmp;
}

// size > 0
State get_tail(CQueue* cq) {
	return cq->content[(cq->head + cq->size - 1) % cq->capacity];
}

// size > 0
State get_head(CQueue* cq) {
	return cq->content[cq->head];
}

void destroy(CQueue* cq) {
	free(cq->content);
	cq->content = NULL;
}



// void print_cq(CQueue* cq) {
// 	if (cq->size == 0) {
// 		puts("NO ITEMS\n");
// 		return;
// 	}
// 
// 	for (unsigned int i = 0; i < cq->capacity; ++i) {
// 		printf("%d ", cq->content[i]);
// 	}
// 	puts("\n");
// }

