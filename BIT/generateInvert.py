import random

N = 5000000
print N
l = range(N)
random.shuffle(l)
print ' '.join(map(str, l))

