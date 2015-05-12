import random

MOD = 469762049
N = 2**23
print N
print ' '.join([str(random.randrange(0, MOD)) for i in xrange(N)])
print ' '.join([str(random.randrange(0, MOD)) for i in xrange(N)])
