.PHONY: clean

clean:
	rm -f modelo.cpp library.txt library.txt.tmp library.pdf

modelo.cpp: header.cpp main.cpp
	rm -f $@
	touch $@
	cat header.cpp >> $@
	echo >> $@
	cat main.cpp >> $@

library.txt: \
		index.txt \
		compila \
		modelo.cpp \
		vimrc.txt \
		hash.sh \
		modelo.java \
		roteiro/recomendacoes.txt \
		roteiro/roteiro.txt \
		roteiro/mandamentos.txt \
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
	./make-header.py --no-line-break "Índice" >> $@.tmp
	cat index.txt >> $@.tmp
	./make-header.py "Script de compilação C++" >> $@.tmp
	cat compila >> $@.tmp
	./make-header.py "Modelo Java" >> $@.tmp
	cat modelo.java >> $@.tmp
	./make-header.py --page-break "Modelo C++" >> $@.tmp
	cat modelo.cpp >> $@.tmp
	./make-header.py ".vimrc" >> $@.tmp
	cat vimrc.txt >> $@.tmp
	./make-header.py "hash.sh" >> $@.tmp
	cat hash.sh >> $@.tmp
	
	./make-header.py --page-break "Recomendações gerais" >> $@.tmp
	cat roteiro/recomendacoes.txt >> $@.tmp
	./make-header.py "Roteiro de prova" >> $@.tmp
	cat roteiro/roteiro.txt >> $@.tmp
	./make-header.py "Os 1010 Mandamentos" >> $@.tmp
	cat roteiro/mandamentos.txt >> $@.tmp
	echo '' >> $@.tmp
	
	cat sizes.txt >> $@.tmp
	
	./make-header.py --page-break "Primos até 10.000" >> $@.tmp
	cat primes/primes.txt >> $@.tmp
	
	./make-header.py --page-break "Utilidades do lar" >> $@.tmp
	cat util/util.cpp >> $@.tmp
	
	./make-header.py "Geometria" >> $@.tmp
	cat geometry/geometry.cpp >> $@.tmp
	
	./make-header.py --page-break "Inteiros de precisão arbitrária" >> $@.tmp
	cat bigint/bigint.cpp >> $@.tmp
	
	./make-header.py --page-break "Teoria dos números" >> $@.tmp
	cat number_theory/number_theory.cpp >> $@.tmp
	
	./make-header.py --page-break "Álgebra Linear" >> $@.tmp
	cat linear_algebra/linear_algebra.cpp >> $@.tmp
	
	./make-header.py --page-break "Simplex" >> $@.tmp
	cat simplex/simplex.cpp >> $@.tmp
	
	./make-header.py --page-break "Poker" >> $@.tmp
	cat poker/poker.cpp >> $@.tmp
	
	./make-header.py --page-break "Grafos" >> $@.tmp
	cat graph/graph.cpp >> $@.tmp
	
	./make-header.py --page-break "Aho-Corasick" >> $@.tmp
	cat aho-corasick/aho-corasick.cpp >> $@.tmp
	
	./make-header.py --page-break "Árvore de Segmentos" >> $@.tmp
	cat segment_tree/segment_tree.cpp >> $@.tmp
	
	./make-header.py --page-break "Polinômios  (Nota: não é muito estável!)" >> $@.tmp
	cat polynomial/polynomial.cpp >> $@.tmp
	
	./make-header.py --page-break "Karatsuba (mult. polinomios)" >> $@.tmp
	cat karatsuba.cpp >> $@.tmp
	
	./make-header.py --page-break "RMQ (Range Minimum Query)" >> $@.tmp
	cat rmq.cpp >> $@.tmp
	
	./make-header.py --page-break "BIT (Binary Index Tree)" >> $@.tmp
	cat bit.cpp >> $@.tmp
	
	./make-header.py --page-break "Teorema do Resto Chinês (TRC)" >> $@.tmp
	cat resto_chines/trc.cpp >> $@.tmp
	
	./make-header.py --page-break "Rabin-Karp (String Matching aleatorizado)" >> $@.tmp
	cat rabin-karp/rabin-karp.cpp >> $@.tmp
	
	./make-header.py --page-break "Notas sobre problemas clássicos de Scheduling" >> $@.tmp
	cat scheduling-notes.txt >> $@.tmp
	
	echo '' >> $@.tmp
	
	iconv -f utf8 -t latin1 -o $@ $@.tmp
	rm -f $@.tmp

library.pdf: library.txt
	ps2pdf -sPAPERSIZE=a4 template.ps < library.txt
	mv template.pdf library.pdf

problem-table.pdf: problem-table.ps
	rm -f problem-table.latin1.ps problem-table.pdf
	iconv -f utf8 -t latin1 -o problem-table.latin1.ps problem-table.ps
	ps2pdf -sPAPERSIZE=a4 problem-table.latin1.ps
	mv problem-table.latin1.pdf problem-table.pdf
	rm -f problem-table.latin1.ps
