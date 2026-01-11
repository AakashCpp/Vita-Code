#include <bits/stdc++.h>
using namespace std;

struct P { double x, y; };

inline double manh(const P &a, const P &b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y);
}

inline double cross_z(const P &p0, const P &p1, const P &p2) {
    return (p1.x - p0.x) * (p2.y - p1.y) - (p1.y - p0.y) * (p2.x - p1.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<P> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;

    double signed_area_acc = 0.0, per = 0.0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        signed_area_acc += p[i].x * p[j].y - p[j].x * p[i].y;
        per += manh(p[i], p[j]);
    }
    double area = 0.5 * fabs(signed_area_acc);
    int orient = (signed_area_acc >= 0.0) ? 1 : -1;

    vector<bool> isConcave(n, false);
    for (int i = 0; i < n; ++i) {
        P prev = p[(i + n - 1) % n];
        P cur  = p[i];
        P next = p[(i + 1) % n];
        double c = cross_z(prev, cur, next);
        isConcave[i] = (c * orient < -1e-12);
    }

    double minLen = 1e18;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        if (!isConcave[i] || !isConcave[j]) {
            double len = manh(p[i], p[j]);
            if (len < minLen) minLen = len;
        }
    }

    double result = 0.0;

    if (minLen > 0.1 + 1e-12) {
        double h_max = (minLen - 0.1) / 2.0;
        if (h_max >= 0.1 - 1e-12) {
            int steps = int(floor(h_max / 0.1 + 1e-9));
            double best = 0.0;
            for (int s = 1; s <= steps; ++s) {
                double h = s * 0.1;
                double baseA = area - per * h + 4.0 * h * h;
                double vol = baseA * h;
                if (vol > best) best = vol;
            }
            if (best > 1e-12) result = best;
        }
    }

    printf("%.2f\n", result);
    return 0;
}
