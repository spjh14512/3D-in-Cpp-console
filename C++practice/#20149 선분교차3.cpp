#include <iostream>
using namespace std;
typedef long long ll;

inline ll MAX(ll x, ll y) {
	return x > y ? x : y;
}
inline ll MIN(ll x, ll y) {
	return x > y ? y : x;
}

class Point {
public:
	ll x, y;
public:
	Point(ll X, ll Y) : x(X), y(Y) {}
	Point operator+(Point other) {
		return Point(x + other.x, y + other.y);
	}
	Point operator-(Point other) {
		return Point(x - other.x, y - other.y);
	}
	bool operator==(Point other) {
		return x == other.x && y == other.y;
	}
};

class Line {
public:
	Point P, Q;
public:
	Line(Point p, Point q) : P(p), Q(q) {}
};

int CCW(Point p1, Point p2, Point p3) {
	p2 = p2 - p1;
	p3 = p3 - p1;

	ll det = p2.x * p3.y - p2.y * p3.x;

	if (det > 0) return 1;
	else if (det == 0) return 0;
	else return -1;
}

Point ans_point(0, 0);
/*	
	0 : 만나지 않음
	1 : 두 선분이 교차하고 한 점에서 만남
	2 : 네 점이 한 직선 위에 있고 한 점에서 만남
	3 : 무수히 많은 점에서 만남	
*/
int Cross(Line l1, Line l2) {
	if (CCW(l1.P, l2.P, l1.Q) == 0 && CCW(l1.P, l2.Q, l1.Q) == 0) {
		// 네 점이 일직선 위에 있는 경우
		bool flag = false;
		if (l1.P.x == l1.Q.x) {
			// Y축에 평행한 선분인 경우
			if ((l1.P.y >= MIN(l2.P.y, l2.Q.y) && l1.P.y <= MAX(l2.P.y, l2.Q.y)) ||
				(l1.Q.y >= MIN(l2.P.y, l2.Q.y) && l1.Q.y <= MAX(l2.P.y, l2.Q.y)) ||
				(l2.P.y >= MIN(l1.P.y, l1.Q.y) && l2.P.y <= MAX(l1.P.y, l1.Q.y)) ||
				(l2.Q.y >= MIN(l1.P.y, l1.Q.y) && l2.Q.y <= MAX(l1.P.y, l1.Q.y)))
			{
				if (l1.P == l2.P || l1.P == l2.Q) {
					flag = true;
					if (l1.Q.y >= MIN(l2.P.y, l2.Q.y) && l1.Q.y <= MAX(l2.P.y, l2.Q.y))
					{
						return 3;
					}
					ans_point = l1.P;
				}
				if (l1.Q == l2.P || l1.Q == l2.Q) {
					flag = true;
					if (l1.P.y >= MIN(l2.P.y, l2.Q.y) && l1.P.y <= MAX(l2.P.y, l2.Q.y))
					{
						return 3;
					}
					ans_point = l1.Q;
				}
				if (l2.P == l1.P || l2.P == l1.Q) {
					flag = true;
					if (l2.Q.y >= MIN(l1.P.y, l1.Q.y) && l2.Q.y <= MAX(l1.P.y, l1.Q.y))
					{
						return 3;
					}
					ans_point = l2.P;
				}
				if (l2.Q == l1.P || l2.Q == l1.Q) {
					flag = true;
					if (l2.P.y >= MIN(l1.P.y, l1.Q.y) && l2.P.y <= MAX(l1.P.y, l1.Q.y))
					{
						return 3;
					}
					ans_point = l2.Q;
				}
				if (flag) return 2;
				return 3;
			}
			return 0;
		}
		else if (l1.P.y == l1.Q.y) {
			// X축에 평행한 선분인 경우
			if ((l1.P.x >= MIN(l2.P.x, l2.Q.x) && l1.P.x <= MAX(l2.P.x, l2.Q.x)) ||
				(l1.Q.x >= MIN(l2.P.x, l2.Q.x) && l1.Q.x <= MAX(l2.P.x, l2.Q.x)) ||
				(l2.P.x >= MIN(l1.P.x, l1.Q.x) && l2.P.x <= MAX(l1.P.x, l1.Q.x)) ||
				(l2.Q.x >= MIN(l1.P.x, l1.Q.x) && l2.Q.x <= MAX(l1.P.x, l1.Q.x)))
			{
				if (l1.P == l2.P || l1.P == l2.Q) {
					flag = true;
					if (l1.Q.x >= MIN(l2.P.x, l2.Q.x) && l1.Q.x <= MAX(l2.P.x, l2.Q.x))
					{
						return 3;
					}
					ans_point = l1.P;
				}
				if (l1.Q == l2.P || l1.Q == l2.Q) {
					flag = true;
					if (l1.P.x >= MIN(l2.P.x, l2.Q.x) && l1.P.x <= MAX(l2.P.x, l2.Q.x))
					{
						return 3;
					}
					ans_point = l1.Q;
				}
				if (l2.P == l1.P || l2.P == l1.Q) {
					flag = true;
					if (l2.Q.x >= MIN(l1.P.x, l1.Q.x) && l2.Q.x <= MAX(l1.P.x, l1.Q.x))
					{
						return 3;
					}
					ans_point = l2.P;
				}
				if (l2.Q == l1.P || l2.Q == l1.Q) {
					flag = true;
					if (l2.P.x >= MIN(l1.P.x, l1.Q.x) && l2.P.x <= MAX(l1.P.x, l1.Q.x))
					{
						return 3;
					}
					ans_point = l2.Q;
				}
				if (flag) return 2;
				return 3;
			}
			return 0;
		}
		else {
			// 우상향 또는 우하향
			if ((l1.P.x >= MIN(l2.P.x, l2.Q.x) && l1.P.x <= MAX(l2.P.x, l2.Q.x)) ||
				(l1.Q.x >= MIN(l2.P.x, l2.Q.x) && l1.Q.x <= MAX(l2.P.x, l2.Q.x)) ||
				(l2.P.x >= MIN(l1.P.x, l1.Q.x) && l2.P.x <= MAX(l1.P.x, l1.Q.x)) ||
				(l2.Q.x >= MIN(l1.P.x, l1.Q.x) && l2.Q.x <= MAX(l1.P.x, l1.Q.x)))
			{
				if (l1.P == l2.P || l1.P == l2.Q) {
					flag = true;
					if (l1.Q.y >= MIN(l2.P.y, l2.Q.y) && l1.Q.y <= MAX(l2.P.y, l2.Q.y))
					{
						return 3;
					}
					ans_point = l1.P;
				}
				if (l1.Q == l2.P || l1.Q == l2.Q) {
					flag = true;
					if (l1.P.y >= MIN(l2.P.y, l2.Q.y) && l1.P.y <= MAX(l2.P.y, l2.Q.y))
					{
						return 3;
					}
					ans_point = l1.Q;
				}
				if (l2.P == l1.P || l2.P == l1.Q) {
					flag = true;
					if (l2.Q.y >= MIN(l1.P.y, l1.Q.y) && l2.Q.y <= MAX(l1.P.y, l1.Q.y))
					{
						return 3;
					}
					ans_point = l2.P;
				}
				if (l2.Q == l1.P || l2.Q == l1.Q) {
					flag = true;
					if (l2.P.y >= MIN(l1.P.y, l1.Q.y) && l2.P.y <= MAX(l1.P.y, l1.Q.y))
					{
						return 3;
					}
					ans_point = l2.Q;
				}
				if (flag) return 2;
				return 3;
			}
			return 0;
		}
	}
	else {
		// 네 점이 일직선 위에 있지 않은 경우
		if (CCW(l1.P, l2.P, l1.Q) * CCW(l1.P, l2.Q, l1.Q) > 0) return 0;
		if (CCW(l2.P, l1.P, l2.Q) * CCW(l2.P, l1.Q, l2.Q) > 0) return 0;
		return 1;
	}
}

int main() {
	ll x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;
	Line L1(Point(x1, y1), Point(x2, y2));
	cin >> x1 >> y1 >> x2 >> y2;
	Line L2(Point(x1, y1), Point(x2, y2));

	switch (Cross(L1, L2)) {
	case 0:
		cout << 0;
		break;
	case 1:
		cout << 1 << '\n';
		cout.precision(16);
		cout << fixed;
		if (L1.P.x == L1.Q.x) {
			// L1이 Y축에 평행한 경우
			double a2 = (double)(L2.Q.y - L2.P.y) / (double)(L2.Q.x - L2.P.x);
			cout << L1.P.x << ' ' << a2 * (L1.P.x - L2.P.x) + L2.P.y;
		}
		else if (L2.P.x == L2.Q.x) {
			// L2가 Y축에 평행한 경우
			double a1 = (double)(L1.Q.y - L1.P.y) / (double)(L1.Q.x - L1.P.x);
			cout << L2.P.x << ' ' << a1 * (L1.P.x - L1.P.x) + L1.P.y;
		}
		else if (L1.P.y == L1.Q.y) {
			// L1이 X축에 평행한 경우
			double a2 = (double)(L2.Q.y - L2.P.y) / (double)(L2.Q.x - L2.P.x);
			cout << (double)(L1.P.y - L2.P.y) / a2 + L2.P.x << ' ' << L1.P.y;
		}
		else if (L2.P.y == L2.Q.y) {
			// L2가 X축에 평행한 경우
			double a1 = (double)(L1.Q.y - L1.P.y) / (double)(L1.Q.x - L1.P.x);
			cout << (double)(L2.P.y - L1.P.y) / a1 + L1.P.x << ' ' << L2.P.y;
		}
		else {
			// 두 선분 모두 축에 평행하지 않은 경우
			double a1 = (double)(L1.Q.y - L1.P.y) / (double)(L1.Q.x - L1.P.x);
			double a2 = (double)(L2.Q.y - L2.P.y) / (double)(L2.Q.x - L2.P.x);
			double ans_X;
			ans_X = (double)(a1 * L1.P.x - a2 * L2.P.x + L2.P.y - L1.P.y) / (a1 - a2);
			cout << ans_X << ' ' << a1 * (ans_X - L1.P.x) + L1.P.y;
		}
		break;
	case 2:
		cout << 1 << '\n';
		cout << ans_point.x << ' ' << ans_point.y;
		break;
	case 3:
		cout << 1;
		break;
	}
}