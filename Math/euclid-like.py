#Euclid-like algorithm for calculating some sums
#See the comments below

#return \sum_{0<=x<=xmax}{\floor((ax+b)/c)}
def sum2(x): #\sum{x}
    return x * (x + 1) / 2
def sum3(x): #\sum{x^2}
    return x * (x + 1) * (2 * x + 1) / 6
 
import sys
 
#return {\sum_{0<=x<=xmax}{\floor{(ax+b)/c)}}
#        \sum_{0<=x<=xmax}{\floor{(ax+b)/c)}}^2
#        \sum_{0<=x<=xmax}{x*\floor{(ax+b)/c)}}
def calc3(a, b, c, xmax):
    if (xmax < 0): return [0] * 3
    if (a == 0):
        v = b / c
        return [v * (xmax + 1), v * v * (xmax + 1), v * sum2(xmax)]
    re = 0
    if (a >= c):
        sa = a / c
        [x, y, z] = calc3(a - sa * c, b, c, xmax)
        return [x + sum2(xmax) * sa, y + sum3(xmax) * sa * sa + 2 * sa * z, z + sum3(xmax) * sa]
    if (b >= c):
        sb = b / c
        [x, y, z] = calc3(a, b - sb * c, c, xmax)
        return [x + (xmax + 1) * sb, y + (xmax + 1) * sb * sb + 2 * sb * x, z + sum2(xmax) * sb]
    ymax = (a * xmax + b) / c
    [x, y, z] = calc3(c, c - b - 1, a, ymax - 1)
    return [ymax * xmax - x, ymax * ymax * xmax - (2 * z + x), sum2(xmax) * ymax - (x + y) / 2]
 
[a, c, b, l, r] = map(int, raw_input().split(' '))
ret = calc3(a, b, c, r)[2] - calc3(a, b, c, l - 1)[2]
print ret 
