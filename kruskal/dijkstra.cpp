typedef vector<vector<pair<int, int>>> graph;
pair<vi, vi> dijkstra(graph& G, int init) {
	int N = sz(G);
	priority_queue<pair<int, int>> heap;
	heap.push({0, init});
	vi ans(N, INF);
	vi P(N, -1);
	vector<bool> mark(N, false);
	ans[init] = 0;
	P[init] = init;
	while (!heap.empty()) {
		int x = heap.top().second; heap.pop();
		if (mark[x]) continue;
		mark[x] = true;
		for (auto &y : G[x]) {
			// delete ans[x] for PRIM
			int nd = y.second + ans[x];
			if (nd < ans[y.first]) {
				P[y.first] = x;
				ans[y.first] = nd;
				heap.push({-nd, y.first});
			}
		}
	}
	return {ans, P};
}
