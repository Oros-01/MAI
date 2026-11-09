#include <algorithm>
#include <cstddef>
#include <deque>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "myvector.h"
#include "mydeque.h"
#include "test_runner.h"

std::vector<int> Snap(const MyVector<int>& v) {
    std::vector<int> r;
    for (size_t i = 0; i < v.size(); ++i) r.push_back(v[i]);
    return r;
}

std::vector<int> Snap(const MyDeque<int>& d) {
    std::vector<int> r;
    for (size_t i = 0; i < d.size(); ++i) r.push_back(d[i]);
    return r;
}

struct Counter {
    static int alive;
    int x;
    Counter(int v = 0) : x(v) { ++alive; }
    Counter(const Counter& o) : x(o.x) { ++alive; }
    Counter& operator=(const Counter& o) { x = o.x; return *this; }
    ~Counter() { --alive; }
};
int Counter::alive = 0;

struct Point { int x = 0, y = 0; };
bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }

void TestVectorBasic() {
    MyVector<int> v;
    ASSERT(v.empty());
    ASSERT_EQUAL(v.size(), 0u);
    v.push_back(42);
    ASSERT_EQUAL(v.size(), 1u);
    ASSERT_EQUAL(v[0], 42);
    v.push_back(0); v.push_back(-1); v.push_back(0); v.push_back(7); v.push_back(7);
    ASSERT_EQUAL(Snap(v), (std::vector<int>{42, 0, -1, 0, 7, 7}));
    v.clear(); v.clear();
    ASSERT(v.empty());
    v.push_back(10); v.push_back(20);
    ASSERT_EQUAL(Snap(v), (std::vector<int>{10, 20}));
    v.pop_back(); v.pop_back();
    ASSERT(v.empty());
    v.push_back(99);
    ASSERT_EQUAL(v[0], 99);
}

void TestVectorCopy() {
    MyVector<int> a;
    a.push_back(1); a.push_back(2); a.push_back(3);
    MyVector<int> b = a;
    ASSERT_EQUAL(Snap(a), (std::vector<int>{1, 2, 3}));
    ASSERT_EQUAL(Snap(b), (std::vector<int>{1, 2, 3}));
    b[0] = 999;
    ASSERT_EQUAL(a[0], 1);
    ASSERT_EQUAL(b[0], 999);
    a = a;
    ASSERT_EQUAL(Snap(a), (std::vector<int>{1, 2, 3}));

    MyVector<int> c; c.push_back(10); c.push_back(20);
    MyVector<int> d; d.push_back(1); d.push_back(2); d.push_back(3); d.push_back(4);
    d = c;
    ASSERT_EQUAL(Snap(d), (std::vector<int>{10, 20}));
    d[0] = 777;
    ASSERT_EQUAL(c[0], 10);

    MyVector<int> e; e = c;
    ASSERT_EQUAL(Snap(e), (std::vector<int>{10, 20}));

    MyVector<int> f; f.push_back(1); f = MyVector<int>();
    ASSERT(f.empty());

    MyVector<int> g; MyVector<int> h = g;
    ASSERT(h.empty());
}

void TestVectorAccess() {
    MyVector<int> v;
    v.push_back(10); v.push_back(20); v.push_back(30);
    ASSERT_EQUAL(v.front(), 10);
    ASSERT_EQUAL(v.back(), 30);
    ASSERT_EQUAL(v[0], 10);
    ASSERT_EQUAL(v[1], 20);
    ASSERT_EQUAL(v[2], 30);
    ASSERT_EQUAL(v.at(1), 20);
    v[0] = 100; v.at(1) = 200; v.front() = 300; v.back() = 400;
    ASSERT_EQUAL(Snap(v), (std::vector<int>{300, 200, 400}));
    const MyVector<int>& cv = v;
    ASSERT_EQUAL(cv.front(), 300);
    ASSERT_EQUAL(cv.back(), 400);
    ASSERT_EQUAL(cv[0], 300);
    ASSERT_EQUAL(cv[2], 400);
    ASSERT_EQUAL(cv.at(1), 200);
}

void TestVectorErrors() {
    bool c;
    { MyVector<int> v; c = false; try { v.pop_back(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyVector<int> v; c = false; try { v.front(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyVector<int> v; c = false; try { v.back(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyVector<int> v; v.push_back(1); v.push_back(2);
      c = false; try { v.at(2); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      c = false; try { v.at(100); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2})); }
    { MyVector<int> v; v.push_back(1);
      c = false; try { v.insert(5, 99); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      ASSERT_EQUAL(Snap(v), (std::vector<int>{1})); }
    { MyVector<int> v; v.push_back(1);
      c = false; try { v.erase(5); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      ASSERT_EQUAL(Snap(v), (std::vector<int>{1})); }
    { MyVector<int> v; c = false; try { v.erase(0); } catch (const std::out_of_range&) { c = true; } ASSERT(c); ASSERT(v.empty()); }
}

void TestVectorGrow() {
    MyVector<int> v;
    ASSERT_EQUAL(v.capacity(), 0u);
    v.push_back(1);
    while (v.size() < v.capacity()) v.push_back(100 + (int)v.size());
    const size_t cap1 = v.capacity();
    const std::vector<int> before = Snap(v);
    v.push_back(999);
    ASSERT(v.capacity() > cap1);
    std::vector<int> expected = before;
    expected.push_back(999);
    ASSERT_EQUAL(Snap(v), expected);
    for (int i = 0; i < 100; ++i) v.push_back(i);
    ASSERT(v.capacity() >= v.size());
    ASSERT_EQUAL(v.size(), expected.size() + 100u);
    for (size_t i = 0; i < expected.size(); ++i) ASSERT_EQUAL(v[i], expected[i]);
}

void TestVectorReserveResize() {
    MyVector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    const size_t cap = v.capacity();
    v.reserve(0); ASSERT_EQUAL(v.capacity(), cap);
    v.reserve(cap); ASSERT_EQUAL(v.capacity(), cap);
    v.reserve(cap + 10); ASSERT(v.capacity() >= cap + 10);
    ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2, 3}));
    v.resize(2); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2}));
    v.resize(2); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2}));
    v.resize(5); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2, 0, 0, 0}));
    v.resize(7, 9); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 2, 0, 0, 0, 9, 9}));
    v.resize(0); ASSERT(v.empty()); ASSERT(v.capacity() >= cap + 10);
    v.resize(3); ASSERT_EQUAL(Snap(v), (std::vector<int>{0, 0, 0}));
    MyVector<int> w; w.resize(100);
    ASSERT_EQUAL(w.size(), 100u);
    ASSERT_EQUAL(w[0], 0);
    ASSERT_EQUAL(w[99], 0);
}

void TestVectorInsertErase() {
    MyVector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    v.insert(0, 0); ASSERT_EQUAL(Snap(v), (std::vector<int>{0, 1, 2, 3}));
    v.insert(2, 9); ASSERT_EQUAL(Snap(v), (std::vector<int>{0, 1, 9, 2, 3}));
    v.insert(v.size(), 99); ASSERT_EQUAL(Snap(v), (std::vector<int>{0, 1, 9, 2, 3, 99}));
    MyVector<int> e; e.insert(0, 5); ASSERT_EQUAL(Snap(e), (std::vector<int>{5}));
    MyVector<int> o; o.push_back(7); o.insert(1, 8); ASSERT_EQUAL(Snap(o), (std::vector<int>{7, 8}));
    v.erase(0); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 9, 2, 3, 99}));
    v.erase(2); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 9, 3, 99}));
    v.erase(v.size() - 1); ASSERT_EQUAL(Snap(v), (std::vector<int>{1, 9, 3}));
    MyVector<int> s; s.push_back(42); s.erase(0); ASSERT(s.empty());
}

void TestVectorSelfRef() {
    MyVector<int> v; v.push_back(10); v.push_back(20);
    v.push_back(v[0]); ASSERT_EQUAL(Snap(v), (std::vector<int>{10, 20, 10}));
    MyVector<int> w; w.push_back(100); w.push_back(200);
    w.insert(1, w[0]); ASSERT_EQUAL(Snap(w), (std::vector<int>{100, 100, 200}));
    MyVector<int> r; r.push_back(7); r.push_back(8);
    r.resize(6, r[0]); ASSERT_EQUAL(Snap(r), (std::vector<int>{7, 8, 7, 7, 7, 7}));
    MyVector<int> s; s.push_back(1); s.reserve(1); s.push_back(s[0]);
    ASSERT_EQUAL(Snap(s), (std::vector<int>{1, 1}));
}

void TestVectorAlgorithms() {
    MyVector<int> v;
    v.push_back(3); v.push_back(1); v.push_back(3);
    std::vector<int> actual;
    for (const auto& x : v) actual.push_back(x);
    ASSERT_EQUAL(actual, (std::vector<int>{3, 1, 3}));
    for (auto& x : v) x *= 2;
    ASSERT_EQUAL(Snap(v), (std::vector<int>{6, 2, 6}));
    const MyVector<int>& cv = v;
    ASSERT(std::find(cv.begin(), cv.end(), 2) != cv.end());
    ASSERT_EQUAL(*std::find(cv.begin(), cv.end(), 2), 2);
    ASSERT(std::find(cv.begin(), cv.end(), 999) == cv.end());
    ASSERT_EQUAL(std::count(cv.begin(), cv.end(), 6), 2);
    std::reverse(v.begin(), v.end());
    ASSERT_EQUAL(Snap(v), (std::vector<int>{6, 2, 6}));
    MyVector<int> empty;
    ASSERT(empty.begin() == empty.end());
}

void TestDequeBasic() {
    MyDeque<int> d;
    ASSERT(d.empty());
    d.push_back(1);
    ASSERT_EQUAL(d.front(), 1);
    ASSERT_EQUAL(d.back(), 1);
    d.push_back(2); d.push_back(3);
    d.push_front(0); d.push_front(-1);
    ASSERT_EQUAL(Snap(d), (std::vector<int>{-1, 0, 1, 2, 3}));
    d.clear(); d.clear();
    ASSERT(d.empty());
    d.push_back(10); d.push_front(9);
    ASSERT_EQUAL(Snap(d), (std::vector<int>{9, 10}));
}

void TestDequeCopy() {
    MyDeque<int> a;
    a.push_back(1); a.push_back(2); a.push_front(0);
    MyDeque<int> b = a;
    ASSERT_EQUAL(Snap(a), (std::vector<int>{0, 1, 2}));
    ASSERT_EQUAL(Snap(b), (std::vector<int>{0, 1, 2}));
    b[0] = 999;
    ASSERT_EQUAL(a[0], 0);
    ASSERT_EQUAL(b[0], 999);
    a = a;
    ASSERT_EQUAL(Snap(a), (std::vector<int>{0, 1, 2}));

    MyDeque<int> c; c.push_back(10); c.push_back(20);
    MyDeque<int> d; d.push_back(1); d.push_back(2); d.push_back(3);
    d = c;
    ASSERT_EQUAL(Snap(d), (std::vector<int>{10, 20}));
    d[0] = 777;
    ASSERT_EQUAL(c[0], 10);

    MyDeque<int> e; e = c;
    ASSERT_EQUAL(Snap(e), (std::vector<int>{10, 20}));

    MyDeque<int> f; f.push_back(1); f = MyDeque<int>();
    ASSERT(f.empty());

    MyDeque<int> g; MyDeque<int> h = g;
    ASSERT(h.empty());
}

void TestDequeAccess() {
    MyDeque<int> d;
    d.push_back(1); d.push_back(2); d.push_back(3);
    d.push_front(0); d.push_front(-1);
    ASSERT_EQUAL(d.front(), -1);
    ASSERT_EQUAL(d.back(), 3);
    ASSERT_EQUAL(d[0], -1);
    ASSERT_EQUAL(d[1], 0);
    ASSERT_EQUAL(d[2], 1);
    ASSERT_EQUAL(d[3], 2);
    ASSERT_EQUAL(d[4], 3);
    ASSERT_EQUAL(d.at(2), 1);
    d[0] = 100; d.at(4) = 400; d.front() = 200; d.back() = 300;
    ASSERT_EQUAL(Snap(d), (std::vector<int>{200, 0, 1, 2, 300}));
    const MyDeque<int>& cd = d;
    ASSERT_EQUAL(cd.front(), 200);
    ASSERT_EQUAL(cd.back(), 300);
    ASSERT_EQUAL(cd[0], 200);
    ASSERT_EQUAL(cd[4], 300);
    ASSERT_EQUAL(cd.at(1), 0);
}

void TestDequeErrors() {
    bool c;
    { MyDeque<int> d; c = false; try { d.pop_front(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyDeque<int> d; c = false; try { d.pop_back(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyDeque<int> d; c = false; try { d.front(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyDeque<int> d; c = false; try { d.back(); } catch (const std::out_of_range&) { c = true; } ASSERT(c); }
    { MyDeque<int> d; c = false; try { d.at(0); } catch (const std::out_of_range&) { c = true; } ASSERT(c); ASSERT(d.empty()); }
    { MyDeque<int> d; d.push_back(1); d.push_back(2);
      c = false; try { d.at(2); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      c = false; try { d.at(100); } catch (const std::out_of_range&) { c = true; } ASSERT(c);
      ASSERT_EQUAL(Snap(d), (std::vector<int>{1, 2})); }
}

void TestDequeTwoEnds() {
    MyDeque<int> d;
    d.push_front(1);
    ASSERT_EQUAL(d.front(), 1);
    ASSERT_EQUAL(d.back(), 1);
    d.pop_front();
    ASSERT(d.empty());
    d.push_back(2); d.pop_back();
    ASSERT(d.empty());
    d.push_front(1); d.push_front(2); d.push_front(3);
    ASSERT_EQUAL(Snap(d), (std::vector<int>{3, 2, 1}));
    d.pop_back(); ASSERT_EQUAL(Snap(d), (std::vector<int>{3, 2}));
    d.pop_front(); ASSERT_EQUAL(Snap(d), (std::vector<int>{2}));
    d.pop_front(); ASSERT(d.empty());
    d.push_back(1); d.push_back(2); d.push_back(3);
    d.pop_front(); ASSERT_EQUAL(Snap(d), (std::vector<int>{2, 3}));
    d.pop_back(); ASSERT_EQUAL(Snap(d), (std::vector<int>{2}));
    d.pop_back(); ASSERT(d.empty());
    MyDeque<int> e;
    e.push_front(1); e.push_back(2); e.push_front(0); e.push_back(3);
    ASSERT_EQUAL(Snap(e), (std::vector<int>{0, 1, 2, 3}));
}

void TestDequeAlternate() {
    MyDeque<int> d;
    for (int i = 0; i < 20; ++i) d.push_back(i);
    for (int i = 0; i < 10; ++i) d.pop_front();
    ASSERT_EQUAL(d.front(), 10);
    ASSERT_EQUAL(d.back(), 19);
    for (int i = 0; i < 10; ++i) d.push_front(-i);
    ASSERT_EQUAL(d.size(), 20u);
    ASSERT_EQUAL(d.front(), -9);
    for (int i = 0; i < 10; ++i) d.pop_back();
    ASSERT_EQUAL(d.back(), 0);
    for (int i = 0; i < 10; ++i) d.pop_front();
    ASSERT(d.empty());
    MyDeque<int> e;
    for (int i = 0; i < 6; ++i) e.push_front(i);
    for (int i = 0; i < 6; ++i) { ASSERT_EQUAL(e.front(), 5 - i); e.pop_front(); }
    ASSERT(e.empty());
}

void TestDequeSelfRef() {
    MyDeque<int> d;
    d.push_back(10); d.push_back(20);
    d.push_front(d.back()); ASSERT_EQUAL(Snap(d), (std::vector<int>{20, 10, 20}));
    d.push_back(d.front()); ASSERT_EQUAL(Snap(d), (std::vector<int>{20, 10, 20, 20}));
    d.push_front(d.front()); ASSERT_EQUAL(Snap(d), (std::vector<int>{20, 20, 10, 20, 20}));
}

void TestTemplateTypes() {
    MyVector<std::string> v;
    v.push_back("a"); v.push_back("bb"); v.push_back("ccc");
    ASSERT_EQUAL(v[0], std::string("a"));
    v[1] = "xx";
    v.erase(0);
    ASSERT_EQUAL(v[0], std::string("xx"));
    MyVector<std::string> vc = v;
    ASSERT_EQUAL(vc.size(), 2u);

    MyDeque<std::string> d;
    d.push_back("x"); d.push_front("y"); d.push_back("z");
    ASSERT_EQUAL(d.front(), std::string("y"));
    ASSERT_EQUAL(d.back(), std::string("z"));
    d.pop_front();
    ASSERT_EQUAL(d.front(), std::string("x"));

    MyVector<Point> pv;
    pv.push_back(Point{1, 2}); pv.push_back(Point{3, 4}); pv.push_back(Point{5, 6});
    ASSERT(pv[1] == (Point{3, 4}));
    pv[1] = Point{9, 9};
    ASSERT(pv[1] == (Point{9, 9}));
    MyVector<Point> pvc = pv;
    ASSERT(pvc[2] == (Point{5, 6}));

    MyDeque<Point> pd;
    pd.push_back(Point{1, 2}); pd.push_front(Point{3, 4}); pd.push_back(Point{5, 6});
    ASSERT(pd.front() == (Point{3, 4}));
    pd.pop_front();
    ASSERT(pd.front() == (Point{1, 2}));
}

void TestObjectLifetime() {
    ASSERT_EQUAL(Counter::alive, 0);
    {
        MyVector<Counter> v;
        v.push_back(Counter(1)); v.push_back(Counter(2)); v.push_back(Counter(3));
        MyVector<Counter> v2 = v;
        MyVector<Counter> v3; v3 = v2;
        v.pop_back(); v.clear();
    }
    ASSERT_EQUAL(Counter::alive, 0);
    {
        MyDeque<Counter> d;
        for (int i = 0; i < 10; ++i) { d.push_back(Counter(i)); d.push_front(Counter(-i)); }
        MyDeque<Counter> d2 = d;
        for (int i = 0; i < 5; ++i) d.pop_front();
        for (int i = 0; i < 5; ++i) d.pop_back();
        d2.clear(); d.clear();
    }
    ASSERT_EQUAL(Counter::alive, 0);
}

void TestVectorRandom() {
    for (unsigned seed : {42u, 12345u, 2026u}) {
        std::mt19937 gen(seed);
        MyVector<int> mine;
        std::vector<int> ref;
        for (int step = 0; step < 10000; ++step) {
            int op = (int)(gen() % 10);
            if (op < 4 && ref.size() < 200) {
                int x = (int)(gen() % 1000) - 500;
                mine.push_back(x); ref.push_back(x);
            } else if (op < 6 && !ref.empty()) {
                mine.pop_back(); ref.pop_back();
            } else if (op < 7 && !ref.empty()) {
                size_t i = gen() % ref.size();
                int x = (int)(gen() % 1000);
                mine[i] = x; ref[i] = x;
            } else if (op < 8) {
                mine.clear(); ref.clear();
            } else if (op < 9 && !ref.empty()) {
                size_t i = gen() % ref.size();
                ASSERT_EQUAL(mine[i], ref[i]);
            } else if (ref.size() < 200) {
                size_t n = gen() % 10;
                mine.resize(n, 7); ref.resize(n, 7);
            }
            ASSERT_EQUAL(mine.size(), ref.size());
            for (size_t i = 0; i < ref.size(); ++i) ASSERT_EQUAL(mine[i], ref[i]);
            if (!ref.empty()) {
                ASSERT_EQUAL(mine.front(), ref.front());
                ASSERT_EQUAL(mine.back(), ref.back());
            }
        }
    }
}

void TestDequeRandom() {
    for (unsigned seed : {42u, 12345u, 2026u}) {
        std::mt19937 gen(seed);
        MyDeque<int> mine;
        std::deque<int> ref;
        for (int step = 0; step < 10000; ++step) {
            int op = (int)(gen() % 10);
            if (op < 3 && ref.size() < 200) {
                int x = (int)(gen() % 1000) - 500;
                mine.push_back(x); ref.push_back(x);
            } else if (op < 5 && ref.size() < 200) {
                int x = (int)(gen() % 1000) - 500;
                mine.push_front(x); ref.push_front(x);
            } else if (op < 6 && !ref.empty()) {
                mine.pop_back(); ref.pop_back();
            } else if (op < 7 && !ref.empty()) {
                mine.pop_front(); ref.pop_front();
            } else if (op < 8 && !ref.empty()) {
                size_t i = gen() % ref.size();
                int x = (int)(gen() % 1000);
                mine[i] = x; ref[i] = x;
            } else if (op < 9) {
                mine.clear(); ref.clear();
            } else if (!ref.empty()) {
                size_t i = gen() % ref.size();
                ASSERT_EQUAL(mine[i], ref[i]);
            }
            ASSERT_EQUAL(mine.size(), ref.size());
            for (size_t i = 0; i < ref.size(); ++i) ASSERT_EQUAL(mine[i], ref[i]);
            if (!ref.empty()) {
                ASSERT_EQUAL(mine.front(), ref.front());
                ASSERT_EQUAL(mine.back(), ref.back());
            }
        }
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestVectorBasic);
    RUN_TEST(tr, TestVectorCopy);
    RUN_TEST(tr, TestVectorAccess);
    RUN_TEST(tr, TestVectorErrors);
    RUN_TEST(tr, TestVectorGrow);
    RUN_TEST(tr, TestVectorReserveResize);
    RUN_TEST(tr, TestVectorInsertErase);
    RUN_TEST(tr, TestVectorSelfRef);
    RUN_TEST(tr, TestVectorAlgorithms);
    RUN_TEST(tr, TestDequeBasic);
    RUN_TEST(tr, TestDequeCopy);
    RUN_TEST(tr, TestDequeAccess);
    RUN_TEST(tr, TestDequeErrors);
    RUN_TEST(tr, TestDequeTwoEnds);
    RUN_TEST(tr, TestDequeAlternate);
    RUN_TEST(tr, TestDequeSelfRef);
    RUN_TEST(tr, TestTemplateTypes);
    RUN_TEST(tr, TestObjectLifetime);
    RUN_TEST(tr, TestVectorRandom);
    RUN_TEST(tr, TestDequeRandom);
    return 0;
}