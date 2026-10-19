#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define abs(a) (((a) >= (0)) ? (a) : (-(a)))

typedef struct {
	int N;
	int K;
	int** content;
} Matrix;

void alloc_matrix(Matrix* m);

void fill_matrix(Matrix* m);

void print_matrix(Matrix* m);

void free_matrix(Matrix* m);

bool is_valid_segment(Matrix* m, int x1, int y1);

void fill_segment_matrix(Matrix* m, Matrix* s);

void prefix_sum(Matrix* m, Matrix* p);

int region_sum(Matrix* p, int i, int j, int k, int l);



int main(int argc, char* argv[]) {
	Matrix m, s, p;

	if (scanf("%d %d", &m.N, &m.K) != 2) {
		perror("Didn't get N and K");
		return 1;
	}

	s.N = m.N;
	p.N = m.N + 1;

	alloc_matrix(&m);
	alloc_matrix(&s);
	alloc_matrix(&p);

	fill_matrix(&m);
	printf("N = %d, K = %d\n", m.N, m.K);
		
	fill_segment_matrix(&m, &s);

	prefix_sum(&s, &p);

 	printf("\n");
 	print_matrix(&m);
 	printf("\n");
 	print_matrix(&s);	
 	printf("\n");
 	print_matrix(&p);
 	printf("\n");

	int max_segments = 0;
	for (int r = 0; r <= m.N - m.K; ++r) {
		for (int c = 0; c <= m.N - m.K; ++c) {
			//printf("OK start window coords: (%d, %d)\n", r, c);
			int tmp_res = region_sum(&p, r, c, r+m.K-1, c+m.K-1);
			if (tmp_res > max_segments) max_segments = tmp_res;
			//printf("sum is %d\n\n", tmp_res); // translated s coords to p coords
		}
	}

	printf("%d\n", max_segments);

	free_matrix(&m);	
	free_matrix(&s);
	free_matrix(&p);	
	
	return 0;
}



void alloc_matrix(Matrix* m) {
	m->content = (int**)malloc(m->N * sizeof(int*));
	if (!m->content) {
		perror("Allocation Error");
		return;
	}

	for (int i = 0; i < m->N; ++i) {
		m->content[i] = (int*)calloc(m->N, sizeof(int));
		if (!m->content[i]) {
			perror("Allocation Error");
			free(m->content);
			return;
		}
	}
}

void fill_matrix(Matrix* m) {
	int n;
	for (int i = 0; i < m->N; ++i) {
		for (int j = 0; j < m->N; ++j) {
			if (scanf("%d", &n) == 1) m->content[i][j] = n;
		}
	}
}

void print_matrix(Matrix* m) {
	for (int i = 0; i < m->N; ++i) {
		for (int j = 0; j < m->N; ++j) {
			printf("%d ", m->content[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(Matrix* m) {
	for (int i = 0; i < m->N; ++i) {
		free(m->content[i]);
	}
	free(m->content);
}

bool is_valid_segment(Matrix* m, int x1, int y1) {
	int x2 = x1+1;
	int y2 = y1+1;

	if (x2 >= m->N || y2 >= m->N || m->content[x1][y1] != 1 || m->content[x2][y2] != 1) return false;

	int x1_start = max(x1-1, 0);
	int y1_start = max(y1-1, 0);
		
	int x2_end = min(x2+1, m->N-1);
	int y2_end = min(y2+1, m->N-1);

	for (int r = x1_start; r <= x2_end; ++r) {
		for (int c = y1_start; c <= y2_end; ++c) {
			if (r == x1 && c == y1 || r == x2 && c == y2 || 
                           (r == x1-1 && c == y1+2) || 
		           (r == x1+2 && c == y1-1)) continue;
			// printf("checking r = %d, c = %d, num = %d\n", r, c, m->content[r][c]);
			if (m->content[r][c] == 1) return false;
		}
	}

	return true;
	
}

void fill_segment_matrix(Matrix* m, Matrix* s) {
	for (int r = 0; r < m->N; ++r) {
		for (int c = 0; c < m->N; ++c) {
			if (is_valid_segment(m, r, c)) s->content[r][c] = 1 ;
		}
	}
}

void prefix_sum(Matrix* m, Matrix* p) {
	for (int r = 1; r < p->N; ++r) {
		for (int c = 1; c < p->N; ++c) {
			p->content[r][c] = m->content[r-1][c-1] + p->content[r-1][c] + p->content[r][c-1] - p->content[r-1][c-1];
		}
	}	
//	print_matrix(p);
}

int region_sum(Matrix* p, int i, int j, int k, int l) {
//	printf("starting (i, j) = (%d, %d)\n", i, j);
//	printf("finishing (k, l) = (%d, %d)\n", k, l);
//	printf("RETURNING [%d][%d] - [%d][%d] - [%d][%d] + [%d][%d]\n", k, l, i, l, k, j, i, j);
//	printf("RETURNING    %d   -    %d   -    %d   +    %d\n", p->content[k][l], p->content[i][l], p->content[k][j], p->content[i][j]);
	return p->content[k][l] - p->content[i][l] - p->content[k][j] + p->content[i][j];
}

