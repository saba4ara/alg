#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define DEBUG_PRINTS 0

typedef struct {
	char R;
	char C;
	char* content;
} Matrix;

typedef enum orientation {
	VERTICAL,
	HORIZONTAL
} Orient;



static inline char read_bit();

void alloc_matrix(Matrix* m);

void fill_matrix(Matrix* m);

void print_matrix(Matrix* m);

// (r, c) is left upper stone of the triple
char calculate_score(Matrix* m, char r, char c, Orient orient);

void delete_triple(Matrix* m, char r, char c, Orient o, char* buf);

void restore_triple(Matrix* m, char r, char c, Orient o, char* buf);

// (r1, c1) and (r2, c2) are left upper stones of the triples
bool triples_overlap(Matrix* m, char r1, char c1, Orient o1, char r2, char c2, Orient o2);

void debug_prints(Matrix* m);

void count_stones_in_m(Matrix* m, char* ones, char* twos, char* threes);

void count_stones_in_buf(char* buf, char* ones, char* twos, char* threes);

char find_upper_bound(char ones, char twos, char threes);

char find_max(Matrix* m, char start_r, char start_c, char score_so_far, char* global_best, char ones, char twos, char threes);


int main (int argc, char* argv[]) {
	Matrix m;
	m.R = read_bit();
	m.C = read_bit();

	if (m.R < 3 || m.R > 10 || m.C < 3 || m.C > 10) {
		perror("Invalid R or C value");
		return 1;
	}

	alloc_matrix(&m);
	fill_matrix(&m);

	if (DEBUG_PRINTS) debug_prints(&m);

	char ones = 0, twos = 0, threes = 0;
	count_stones_in_m(&m, &ones, &twos, &threes);

	char global_best = 0;
	printf("%d\n", find_max(&m, 0, 0, 0, &global_best, ones, twos, threes));

	free(m.content);

	return 0;
}



static inline char read_bit() {
	int c;
	while (((c = getchar_unlocked()) < '0' || c > '9' ) && c != EOF);
	return (char)(c - '0');
}

void alloc_matrix(Matrix* m) {
	m->content = (char*)calloc(m->R * m->C, sizeof(char));
	if (!m->content) {
		perror("Allocation Error");
		return;
	}
}

void fill_matrix(Matrix* m) {
	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			m->content[r * m->C + c] = read_bit();
		}
	}
}

void print_matrix(Matrix* m) {
	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			printf("%d ", m->content[r * m->C + c]); 
		}
		printf("\n");
	}
}

char calculate_score(Matrix* m, char r, char c, Orient o) {
	char res = 0;	

	if (r < 0 || r >= m->R || c < 0 || c >= m->C) return -1;

	char first_stone, second_stone, third_stone;
	first_stone = second_stone = third_stone = m->content[r * m->C + c];

	switch (o) {
		case VERTICAL:
			if ((r+2) >= m->R) return -1;

			first_stone = m->content[r * m->C + c];
			second_stone = m->content[(r+1) * m->C + c];
			third_stone = m->content[(r+2) * m->C + c];
				
			break;
		case HORIZONTAL:
			if ((c+2) >= m->C) return -1;

			first_stone = m->content[r * m->C + c];
		 	second_stone = m->content[r * m->C + (c+1)];
		 	third_stone = m->content[r * m->C + (c+2)];
			
			break;
	}	

	if (first_stone == 0 || second_stone == 0 || third_stone == 0) return -1;
	
	if (first_stone == second_stone && second_stone == third_stone) res = 3;
	else if ((first_stone == second_stone) || (second_stone == third_stone) || (first_stone == third_stone)) res = 1;
	
	return res;
}

void delete_triple(Matrix* m, char r, char c, Orient o, char* buf) {
	switch (o) {
		case VERTICAL:
			buf[0] = m->content[r * m->C + c];
			buf[1] = m->content[(r+1) * m->C + c];
			buf[2] = m->content[(r+2) * m->C + c];
			
			m->content[r * m->C + c] = m->content[(r+1) * m->C + c] = m->content[(r+2) * m->C + c] = 0;
			break;
		case HORIZONTAL:
			buf[0] = m->content[r * m->C + c];
			buf[1] = m->content[r * m->C + (c+1)];
			buf[2] = m->content[r * m->C + (c+2)];
	
			m->content[r * m->C + c] = m->content[r * m->C + (c+1)] = m->content[r * m->C + (c+2)] = 0;
			break;
	}
}

void restore_triple(Matrix* m, char r, char c, Orient o, char* buf) {
	switch (o) {
		case VERTICAL:
			m->content[r * m->C + c] = buf[0];
			m->content[(r+1) * m->C + c] = buf[1];
			m->content[(r+2) * m->C + c] = buf[2];	
			break;
		case HORIZONTAL:
			m->content[r * m->C + c] = buf[0];
			m->content[r * m->C + (c+1)] = buf[1];
			m->content[r * m->C + (c+2)] = buf[2];
			break;
	}
}

bool triples_overlap(Matrix* m, char r1, char c1, Orient o1, char r2, char c2, Orient o2) {
	char end_r1 = r1 + (o1 == VERTICAL ? 2 : 0);
	char end_c1 = c1 + (o1 == VERTICAL ? 0 : 2);
	char end_r2 = r2 + (o2 == VERTICAL ? 2 : 0);
	char end_c2 = c2 + (o2 == VERTICAL ? 0 : 2);

	bool rows_overlap = (r1 <= end_r2) && (r2 <= end_r1);
	bool cols_overlap = (c1 <= end_c2) && (c2 <= end_c1);
	
	return rows_overlap && cols_overlap;
}

void debug_prints(Matrix* m) {
	printf("%d %d\n", m->R, m->C);
	print_matrix(m);
	printf("\n");



//	for (char r = 0; r < m->R; ++r) {
//		for (char c = 0; c < m->C; ++c) {
//			char ver_res = calculate_score(m, r, c, VERTICAL);
//			if (ver_res == 3) printf("vertical triple on (%d, %d) is %d\n", r, c, ver_res);
//
//			char hor_res = calculate_score(m, r, c, HORIZONTAL);
//			if (hor_res == 3) printf("horizontal triple on (%d, %d) is %d\n", r, c, hor_res);
//		}
//	}

	//srand(time(NULL));
	//int i = 0;
	//while (i < 10) {
	//	char r1 = rand() % m->R;
	//	char c1 = rand() % m->C;
	//	char r2 = rand() % m->R;
	//	char c2 = rand() % m->C;
	//	
	//	char o1 = rand() % 2;
	//	char o2 = rand() % 2;

	//	if (r1 + 2 >= m->R || r2 + 2 >= m->R || c1 + 2 >= m->C || c2 + 2 >= m->C) continue;

	//	//printf("(%d, %d) and (%d, %d) are random triple\n", r1, c1, r2, c2);
	//	printf("do (%d, %d) %s and (%d, %d) %s collide? - %s\n", r1, c1, o1 == 0 ? "VERTICAL" : "HORIZONTAL", r2, c2, o2 == 0 ? "VERTICAL" : "HORIZONTAL", triples_overlap(m, r1, c1, o1, r2, c2, o2) ? "YES" : "NO");
	//	printf("\n");
	//	i++;
	//} 
}

void count_stones_in_m(Matrix* m, char* ones, char* twos, char* threes) {
	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			char cur_val = m->content[r * m->C + c];

			if (cur_val == 0) continue;
			
			if (cur_val == 1) (*ones)++;
			else if (cur_val == 2) (*twos)++;
			else if (cur_val == 3) (*threes)++;
		}
	}
}

void count_stones_in_buf(char* buf, char* ones, char* twos, char* threes) {
	for (unsigned char i = 0; i < 3; ++i) {
		if (buf[i] == 0) continue;

		if (buf[i] == 1) (*ones)++;
		else if (buf[i] == 2) (*twos)++;
		else if (buf[i] == 3) (*threes)++; 
	}
}

char find_upper_bound(char ones, char twos, char threes) {
	return (ones/3 + twos/3 + threes/3) * 3 + (ones%3 + twos%3 + threes%3) / 3;
}

char find_max(Matrix* m, char start_r, char start_c, char score_so_far, char* global_best, char ones, char twos, char threes) {
	if (score_so_far + find_upper_bound(ones, twos, threes) <= *global_best) return 0;

	char max_score = 0;
	char buf[3] = {0};

	for (char r = start_r; r < m->R; ++r) {
		for (char c = (r == start_r ? start_c : 0); c < m->C; ++c) {
			
			if (m->content[r * m->C + c] == 0) continue;

			for (char o = VERTICAL; o <= HORIZONTAL; ++o) {
				
				if ((o == VERTICAL && r+2 >= m->R) || (o == HORIZONTAL && c+2 >= m->C)) continue;

				char cur_path_score = 0;
				
				char cur_move_score = calculate_score(m, r, c, o);
				if (cur_move_score > 0) {
					char d_ones = 0, d_twos = 0, d_threes = 0;
					delete_triple(m, r, c, o, buf);
					count_stones_in_buf(buf, &d_ones, &d_twos, &d_threes);
					cur_path_score = cur_move_score + find_max(m, r, c, score_so_far + cur_move_score, global_best, ones-d_ones, twos-d_twos, threes-d_threes);
					if (cur_path_score > max_score) {
						max_score = cur_path_score;
						if (score_so_far + max_score > *global_best) *global_best = score_so_far + max_score;
					}
					restore_triple(m, r, c, o, buf);
				}
			}
		}
	}	

	return max_score;
}

