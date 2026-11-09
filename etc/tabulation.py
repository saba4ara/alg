def fill_dyn_arr() -> None:
	for y in range(N+1):
		for x in range(N+1):
			if y == 0 or x == 0: 
				dyn_arr[y * (N+1) + x] = 1
			else: 
				dyn_arr[y * (N+1) + x] = 1 + dyn_arr[(y-1) * (N+1) + x] + dyn_arr[y * (N+1) + (x-1)]

N: int = int(input())
dyn_arr = [0 for _ in range((N+1) * (N+1))]

fill_dyn_arr()
#print(dyn_arr)
print(dyn_arr[-1])
