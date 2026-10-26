#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

static char R, C, N;
static char grid[100];
static unsigned char global_best = 0;



static inline char read_bit();

static inline unsigned char calc_score(char a, char b, char c);

static inline unsigned char find_upper_bound(char ones, char twos, char threes);

void find_max(unsigned char idx, char score_so_far, char ones, char twos, char threes);



int main (int argc, char* argv[]) {
	R = read_bit();
	C = read_bit();

	if (R < 3 || R > 10 || C < 3 || C > 10) {
		perror("Invalid R or C value");
		return 1;
	}

	N = R * C;

	char ones = 0, twos = 0, threes = 0;

	for (unsigned char i = 0; i < N; ++i) {
		char val = read_bit();
		grid[i] = val;
		if (val == 1) ones++;
		else if (val == 2) twos++;
		else if (val == 3) threes++;
	}

	find_max(0, 0, ones, twos, threes);
	printf("%d\n", global_best);

	return 0;
}



static inline char read_bit() {
	char res = 0;
	int c;
	while (((c = getchar_unlocked()) < '0' || c > '9' ) && c != EOF);
	
	while (c >= '0' && c <= '9') {
		res = res * 10 + (c - '0');
		c = getchar_unlocked();
	}

	return res;
}

static inline unsigned char calc_score(char a, char b, char c) {
	if (a == b && b == c) return 3;
	if (a == b || b == c || a == c) return 1;
	return 0;
}

static inline unsigned char find_upper_bound(char ones, char twos, char threes) {
	char total_triples = (ones + twos + threes) / 3;
	char max_three_pts = (ones/3) + (twos/3) + (threes/3);

	char max_pairs = (ones/2) + (twos/2) + (threes/2);

	if (total_triples > max_pairs) total_triples = max_pairs;
	// if (max_three_pts > total_triples) max_three_pts = total_triples;

	// max_three_pts * 3 + (total_triples - max_three_pts) * 1
	return max_three_pts * 2 + total_triples;
}

void find_max(unsigned char idx, char score_so_far, char ones, char twos, char threes) {
	if (score_so_far > global_best) global_best = score_so_far;

	while (idx < N && grid[idx] == 0) idx++;
	if (idx >= N) return;

	if (score_so_far + find_upper_bound(ones, twos, threes) <= global_best) return;

	char r = idx / C;
	char c = idx % C;
	char first = grid[idx];

	// CHECK TRIPLE HORIZONTALLY
	if (c + 2 < C) {
		char second = grid[idx+1];
		char third = grid[idx+2];

		if (second != 0 && third != 0) {
			char pts = calc_score(first, second, third);
			if (pts > 0) {
				char n1 = ones - (first == 1) - (second == 1) - (third == 1);
                		char n2 = twos - (first == 2) - (second == 2) - (third == 2);
                		char n3 = threes - (first == 3) - (second == 3) - (third == 3);
				
				grid[idx] = grid[idx+1] = grid[idx+2] = 0;
				find_max(idx + 3, score_so_far + pts, n1, n2, n3);
				grid[idx] = first;
				grid[idx+1] = second;
				grid[idx+2] = third;
			}
		}
	}

	// CHECK TRIPLE VERTICALLY
	if (r + 2 < R) {
		unsigned char idx2 = idx + C;
		unsigned char idx3 = idx + 2*C;

		char second = grid[idx2];
		char third = grid[idx3];

		if (second != 0 && third != 0) {
			char pts = calc_score(first, second, third);
			if (pts > 0) {
				char n1 = ones - (first == 1) - (second == 1) - (third == 1);
                		char n2 = twos - (first == 2) - (second == 2) - (third == 2);
                		char n3 = threes - (first == 3) - (second == 3) - (third == 3);
				
				grid[idx] = grid[idx2] = grid[idx3] = 0;
				find_max(idx + 1, score_so_far + pts, n1, n2, n3);
				grid[idx] = first;
				grid[idx2] = second;
				grid[idx3] = third;
			}
		}
	}

	// SKIP THIS STONE
	find_max(idx + 1, score_so_far, ones - (first == 1), twos - (first == 2), threes - (first == 3));

}

