#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<class ld>
struct Polygon {
    using pd = pair<ld, ld>;
#define X first
#define Y second
    vector<pd> p;
    int n;
    static inline bool OnSegment(const pd& a, const pd& b, const pd& c) {
        return (b.X <= max(a.X, c.X) && b.X >= min(a.X, c.X) && b.Y <= max(a.Y, c.Y) && b.Y >= min(a.Y, c.Y));
    }
    static inline ld CrossProduct(const pd& a, const pd& b, const pd& c) {
        return (b.X - a.X) * (b.Y - c.Y) - (b.Y - a.Y) * (b.X - c.X);
    }
    static inline int Orientation(const pd& a, const pd& b, const pd& c) {
        ld nr = CrossProduct(a, b, c);
        return (nr == 0) ? 0 : (nr > 0 ? 1 : -1);
    }
    static inline bool Intersect(const pd& a, const pd& b, const pd& c, const pd& d) {
        auto o1 = Orientation(a, b, c), o2 = Orientation(a, b, d), o3 = Orientation(c, d, a), o4 = Orientation(c, d, b);
        if (o1 != o2 && o3 != o4)
            return true;
        if (!o1 && OnSegment(a, c, b) || !o2 && OnSegment(a, d, b) || !o3 && OnSegment(c, a, d) || !o4 && OnSegment(c, b, d))
            return true;
        return false;
    }
    inline Polygon(const vector<pd>& List) {
        p = List;
        n = p.size();
    }
    inline int NrSides() {
        cout << n;
    }
    inline ld Area() {
        ld ans = 0;
        p.emplace_back(p[0]);
        for (int i = 0; i < n; ++i)
            ans += p[i].X * p[i + 1].Y - p[i].Y * p[i + 1].X;
        p.pop_back();
        return abs(ans);
    }
    inline ld Perimeter() {
        ld ans = 0;
        p.emplace_back(p[0]);
        for (int i = 0; i < n; ++i)
            ans += sqrt(p[i].X * p[i + 1].X + p[i].Y * p[i + 1].Y);
        p.pop_back();
        return ans;
    }
    inline pd COM() {
        pd ans(0, 0);
        for (const auto& nr : p)
            ans.X += nr.X, ans.Y += nr.Y;
        ans.X /= n, ans.Y /= n;
        return ans;
    }
    inline bool Intersects(const pd& Point1, const pd& Point2) {
        int ans = 0;
        p.emplace_back(p[0]);
        for (int i = 0; i < n; ++i)
            if (Intersect(p[i], p[i + 1], Point1, Point2)) {
                ans = 1;
                break;
            }
        p.pop_back();
        return ans;
    }
    inline ld Position(const pd& Point) {
        int cnt = 0, ok = 1;
        p.emplace_back(p[0]);
        for (int i = 0; i < n; ++i) {
            ld o = Orientation(p[i], p[i + 1], Point);
            if (!o && OnSegment(p[i], Point, p[i + 1])) {
                ok = 0;
                break;
            }
            if (p[i].Y <= Point.Y && Point.Y < p[i + 1].Y && o > 0)
                ++cnt;
            if (p[i].Y > Point.Y && Point.Y >= p[i + 1].Y && o < 0)
                --cnt;
        }
        p.pop_back();
        return ok ? (cnt ? 1 : -1) : 0;
    }
    inline bool Convex() {
        int ans = 1, positive = 0, negative = 0;
        p.emplace_back(p[0]);
        p.emplace_back(p[1]);
        for (int i = 1; i <= n; ++i) {
            auto current = Orientation(p[i - 1], p[i], p[i + 1]);
            positive |= (current > 0);
            negative |= (current < 0);
            if (positive & negative) {
                ans = 0;
                break;
            }
        }
        p.pop_back();
        p.pop_back();
        return ans;
    }
    void ConvexHull() {
        vector<pd> up, down, sol;
        sort(p.begin(), p.end());
        pd x = p[0], y = p.back();
        up.emplace_back(x), down.emplace_back(x);
        for (int i = 1; i < n; ++i) {
            if (Orientation(x, p[i], y) <= 0) {
                while (up.size() >= 2 && Orientation(up[up.size() - 2], up[up.size() - 1], p[i]) > 0)
                    up.pop_back();
                up.emplace_back(p[i]);
            }
            if (Orientation(x, p[i], y) >= 0) {
                while (down.size() >= 2 && Orientation(down[down.size() - 2], down[down.size() - 1], p[i]) < 0)
                    down.pop_back();
                down.emplace_back(p[i]);
            }
        }
        for (auto x : up)
            sol.emplace_back(x);
        for (auto it = down.rbegin() + 1; it != down.rend() - 1; ++it)
            sol.emplace_back(*it);
        p = sol;
        n = sol.size();
    }
    inline void Print() {
        cout << n << '\n';
        for (const auto& x : p)
            cout << x.first << ' ' << x.second << '\n';
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n;
    cin >> n;
    vector<pair<ll, ll>> a(n);
    for (auto& x : a)
        cin >> x.X >> x.Y;
    Polygon<ll> p(a);
    p.ConvexHull();
    p.Print();
}