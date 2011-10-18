#!/usr/bin/python
# encoding: utf-8

import sys

def chomp(s):
	if s.endswith('\n'):
		return s[:-1]
	return s

args = []
argp = []

for arg in sys.argv[1:]:
	if arg.startswith('--'):
		args.append(arg[2:])
	else:
		argp.append(arg.decode('utf-8'))

if 'page-break' in args:
	sys.stdout.write('\f\n')
elif 'no-line-break' in args:
	pass
else:
	sys.stdout.write('\n')

if len(argp) > 2:
	print >> sys.stderr, "WARNING: discarding input"
	argp = argp[:2]

sys.stdout.write('/' * 80)
sys.stdout.write('\n')

sys.stdout.write('//')
sys.stdout.write(' ')
sys.stdout.write(argp[0].encode('utf-8'))
sys.stdout.write(' ')
sys.stdout.write('/' * (80 - sum([len(_) for _ in argp]) - 4 * len(argp)))
if len(argp) > 1:
	sys.stdout.write(' ')
	sys.stdout.write(argp[1].encode('utf-8'))
	sys.stdout.write(' ')
	sys.stdout.write('//')
sys.stdout.write('\n')

sys.stdout.write('/' * 80)
sys.stdout.write('\n')

sys.stdout.write('\n')
