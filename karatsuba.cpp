#include <string.h>

int aux[16][3][100010]; // a primeira dimensao deve respeitar (lg GRAU_MAX)
                        // e a terceira (2 * GRAU_MAX)
int MOD; // definir o modulo!
const int LIM = 35; 

// Assume que r[] ja esta todo zerado, e p1[] e p2[] normalizados em MOD
// g1 eh o grau de p1[] e g2 o grau de p2[]. _lvl nao deve ser usado
// o x^0 estah no indice 0, e o x^n no indice n do vetor passado

// Complexidade: O(n ^ lg 3) = O(n ^ 1.58)
// start: a4f09b1b3dc17c0758b7a5a914f0129c
void karatsuba(int r[], int g1, int p1[], int g2, int p2[], int _lvl = 0) {
	if (g1 <= LIM || g2 <= LIM) { // otimizacoes constantes
		for (int i = 0; i <= g1; i++)
			for (int j = 0; j <= g2; j++)
				r[i+j] = (r[i+j] + p1[i]*p2[j])%MOD;
		return;
	}
	int grau = max(g1, g2);
	int grau2 = grau/2;

	// A0*B0
	karatsuba(r, grau2, p1, grau2, p2, _lvl+1);

	// A1*B1
	karatsuba(r+2*grau2+2, g1-grau2-1, p1+grau2+1, g2-grau2-1, p2+grau2+1, _lvl+1);

	// (A0+A1)*(B0+B1)
	for (int i = 0; i <= grau2; i++)
		aux[_lvl][0][i] = (p1[i] + ((i + grau2 + 1 <= g1)?p1[i+grau2+1]:0)) % MOD;
	for (int i = 0; i <= grau2; i++)
		aux[_lvl][1][i] = (p2[i] + ((i + grau2 + 1 <= g2)?p2[i+grau2+1]:0)) % MOD;
	memset(aux[_lvl][2], 0, (grau + 2)*sizeof(int));
	karatsuba(aux[_lvl][2], grau2, aux[_lvl][0], grau2, aux[_lvl][1], _lvl+1);

	// poe resultado em r[]
	for (int i = 0; i <= 2*grau2; i++)
		aux[_lvl][2][i] = (aux[_lvl][2][i] + 2 * MOD - r[i] - r[i+2*grau2+2]) % MOD;
	for (int i = 0; i <= 2*grau2; i++)
		r[grau2 + i + 1] = (r[grau2 + i + 1] + aux[_lvl][2][i]) % MOD;
}

