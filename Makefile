.PHONY: clean

clean:
	rm -f modelo.cpp library.txt library.txt.tmp library.pdf

#template.cpp: header.cpp main.cpp
#	rm -f $@
#	touch $@
#	cat header.cpp >> $@
#	echo >> $@
#	cat main.cpp >> $@

library.txt: \
		index.txt \
		compila \
		template.cpp \
		vimrc.txt \
		hash.sh \
		modelo.java \
		sizes.txt \
		primes/primes.txt \
		util/util.cpp \
		geometry/geometry.cpp \
		bigint/bigint.cpp \
		number_theory/number_theory.cpp \
		linear_algebra/linear_algebra.cpp \
		simplex/simplex.cpp \
		poker/poker.cpp \
		graph/graph.cpp \
		aho-corasick/aho-corasick.cpp \
		segment_tree/segment_tree.cpp \
		polynomial/polynomial.cpp \
		karatsuba.cpp \
		rmq.cpp \
		bit.cpp \
		resto_chines/trc.cpp \
		rabin-karp/rabin-karp.cpp \
		scheduling-notes.txt
	rm -f $@ $@.tmp
	touch $@.tmp
	./make-header.py --no-line-break "Summary" >> $@.tmp
	cat index.txt >> $@.tmp
	./make-header.py "C++ Compilation script" >> $@.tmp
	cat compila >> $@.tmp
	./make-header.py "Java template" >> $@.tmp
	cat modelo.java >> $@.tmp
	./make-header.py --page-break "C++ template" >> $@.tmp
	cat template.cpp >> $@.tmp
	
	./make-header.py --page-break "Some numbers to know" >> $@.tmp
	cat sizes.txt >> $@.tmp
	
	./make-header.py --page-break "Primes up to 10000" >> $@.tmp
	cat primes/primes.txt >> $@.tmp
	
	./make-header.py --page-break "Geometry" >> $@.tmp
	cat geometry/geometry.cpp >> $@.tmp
	
	./make-header.py --page-break "Number Theory" >> $@.tmp
	cat number_theory/number_theory.cpp >> $@.tmp
	
	./make-header.py --page-break "Poker" >> $@.tmp
	cat poker/poker.cpp >> $@.tmp
	
	./make-header.py --page-break "Graphs" >> $@.tmp
	cat graph/graph.cpp >> $@.tmp
	./make-header.py "Bipartite Graphs" >> $@.tmp
	cat graph/bipartite_graph.cpp >> $@.tmp
	
	./make-header.py "Union Find" >> $@.tmp
	cat data_structures/union_find.cpp >> $@.tmp
	
	./make-header.py --page-break "Least Common Ancestor" >> $@.tmp
	cat heavy_light/lca.cpp >> $@.tmp
	./make-header.py "Heavy-Light Decomposition" >> $@.tmp
	cat heavy_light/heavy_light.cpp >> $@.tmp
	
	./make-header.py --page-break "Segment Tree" >> $@.tmp
	cat data_structures/segment_tree.cpp >> $@.tmp
	
	./make-header.py --page-break "Segment Tree with Lazy Propagation" >> $@.tmp
	cat data_structures/segment_tree_lazy.cpp >> $@.tmp
	
	./make-header.py "BIT (Binary Index Tree)" >> $@.tmp
	cat data_structures/binary_indexed_tree.cpp >> $@.tmp
	
	./make-header.py --page-break "Polynomials (not very stable!)" >> $@.tmp
	cat polynomial/polynomial.cpp >> $@.tmp
	
	./make-header.py --page-break "Karatsuba (integer polynomials multiplication)" >> $@.tmp
	cat karatsuba.cpp >> $@.tmp
	
	./make-header.py --page-break "Chinese Remainder Theorem" >> $@.tmp
	cat resto_chines/trc.cpp >> $@.tmp
	
	./make-header.py "Upper Envelope" >> $@.tmp
	cat upper_envelope/upper_envelope.cpp >> $@.tmp
	
	./make-header.py --page-break "Rabin-Karp (randomized String Matching)" >> $@.tmp
	cat rabin-karp/rabin-karp2.cpp >> $@.tmp
	
	./make-header.py --page-break "Aho-Corasick" >> $@.tmp
	cat aho-corasick/aho-corasick2.cpp >> $@.tmp
	
	./make-header.py "Suffix Array" >> $@.tmp
	cat suffix_array/suffix_array.cpp >> $@.tmp
	
	echo '' >> $@.tmp
	
	iconv -f UTF-8 -t LATIN1 $@.tmp > $@
	rm -f $@.tmp

library.pdf: library.txt
	ps2pdf -dNOSAFER -sPAPERSIZE=a4 template.ps < library.txt
	mv template.pdf library.pdf

problem-table.pdf: problem-table.ps
	rm -f problem-table.latin1.ps problem-table.pdf
	iconv -f utf8 -t latin1 -o problem-table.latin1.ps problem-table.ps
	ps2pdf -sPAPERSIZE=a4 problem-table.latin1.ps
	mv problem-table.latin1.pdf problem-table.pdf
	rm -f problem-table.latin1.ps
