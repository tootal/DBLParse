import operator as op
import numpy as np
from functools import reduce

C_N = 1000
C_R = 400

def ncr(n, r):
    r = min(r, n-r)
    if r == 0: return 1
    numer = reduce(op.mul, range(n, n-r, -1))
    denom = reduce(op.mul, range(1, r+1))
    return numer//denom

binoms = np.ndarray(shape=(C_N + 1, C_R + 1), dtype=float)
for i in range(C_N + 1):
	for j in range(C_R + 1):
		if i >= j:
			binoms[i,j] = ncr(i,j)
		else:
			binoms[i,j] = 0

np.savetxt('nCr.txt', binoms, delimiter=',')
