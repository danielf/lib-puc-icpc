TESTS = input-output \
        op-plus op-minus op-multiplies op-divides op-root \
		mersenne compute-pi compute-e

.PHONY: all test clean test_% debug_%
.SECONDARY: $(TESTS:%=%.cpp) $(TESTS:%=%.out)

all: $(TESTS)

test: $(TESTS:%=test_%)

clean:
	rm -f *~ .*.swp gmon.out
	for i in $(TESTS); do \
		rm -f $$i $$i.cpp $$i.out; \
	done

debug_%: % %.in %.out
	less $*.out

test_%: % %.in %.out %.sol
	diff $*.sol $*.out

%.out: % %.in
	./$* < $*.in > $*.out

%.cpp: ../header.cpp bigint.cpp %.main.cpp 
	rm -f $@
	cat $^ > $@

%: %.cpp
	g++ -o $@ $@.cpp -Wall -pedantic -lm -g -pg

