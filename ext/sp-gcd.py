#return sigma(0<=x<=xmax:floor((ax+b)/c))
def sGCD(a, b, c, xmax):
	#print a, b, c, xmax, 
	if (xmax < 0): return 0
	if (c < 0): [a, b, c] = [-a, -b, -c]
	xmax += 1
	re = 0
	if (a < 0) or (a >= c):
		am = (a % c + c) % c
		k = (a - am) / c
		re += k * xmax * (xmax - 1) / 2
		a = am
	if (b < 0) or (b >= c):
		bm = (b % c + c) % c
		k = (b - bm) / c
		re += k * xmax
		b = bm
	#print re
	if (a * xmax + b >= c):
		re += sGCD(c, (a * xmax + b) % c, a, (a * xmax + b) / c - 1)
	return re

