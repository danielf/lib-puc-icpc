TESTS = seg-intersect \
        alamo venn icpc regata #mission

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

%.cpp: ../header.cpp geometry.cpp ../graph/graph.cpp field.cpp ../util/util.cpp triangulation.cpp %.main.cpp 
	rm -f $@
	cat $^ > $@

%: %.cpp
	g++ -o $@ $@.cpp -Wall -pedantic -lm -g -pg

geometry.txt: geometry.cpp field.cpp triangulation.cpp
	rm -f $@
	touch $@
	../make-header.py --no-line-break "Geometria" >> $@
	cat geometry.cpp >> $@
	../make-header.py "Campos" "15 min." >> $@
	cat field.cpp >> $@
	../make-header.py "Triangulações" "15 min." >> $@
	cat triangulation.cpp >> $@
