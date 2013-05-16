from random import *

def gcd(x, y):
	if y == 0:
		return x
	else:
		return gcd(y, x % y)

def pow_mod(x, m, mod):
	if (m == 0):
		return 1
	z = pow_mod(x, m >> 1, mod)
	z = z * z % mod
	if (m & 1):
		z = z * x % mod
	return z

def primitive(x):
	for i in [2, 3, 5, 7, 11, 13, 17, 19, 23]:
		if (pow_mod(i, x - 1, x) % x != 1):
			return 0
	return 1

def rho_iterate(num):
	i = 1; k = 2
	delt = randint(1, num - 1)
	y = x = randint(0, num - 1)
	while (1):
		i = i + 1
		x = (x * x + delt) % num
		d = gcd(y - x, num)
		if (y == x):
			return 0
		if (d != 1):
			return d
		if (i == k):
			y = x; k = k + k

facts = {}

def submit(x):
	if (x in facts):
		facts[x] += 1
	else:
		facts[x] = 1


def naive_expand(n):
	i = 2
	while (i * i <= n):
		while (n % i == 0):
			submit(i)
			n = n / i
		i = i + 1
	if (n > 1):
		submit(n)

def expand(x):
	if (x <= 300):
		naive_expand(x)
	else:
		if (primitive(x)):
			submit(x)
		else:
			y = x
			while (y == x) or (y == 0):
				y = rho_iterate(x)
			expand(y)
			expand(x / y)

x = int(raw_input(""))
while (x != 0):
	facts = {}
	expand(x)
	tmp = facts.items()
	tmp.sort()
	for x in tmp:
		print "%d^%d" % (x[0], x[1]),
	print
	x = int(raw_input(""))

