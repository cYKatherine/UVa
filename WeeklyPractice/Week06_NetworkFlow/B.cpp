#include <bits/stdc++.h>

using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

template<typename T> struct Dinics {
    struct edge { int to; T flow, cap; };  T INF = numeric_limits<T>::max();
    int n, m; vi dist, work; queue<int> q;  vector<edge> edges; vvi adj;
    bool bfs(int s, int t) {
        dist.assign(n, -1);
        dist[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto & i : adj[u]) {
                edge& e  = edges[i];  int v = e.to;
                if (dist[v] < 0 && e.flow < e.cap) dist[v] = dist[u] + 1, q.push(v);
            }
        }
        return dist[t] >= 0;
    }
    T dfs(int u, int t, T f) {
        if (u == t) return f;
        for (int & i = work[u]; i < (int)adj[u].size(); i++) {
            auto& e = edges[adj[u][i]], &rev = edges[adj[u][i]^1];
            if (e.flow < e.cap && dist[e.to] == dist[u] + 1) {
                T df = dfs(e.to, t, min(f, e.cap-e.flow));
                if (df > 0) { e.flow += df; rev.flow -= df; return df;}
            }
        }
        return 0;
    }
    // Create a flow network with n nodes -- add edges with add_edge(u,v,cap)
    Dinics(int n) : n(n), m(0), adj(n) { }
    int add_edge(int from, int to, T cap) { // add an edge (from -> to) with
        adj[from].push_back(m++), adj[to].push_back(m++); // capacity of cap units.
        edges.push_back({to, 0, cap}), edges.push_back({from, 0, 0});
        return m - 2; // Change {from,0,0} to {from,0,cap} for bidirectional edges
    }
    edge& get_edge(int i) { return edges[i]; } // get a reference to the i’th edge
    T max_flow(int s, int t) { // find the maximum flow from s to t
        T res = 0; for (auto& e : edges) e.flow = 0;
        while (work.assign(n, 0), bfs(s, t))
            while (T delta = dfs(s, t, INF)) res += delta;
        return res;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vi a(n+5), b(n+5);
    Dinics<long long> mf(2*n+2);
    int out[2*n+4][2*n+4];
    for (int i = 0; i < 2*n+4; i++) {
        for (int j = 0; j < 2*n+4; j++) {
            out[i][j] = -1;
        }
    }
    int checka = 0, checkb = 0, edge_count = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        mf.add_edge(0, 2*i+2, a[i]);
        edge_count += 2;
        checka += a[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
        mf.add_edge(2*i+3, 1, b[i]);
        edge_count += 2;
        checkb += b[i];
    }

    if (checka != checkb) {
        cout << "NO" << endl;
        return 0;
    }

    for (int i = 0; i < n; i++) {
        mf.add_edge(2*i+2, 2*i+3, 101);
        out[i][i] = edge_count;
        edge_count += 2;
    }
    for (int i = 0; i < m; i++) {
        int p, q;
        cin >> p >> q;
        p--;  q--;
        mf.add_edge(2*p+2, 2*q+3, 101);
        out[p][q] = edge_count;
        edge_count += 2;
        mf.add_edge(2*q+2, 2*p+3, 101);
        out[q][p] = edge_count;
        edge_count += 2;
    }
    int result = mf.max_flow(0, 1);
    if (result == checka) {
        cout << "YES"<< endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (out[i][j] != -1) {
                    cout << mf.get_edge(out[i][j]).flow << " ";
                }
                else cout << 0 << " ";
            }
            cout << endl;
        }
    }
    else cout << "NO" << endl;
    return 0;
}
