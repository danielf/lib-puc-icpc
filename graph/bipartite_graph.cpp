struct bipartite_graph {
// start: 15d633d47577c09ead3f01702ac57f13  -
  int A, B;
  vector<vector<int>> adj;

  bipartite_graph(int _A, int _B) {
    A = _A, B = _B;
    adj.resize(A + B);
  }

  void edge(int i, int j) {
    adj[i].push_back(A+j);
    adj[A+j].push_back(i);
  }
// end

// start: 33ab9cb3460ef82cf446dcf63b153d72  -
  vector<int> visit, match;

  bool augment(int loc, int run) {
    if(visit[loc] == run) return false;
    visit[loc] = run;

    for (int nbr : adj[loc]) {
      if (match[nbr] == -1 || augment(match[nbr], run)) {
        match[loc] = nbr, match[nbr] = loc;
        return true;
      }
    }

    return false;
  }
// end

// start: 4bf94a144cb9a9695e95740e39644303  -
  int matching() {
    visit = vector<int>(A+B, -1);
    match = vector<int>(A+B, -1);

    int ans = 0;
    for (int i = 0; i < A; i++)
      ans += augment(i, i);
    return ans;
  }
// end

// start: 2257c2cea2b93264b3d24ded03d8425c  -
  vector<bool> vertex_cover() {
    vector<bool> res(A + B, false);
    queue<int> bfs;

    for (int i = 0; i < A; i++) {
      if (match[i] == -1) bfs.push(i);
      else res[i] = true;
    }

    while (!bfs.empty()) {
      int loc = bfs.front();
      bfs.pop();
      for (int nbr : adj[loc]) {
        if (res[nbr]) continue;
        res[nbr] = true;
        int loc2 = match[nbr];
        if (loc2 == -1) continue;
        res[loc2] = false;
        bfs.push(loc2);
      }
    }

    return res;
  }
// end
};

