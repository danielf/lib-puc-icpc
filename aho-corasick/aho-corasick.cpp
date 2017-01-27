#include <map>
#include <list>
#include <queue>
#include <string>

const int MAX_NO = 100010;
const int MAX_PAD = 1010;

typedef map<char, int> mapach;
typedef map<string, int> mapastr;

struct automato {
  mapach trans[MAX_NO];
  mapastr pad;
  list<int> pos[MAX_PAD];
  int falha[MAX_NO], final[MAX_NO], tam[MAX_PAD], numNos;

  automato(): numNos(0) {}

  //////////////////////////////////////////////////////////////////////////////
  // Função de inicialização.
  //
  // Uma chamada por instância, antes de todas as outras funções.
  //

  void inic() {
    memset(falha, NULO, sizeof(falha));
    memset(final, NULO, sizeof(final));
    for (int i = 0; i < numNos; i++) trans[i].clear();
    pad.clear(); numNos = 1;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Função que adiciona um padrão ao autômato reconhecedor.
  //
  // Uma chamada por padrão, depois da inicialização.
  // Retorna o índice de acesso a variável global pos.
  //

  int adiciona_padrao(char* s) {
    pair<mapach::iterator, bool> pch;
    int i, no = 0, numPad = pad.size();

    if (pad.count(s)) return pad[s];
    else pad.insert(make_pair(s, numPad));

    for (i = 0; s[i]; i++) {
      if ((pch = trans[no].insert(make_pair(s[i], numNos))).second) numNos++;
      no = pch.first->second;
    }

    tam[numPad] = i ? i : 1;
    return final[no] = numPad;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Função que gera o tratamento de falhas.
  //
  // Uma chamada por instância, depois da adição de todos os padrões.
  //

  void gera_falhas() {
    queue<int> fila;
    int filho;
    foreach (it, all(trans[0])) {
      falha[filho = it->second] = 0;
      fila.push(filho);
    }

    while (!fila.empty()) {
      int atual = fila.front(); fila.pop();

      foreach (it, all(trans[atual])) {
        char c = it->first; filho = it->second; int ret = falha[atual];

        while (ret != NULO && !trans[ret].count(c))
          ret = falha[ret];

        if (ret != NULO) {
          falha[filho] = trans[ret][c];
          if (final[filho] == NULO && final[falha[filho]] != NULO)
            final[filho] = final[falha[filho]];
        } else if (trans[0].count(c)) falha[filho] = trans[0][c];

        fila.push(filho);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Função que busca os padrões em uma cadeia de consulta.
  //
  // Uma chamada por consulta, depois da geração do tratamento de falhas.
  // Preenche a variável global pos.
  //

  void consulta(char* s) {
    int ret, atual = 0, i = 0;

    int N = pad.size();
    for (int j = 0; j < N; j++) pos[j].clear();

    do {
      while (atual != NULO && !trans[atual].count(s[i]))
        atual = falha[atual];
      atual = (atual == NULO) ? 0 : trans[atual][s[i]];

      for (ret = atual; ret != NULO && final[ret] != NULO; ret = falha[ret]) {
        pos[final[ret]].push_back(i - tam[final[ret]] + 1);
        while (falha[ret] != NULO && final[falha[ret]] == final[ret])
          ret = falha[ret];
      }
    } while (s[i++]);
  }
};
