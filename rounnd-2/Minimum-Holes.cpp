#include <bits/stdc++.h>
using namespace std;


struct GeneralMatching {
    int n;           
    vector<vector<int>> g;
    vector<int> match;  
    GeneralMatching(int n = 0) { init(n); }
    void init(int _n) {
        n = _n;
        g.assign(n, {});
        match.assign(n, -1);
    }
    void addEdge(int u, int v) {
        if (u == v) return;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int maxMatching() {
        vector<int> base(n), p(n), q;
        vector<int> used(n), blossom(n);
        vector<int> link(n);

        function<int(int,int)> lca = [&](int a, int b) {
            vector<char> usedPath(n, false);
            while (true) {
                a = base[a];
                usedPath[a] = true;
                if (match[a] == -1) break;
                a = p[match[a]];
            }
            while (true) {
                b = base[b];
                if (usedPath[b]) return b;
                b = p[match[b]];
            }
        };

        function<void(int,int,int,deque<int>&)> markPath = [&](int v, int b, int children, deque<int> &Q) {
            while (base[v] != b) {
                blossom[base[v]] = blossom[base[match[v]]] = 1;
                p[v] = children;
                children = match[v];
                Q.push_back(children);
                v = p[children];
            }
        };

        function<int(int)> findPath = [&](int root) -> int {
            used.assign(n, 0);
            p.assign(n, -1);
            for (int i = 0; i < n; ++i) base[i] = i;
            deque<int> Q;
            Q.push_back(root);
            used[root] = 1;
            while (!Q.empty()) {
                int v = Q.front(); Q.pop_front();
                for (int to : g[v]) {
                    if (base[v] == base[to] || match[v] == to) continue;
                    if (to == root || (match[to] != -1 && p[match[to]] != -1)) {
                        // blossom found
                        fill(blossom.begin(), blossom.end(), 0);
                        int curbase = lca(v, to);
                        markPath(v, curbase, to, Q);
                        markPath(to, curbase, v, Q);
                        for (int i = 0; i < n; ++i)
                            if (blossom[base[i]]) {
                                base[i] = curbase;
                                if (!used[i]) {
                                    used[i] = 1;
                                    Q.push_back(i);
                                }
                            }
                    } else if (p[to] == -1) {
                        p[to] = v;
                        if (match[to] == -1)
                            return to;
                        to = match[to];
                        used[to] = 1;
                        Q.push_back(to);
                    }
                }
            }
            return -1;
        };

        int res = 0;
        base.resize(n);
        p.resize(n);
        used.resize(n);
        blossom.resize(n);

        for (int v = 0; v < n; ++v) {
            if (match[v] == -1) {
                int to = findPath(v);
                if (to == -1) continue;
                // augment
                while (to != -1) {
                    int pv = p[to];
                    int nv = match[pv];
                    match[to] = pv;
                    match[pv] = to;
                    to = nv;
                }
            }
        }
        for (int i = 0; i < n; ++i) if (match[i] != -1 && i < match[i]) ++res;
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    int N; cin >> N;

    vector<array<int,4>> segs;
    vector<int> xs = {0, n}, ys = {0, m};

    for (int i = 0; i < N; ++i) {
        int x1,y1,x2,y2;
        cin >> x1 >> y1 >> x2 >> y2;
        // ignore zero-length segments
        if (x1 == x2 && y1 == y2) continue;
        // must be axis aligned
        if (x1 == x2) {
            if (y1 > y2) swap(y1,y2);
        } else if (y1 == y2) {
            if (x1 > x2) swap(x1,x2);
        } else {
            cout << "Invalid\n";
            return 0;
        }
        // inside box
        if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > n || x2 > n || y1 > m || y2 > m) {
            cout << "Invalid\n";
            return 0;
        }
        segs.push_back({x1,y1,x2,y2});
        xs.push_back(x1); xs.push_back(x2);
        ys.push_back(y1); ys.push_back(y2);
    }

    sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int X = (int)xs.size();
    int Y = (int)ys.size();
    if (X <= 1 || Y <= 1) { cout << "Invalid\n"; return 0; }

    auto xi = [&](int v){ return int(lower_bound(xs.begin(), xs.end(), v) - xs.begin()); };
    auto yi = [&](int v){ return int(lower_bound(ys.begin(), ys.end(), v) - ys.begin()); };

    int W = X - 1, H = Y - 1;
    if (W <= 0 || H <= 0) { cout << "Invalid\n"; return 0; }

    // walls arrays
    vector<vector<bool>> verticalWalls(X, vector<bool>(H, false));
    vector<vector<bool>> horizontalWalls(W, vector<bool>(Y, false));

    // outer borders
    for (int cx = 0; cx < W; ++cx) {
        horizontalWalls[cx][0] = true;
        horizontalWalls[cx][Y-1] = true;
    }
    for (int cy = 0; cy < H; ++cy) {
        verticalWalls[0][cy] = true;
        verticalWalls[X-1][cy] = true;
    }

    // mark segments
    for (auto &s : segs) {
        int x1=s[0], y1=s[1], x2=s[2], y2=s[3];
        if (x1 == x2) {
            int vx = xi(x1), sy = yi(y1), ey = yi(y2);
            if (vx < 0 || vx >= X) { cout << "Invalid\n"; return 0; }
            for (int k = sy; k < ey; ++k) {
                if (k < 0 || k >= H) { cout << "Invalid\n"; return 0; }
                verticalWalls[vx][k] = true;
            }
        } else {
            int hy = yi(y1), sx = xi(x1), ex = xi(x2);
            if (hy < 0 || hy >= Y) { cout << "Invalid\n"; return 0; }
            for (int k = sx; k < ex; ++k) {
                if (k < 0 || k >= W) { cout << "Invalid\n"; return 0; }
                horizontalWalls[k][hy] = true;
            }
        }
    }

    // flood-fill components on cells
    vector<vector<int>> comp(W, vector<int>(H, -1));
    int compCount = 0;
    for (int cx = 0; cx < W; ++cx) {
        for (int cy = 0; cy < H; ++cy) {
            if (comp[cx][cy] != -1) continue;
            queue<pair<int,int>> q;
            q.push({cx,cy});
            comp[cx][cy] = compCount;
            while (!q.empty()) {
                auto cur = q.front(); q.pop();
                int x = cur.first, y = cur.second;
                if (x-1 >= 0 && !verticalWalls[x][y] && comp[x-1][y] == -1) {
                    comp[x-1][y] = compCount; q.push({x-1,y});
                }
                if (x+1 < W && !verticalWalls[x+1][y] && comp[x+1][y] == -1) {
                    comp[x+1][y] = compCount; q.push({x+1,y});
                }
                if (y-1 >= 0 && !horizontalWalls[x][y] && comp[x][y-1] == -1) {
                    comp[x][y-1] = compCount; q.push({x,y-1});
                }
                if (y+1 < H && !horizontalWalls[x][y+1] && comp[x][y+1] == -1) {
                    comp[x][y+1] = compCount; q.push({x,y+1});
                }
            }
            ++compCount;
        }
    }

    vector<int> minX(compCount, INT_MAX), minY(compCount, INT_MAX), maxX(compCount, INT_MIN), maxY(compCount, INT_MIN), cnt(compCount,0);
    for (int x = 0; x < W; ++x) for (int y = 0; y < H; ++y) {
        int id = comp[x][y];
        if (id < 0 || id >= compCount) { cout << "Invalid\n"; return 0; }
        minX[id] = min(minX[id], x);
        minY[id] = min(minY[id], y);
        maxX[id] = max(maxX[id], x);
        maxY[id] = max(maxY[id], y);
        ++cnt[id];
    }
    for (int id = 0; id < compCount; ++id) {
        long long w = (long long)(maxX[id] - minX[id] + 1);
        long long h = (long long)(maxY[id] - minY[id] + 1);
        if (w * h != cnt[id]) { cout << "Invalid\n"; return 0; }
    }

    int OUTSIDE = compCount;
    int V = compCount + 1;
    GeneralMatching gm(V);

    // vertical walls
    for (int vx = 0; vx < X; ++vx) {
        for (int y = 0; y < H; ++y) {
            if (!verticalWalls[vx][y]) continue;
            int lc = vx - 1, rc = vx;
            bool leftOK = (lc >= 0 && lc < W), rightOK = (rc >= 0 && rc < W);
            if (leftOK && rightOK) {
                int a = comp[lc][y], b = comp[rc][y];
                if (a != b) { gm.addEdge(a, b); }
            } else if (leftOK && !rightOK) {
                int a = comp[lc][y]; gm.addEdge(a, OUTSIDE);
            } else if (!leftOK && rightOK) {
                int b = comp[rc][y]; gm.addEdge(b, OUTSIDE);
            }
        }
    }

    // horizontal walls
    for (int hy = 0; hy < Y; ++hy) {
        for (int x = 0; x < W; ++x) {
            if (!horizontalWalls[x][hy]) continue;
            int dc = hy - 1, uc = hy;
            bool downOK = (dc >= 0 && dc < H), upOK = (uc >= 0 && uc < H);
            if (downOK && upOK) {
                int a = comp[x][dc], b = comp[x][uc];
                if (a != b) { gm.addEdge(a, b); }
            } else if (downOK && !upOK) {
                int a = comp[x][dc]; gm.addEdge(a, OUTSIDE);
            } else if (!downOK && upOK) {
                int b = comp[x][uc]; gm.addEdge(b, OUTSIDE);
            }
        }
    }

    if (gm.g[OUTSIDE].empty()) {
        // rebuild gm with V-1 vertices (no OUTSIDE)
        GeneralMatching gm2(V-1);
        for (int u = 0; u < V-1; ++u)
            for (int v : gm.g[u])
                if (v < V-1) gm2.addEdge(u, v);
        gm = move(gm2);
        V = V - 1;
    }

    for (int v = 0; v < V; ++v) {
        if (gm.g[v].empty()) {
            if (V == 1) { cout << 1 << '\n'; return 0; }
            cout << "Invalid\n";
            return 0;
        }
    }

    int matching = gm.maxMatching();

    int minEdgeCover = V - matching;
    cout << minEdgeCover << '\n';
    return 0;
}