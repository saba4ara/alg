#include "circular_q.h"

#include <stdio.h>
#include <stdlib.h>



void init(CQueue* cq) {
	cq->head = cq->tail = cq->size = 0;
	cq->capacity = CQ_SIZE;	

	cq->content = (int*)calloc(cq->capacity, sizeof(int));
	if (!cq->content) exit(2);
}

void resize(CQueue* cq) {
	int* tmp = (int*)calloc(2 * cq->capacity, sizeof(int));
	if (!tmp) exit(2);
	
	for (unsigned int i = 0; i < cq->size; ++i) {
		tmp[i] = cq->content[(cq->head + i) % cq->capacity];
	}

	cq->capacity *= 2;
	
	cq->head = 0;
	cq->tail = cq->size;
	free(cq->content);
	cq->content = tmp;
}

void push(CQueue* cq, int x) {
	if (cq->size == cq->capacity) resize(cq);
	
	cq->content[cq->tail] = x;
	
	cq->tail = (cq->tail + 1) % cq->capacity;
	cq->size++;
}

// size > 0
int pop(CQueue* cq) {
	int tmp = cq->content[cq->head];
	cq->content[cq->head] = 0;
	cq->head = (cq->head + 1) % cq->capacity;
	cq->size--;
	
	return tmp;
}

// size > 0
int get_tail(CQueue* cq) {
	return cq->content[(cq->head + cq->size - 1) % cq->capacity];
}

// size > 0
int get_head(CQueue* cq) {
	return cq->content[cq->head];
}

void destroy(CQueue* cq) {
	free(cq->content);
	cq->content = NULL;
}



void print_cq(CQueue* cq) {
	if (cq->size == 0) {
		puts("NO ITEMS\n");
		return;
	}

	for (unsigned int i = 0; i < cq->capacity; ++i) {
		printf("%d ", cq->content[i]);
	}
	puts("\n");
}

// void debug_print(CQueue* cq) {
// 	printf("head = %u\ntail = %u\nsize = %u\ncapacity = %u\n", cq->head, cq->tail, cq->size, cq->capacity);
// 	printf("head: %d\n", get_head(cq));
// 	printf("tail: %d\n", get_tail(cq));
// 	print_cq(cq);
// }



// int main(int argc, char* argv[]) {
// 	CQueue cq;
// 	init(&cq);
// 	
// 	push(&cq, 14);
// 	debug_print(&cq);
// 	
// 	push(&cq, 22);
// 	debug_print(&cq);
// 
// 	push(&cq, 13);
// 	debug_print(&cq);
// 
// 	pop(&cq);
// 	debug_print(&cq);
// 
// 	push(&cq, 15);
// 	debug_print(&cq);
// 
// 	push(&cq, 4);
// 	debug_print(&cq);
// 
// 	resize(&cq);
// 	debug_print(&cq);
// 
// 	push(&cq, -6);
// 	debug_print(&cq);
// 
// 	push(&cq, -8);
// 	debug_print(&cq);
// 
// 	pop(&cq);
// 	debug_print(&cq);
// 
// 	push(&cq, -10);
// 	debug_print(&cq);
// 
// 	free(cq.content);
// 	cq.content = NULL;
// 
// 	return 0;
// }

