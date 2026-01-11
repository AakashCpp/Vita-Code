#include <bits/stdc++.h>
using namespace std;

inline uint64_t packPerm(const int *a, int n) {
    uint64_t v = 0;
    for (int i = 0; i < n; ++i) v |= (uint64_t(a[i]) & 0xFULL) << (4 * i);
    return v;
}
inline void unpackPerm(uint64_t v, int *a, int n) {
    for (int i = 0; i < n; ++i) a[i] = int((v >> (4 * i)) & 0xFULL);
}

int bidir_min_moves(int n, const vector<int> &start, const vector<int> &target) {
    if (start == target) return 0;
    int bufA[10], bufB[10];
    for (int i = 0; i < n; ++i) bufA[i] = start[i], bufB[i] = target[i];
    uint64_t sKey = packPerm(bufA, n), tKey = packPerm(bufB, n);

    unordered_map<uint64_t,int> d1, d2;
    d1.reserve(1<<14); d2.reserve(1<<14);
    queue<uint64_t> q1, q2;
    d1[sKey] = 0; q1.push(sKey);
    d2[tKey] = 0; q2.push(tKey);

    int cur[10], nxt[10];
    while (!q1.empty() && !q2.empty()) {
        if (q1.size() <= q2.size()) {
            int qs = q1.size();
            while (qs--) {
                uint64_t key = q1.front(); q1.pop();
                unpackPerm(key, cur, n);
                int depth = d1[key];
                for (int l = 0; l < n; ++l) {
                    for (int r = l; r < n; ++r) {
                        int len = r - l + 1;
                        int maxPos = n - len;
                        for (int pos = 0; pos <= maxPos; ++pos) {
                            if (pos == l) continue;
                            int idx = 0;
                            if (pos < l) {
                                for (int i = 0; i < pos; ++i) nxt[idx++] = cur[i];
                                for (int i = l; i <= r; ++i) nxt[idx++] = cur[i];
                                for (int i = pos; i < l; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1; i < n; ++i) nxt[idx++] = cur[i];
                            } else { 
                                for (int i = 0; i < l; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1; i < r+1 + (pos - l); ++i) nxt[idx++] = cur[i];
                                for (int i = l; i <= r; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1 + (pos - l); i < n; ++i) nxt[idx++] = cur[i];
                            }
                            uint64_t nk = packPerm(nxt, n);
                            if (d1.find(nk) == d1.end()) {
                                d1[nk] = depth + 1;
                                auto it = d2.find(nk);
                                if (it != d2.end()) return d1[nk] + it->second;
                                q1.push(nk);
                            }
                        }
                    }
                }
            }
        } else {
            int qs = q2.size();
            while (qs--) {
                uint64_t key = q2.front(); q2.pop();
                unpackPerm(key, cur, n);
                int depth = d2[key];
                for (int l = 0; l < n; ++l) {
                    for (int r = l; r < n; ++r) {
                        int len = r - l + 1;
                        int maxPos = n - len;
                        for (int pos = 0; pos <= maxPos; ++pos) {
                            if (pos == l) continue;
                            int idx = 0;
                            if (pos < l) {
                                for (int i = 0; i < pos; ++i) nxt[idx++] = cur[i];
                                for (int i = l; i <= r; ++i) nxt[idx++] = cur[i];
                                for (int i = pos; i < l; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1; i < n; ++i) nxt[idx++] = cur[i];
                            } else {
                                for (int i = 0; i < l; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1; i < r+1 + (pos - l); ++i) nxt[idx++] = cur[i];
                                for (int i = l; i <= r; ++i) nxt[idx++] = cur[i];
                                for (int i = r+1 + (pos - l); i < n; ++i) nxt[idx++] = cur[i];
                            }
                            uint64_t nk = packPerm(nxt, n);
                            if (d2.find(nk) == d2.end()) {
                                d2[nk] = depth + 1;
                                auto it = d1.find(nk);
                                if (it != d1.end()) return d2[nk] + it->second;
                                q2.push(nk);
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    string line;
    getline(cin, line);

    while (getline(cin, line)) { if (line.find("shuffled") != string::npos) break; }

    vector<string> shuffled;
    shuffled.reserve(N);
    for (int i = 0; i < N; ++i) {
        string s;
        getline(cin, s);
        if (s.size() == 0) { --i; continue; }
        shuffled.push_back(s);
    }

    while (getline(cin, line)) { if (line.find("original") != string::npos) break; }

    vector<string> original;
    original.reserve(N);
    for (int i = 0; i < N; ++i) {
        string s;
        getline(cin, s);
        if (s.size() == 0) { --i; continue; }
        original.push_back(s);
    }

    unordered_map<string,int> pos;
    pos.reserve(N*2);
    for (int i = 0; i < N; ++i) pos[original[i]] = i;

    vector<int> start(N), target(N);
    for (int i = 0; i < N; ++i) start[i] = pos[shuffled[i]];
    for (int i = 0; i < N; ++i) target[i] = i;

    int ans = bidir_min_moves(N, start, target);
    if (ans < 0) ans = N;
    cout << ans << '\n';
    return 0;
}
