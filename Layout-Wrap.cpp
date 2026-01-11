#include <bits/stdc++.h>
using namespace std;

int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
struct Seg { int x1, y1, x2, y2; };
vector<Seg> segs;

bool pointInside(double px, double py) {
    int cnt = 0;
    for (auto &s : segs) {
        if (s.x1 == s.x2) {
            int x = s.x1, y1 = s.y1, y2 = s.y2;
            if (y1 > y2) swap(y1, y2);
            if (py >= y1 && py < y2 && x > px) ++cnt;
        }
    }
    return (cnt % 2) == 1;
}

int R, C;
vector<vector<char>> cell;

struct Rect { int r0, c0, h, w, id; };
vector<Rect> placed;

bool fits(int r0, int c0, int h, int w, const vector<vector<char>>& used) {
    if (r0 < 0 || c0 < 0 || r0 + h > R || c0 + w > C) return false;
    for (int r = r0; r < r0 + h; ++r)
        for (int c = c0; c < c0 + w; ++c)
            if (!cell[r][c] || used[r][c]) return false;
    return true;
}

void fillRect(int r0, int c0, int h, int w, vector<vector<char>>& used, int val) {
    for (int r = r0; r < r0 + h; ++r)
        for (int c = c0; c < c0 + w; ++c)
            used[r][c] = val;
}

bool validAdj(const vector<Rect>& rects) {
    int n = rects.size();
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Rect A = rects[i], B = rects[j];
            if (A.c0 + A.w == B.c0 || B.c0 + B.w == A.c0) {
                int top = max(A.r0, B.r0), bot = min(A.r0 + A.h, B.r0 + B.h);
                if (bot - top > 0) { adj[i].push_back(j); adj[j].push_back(i); }
            }
            if (A.r0 + A.h == B.r0 || B.r0 + B.h == A.r0) {
                int left = max(A.c0, B.c0), right = min(A.c0 + A.w, B.c0 + B.w);
                if (right - left > 0) {
                    bool ex = 0; for (int x : adj[i]) if (x == j) ex = 1;
                    if (!ex) { adj[i].push_back(j); adj[j].push_back(i); }
                }
            }
        }
    }
    int edges = 0;
    for (auto &v : adj) edges += v.size();
    edges /= 2;
    if (edges != n - 1) return false;
    vector<char> vis(n);
    queue<int> q;
    q.push(0);
    vis[0] = 1;
    int cnt = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; q.push(v); ++cnt; }
    }
    return cnt == n;
}

bool backtrack(vector<tuple<int, int, int>>& types, vector<vector<char>>& used) {
    int br = -1, bc = -1;
    for (int r = 0; r < R && br == -1; ++r)
        for (int c = 0; c < C && br == -1; ++c)
            if (cell[r][c] && !used[r][c]) { br = r; bc = c; }
    if (br == -1) return validAdj(placed);
    for (size_t i = 0; i < types.size(); ++i) {
        int w = get<0>(types[i]), h = get<1>(types[i]), &cnt = get<2>(types[i]);
        if (!cnt) continue;
        vector<pair<int, int>> ori = {{h, w}, {w, h}};
        for (auto [oh, ow] : ori) {
            if (fits(br, bc, oh, ow, used)) {
                fillRect(br, bc, oh, ow, used, 1);
                placed.push_back({br, bc, oh, ow, (int)i});
                --cnt;
                if (backtrack(types, used)) return true;
                ++cnt;
                placed.pop_back();
                fillRect(br, bc, oh, ow, used, 0);
            }
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N; cin >> N;
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        Seg s = {x1, y1, x2, y2};
        segs.push_back(s);
        minX = min({minX, x1, x2});
        minY = min({minY, y1, y2});
        maxX = max({maxX, x1, x2});
        maxY = max({maxY, y1, y2});
    }
    for (auto &s : segs) {
        s.x1 -= minX; s.x2 -= minX;
        s.y1 -= minY; s.y2 -= minY;
    }
    maxX -= minX; maxY -= minY;
    C = maxX; R = maxY;
    if (R <= 0 || C <= 0) { cout << 0; return 0; }
    cell.assign(R, vector<char>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            cell[r][c] = pointInside(c + 0.5, r + 0.5);
    int area = 0;
    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) if (cell[r][c]) ++area;
    if (!area) { cout << 0; return 0; }

    for (int a = 1; a <= area; ++a) {
        for (int b = a; b <= area; ++b) {
            long long denom = 2LL * (a + b);
            long long numer = area - 2LL * a * b;
            if (denom <= 0 || numer <= 0 || numer % denom) continue;
            long long c = numer / denom;
            if (c < b || c > area) continue;
            vector<tuple<int, int, int>> types = {
                {a, b, 2}, {a, (int)c, 2}, {b, (int)c, 2}
            };
            placed.clear();
            vector<vector<char>> used(R, vector<char>(C));
            if (backtrack(types, used)) {
                cout << a * b * c;
                return 0;
            }
        }
    }
    cout << 0;
}