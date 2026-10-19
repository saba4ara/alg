def prefix_sum(a):
	p = [0] * (len(a)+1)
	for i in range(len(a)):
		p[i+1] = p[i] + a[i] 
	return p

def m_prefix_sum(m):
	N = len(m)+1
	p = [[0] * N for _ in range(N)]
	for r in range(1, N):
		for c in range(1, N):		
			p[r][c] = m[r-1][c-1] + p[r-1][c] + p[r][c-1] - p[r-1][c-1]
	return p

a = [1, -2, 4, 5, -1, -5, 2, 7]

m = [[1, 2, 3],
     [4, 5, 6],
     [7, 8, 9]]

print(prefix_sum(a))
print(m_prefix_sum(m))
