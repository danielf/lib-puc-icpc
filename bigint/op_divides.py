F = open('op-divides.in', 'r')

h = 1
for line in F.xreadlines():
	a, b = map(int, line.split())
	print '#%d' % (h, )
	asgn = (a and (a / abs(a))) or 0
	bsgn = (b and (b / abs(b))) or 0
	csgn = asgn * bsgn
	q = (abs(a) / abs(b)) * csgn
	r = (abs(a) % abs(b)) * asgn
	print '%d / %d = %d' % (a, b, q)
	print '%d %% %d = %d' % (a, b, r)
	print
	h += 1
