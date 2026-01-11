#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<string> rings(N);
    for (int i = 0; i < N; ++i) {
        cin >> rings[i];
    }

    // lengths of each ring
    vector<int> L(N);
    for (int r = 0; r < N; ++r) L[r] = static_cast<int>(rings[r].size());

    const int INF = numeric_limits<int>::max();
    vector<vector<int>> dist(N);
    for (int r = 0; r < N; ++r) dist[r] = vector<int>(L[r], INF);

    queue<pair<int,int>> q;

    // Initialize: entering any open cell on the outermost ring costs 1 step
    int outer = N - 1;
    for (int p = 0; p < L[outer]; ++p) {
        if (rings[outer][p] == '0') {
            dist[outer][p] = 1;
            q.push({outer, p});
        }
    }

    // BFS over nodes (r, p)
    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int r = cur.first;
        int p = cur.second;
        int d = dist[r][p];

        // move left and right on same ring (circular)
        int left = (p - 1 + L[r]) % L[r];
        int right = (p + 1) % L[r];

        if (rings[r][left] == '0' && dist[r][left] > d + 1) {
            dist[r][left] = d + 1;
            q.push({r, left});
        }
        if (rings[r][right] == '0' && dist[r][right] > d + 1) {
            dist[r][right] = d + 1;
            q.push({r, right});
        }

        // move inward (to r-1) if exists
        if (r > 0) {
            int inner = p / 2;
            if (rings[r-1][inner] == '0' && dist[r-1][inner] > d + 1) {
                dist[r-1][inner] = d + 1;
                q.push({r-1, inner});
            }
        }

        // move outward (to r+1) if exists: positions 2*p and 2*p+1
        if (r + 1 < N) {
            int out1 = 2 * p;
            int out2 = 2 * p + 1;
            if (out1 < L[r+1] && rings[r+1][out1] == '0' && dist[r+1][out1] > d + 1) {
                dist[r+1][out1] = d + 1;
                q.push({r+1, out1});
            }
            if (out2 < L[r+1] && rings[r+1][out2] == '0' && dist[r+1][out2] > d + 1) {
                dist[r+1][out2] = d + 1;
                q.push({r+1, out2});
            }
        }
    }

    // Compute answer: min over innermost open rooms (ring 0) of dist + 1 (enter center)
    int ans = INF;
    for (int p = 0; p < L[0]; ++p) {
        if (rings[0][p] == '0' && dist[0][p] != INF) {
            ans = min(ans, dist[0][p] + 1);
        }
    }

    if (ans == INF) {
        // No path found (depending on problem an unreachable case might not occur)
        cout << -1 << '\n';
    } else {
        cout << ans-1 << '\n';
    }

    return 0;
}