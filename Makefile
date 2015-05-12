.PHONY: clean

clean:
	rm -f modelo.cpp library.txt library.txt.tmp library.pdf

#template.cpp: header.cpp main.cpp
#	rm -f $@
#	touch $@
#	cat header.cpp >> $@
#	echo >> $@
#	cat main.cpp >> $@

library.txt:
	rm -f $@ $@.tmp
	touch $@.tmp
	./make-header.py --no-line-break "Summary" >> $@.tmp
	cat index.txt >> $@.tmp
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
	
	./make-header.py --page-break "Kruskal" >> $@.tmp
	cat kruskal/kruskal.cpp >> $@.tmp
	
	./make-header.py "Dijkstra - PRIM" >> $@.tmp
	cat kruskal/dijkstra.cpp >> $@.tmp
	
	./make-header.py --page-break "LCA" >> $@.tmp
	cat kruskal/lca.cpp >> $@.tmp
	
	./make-header.py --page-break "Segment Tree" >> $@.tmp
	cat segment_tree/segtree.cpp >> $@.tmp
	
	./make-header.py --page-break "Segment Tree (lazy update)" >> $@.tmp
	cat segment_tree/segtree_lazy.cpp >> $@.tmp
	
	./make-header.py "Union Find" >> $@.tmp
	cat kruskal/unionfind.cpp >> $@.tmp
	
	./make-header.py --page-break "RMQ (Range Minimum Query)" >> $@.tmp
	cat RMQ/RMQ.cpp >> $@.tmp
	
	./make-header.py --page-break "BIT (Binary Index Tree)" >> $@.tmp
	cat BIT/bit.cpp >> $@.tmp
	
	./make-header.py --page-break "Suffix Array" >> $@.tmp
	cat suffix/suffix.cpp >> $@.tmp
	
	./make-header.py --page-break "Chinese Remainder Theorem" >> $@.tmp
	cat resto_chines/trc.cpp >> $@.tmp
	
	./make-header.py --page-break "Rabin-Karp (randomized String Matching)" >> $@.tmp
	cat rabin-karp/rabin-karp.cpp >> $@.tmp
	
	./make-header.py --page-break "FFT" >> $@.tmp
	cat FFT.cpp >> $@.tmp
	
	./make-header.py --page-break "Trie (Example of)" >> $@.tmp
	cat trie.cpp >> $@.tmp
	
	./make-header.py --page-break "Aho-Corasick" >> $@.tmp
	cat aho-corasick/aho-corasick.cpp >> $@.tmp
	
	./make-header.py --page-break "Polynomials (not very stable!)" >> $@.tmp
	cat polynomial/polynomial.cpp >> $@.tmp
	
	./make-header.py --page-break "Karatsuba (integer polynomials multiplication)" >> $@.tmp
	cat karatsuba.cpp >> $@.tmp
	
	./make-header.py --page-break "Scheduling" >> $@.tmp
	cat scheduling-notes.txt >> $@.tmp
	
	
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	echo " " >> $@.tmp
	echo '' >> $@.tmp
	
	iconv -f utf8 -t latin1 -o $@ $@.tmp
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
