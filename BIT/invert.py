N = int(raw_input())
l = map(int, raw_input().split())

ans = 0
for i in xrange(N):
    for j in xrange(i):
        if l[j] > l[i]: ans += 1
print ans
