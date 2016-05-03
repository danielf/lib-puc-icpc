const double EPS = 1e-8;

// start: 18e009c8146f4120c0e91f83dbaaaa51  -
int cmp(double x, double y = 0, double tol = EPS) {
  return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
// end

// start: 5eb87a319bddd701d220242f1cac72a5  -
struct point {
	double x, y;
	explicit point(double x = 0, double y = 0): x(x), y(y) {}

	// Only type in the operators you actually need
// start: 86951a41c91abb616a92e381d0fa4f86  -
	point operator +(point q) const { return point(x + q.x, y + q.y); }
	point operator -(point q) const { return point(x - q.x, y - q.y); }
	point operator *(double t) const { return point(x * t, y * t); }
	point operator /(double t) const { return point(x / t, y / t); }
	// dot product
	double operator *(point q) const { return x * q.x + y * q.y; }
	// "cross" product (actually "signed area of the quadrilateral"
	double operator %(point q) const { return x * q.y - y * q.x; }
// end
	// uses "cmp" from the template
	int cmp(point q) const {
		if (int t = ::cmp(x, q.x)) return t;
		return ::cmp(y, q.y);
	}
	bool operator ==(point q) const { return cmp(q) == 0; }
	bool operator !=(point q) const { return cmp(q) != 0; }
	bool operator < (point q) const { return cmp(q) <  0; }

	friend ostream& operator <<(ostream& o, point p) {
		return o << "(" << p.x << ", " << p.y << ")";
	}

	static point pivot; // only needed for convex hull
};
// end

point point::pivot; // only needed for convex hull

// start: d3025b054e19517a014c9884869d805a  -
double norm(point p){ return sqrt(p * p); }
double arg(point p) { return atan2(p.y, p.x); }
// end

typedef vector<point> polygon;

// decides type of turn pqr. 1 if left, -1 if right, 0 if straight
// start: ecee36c02340dbba3e88ed32123335fc  -
inline int ccw(point p, point q, point r) {
	return cmp((p - r) % (q - r));
}
// end

// angle of turn pqr
// start: 9611e3ebdd4800569cdbad30109fff6c  -
inline double angle(point p, point q, point r) {
	point u = p - q, v = r - q;
	return atan2(u % v, u * v);
}
// end

////////////////////////////////////////////////////////////////////////////////
// Is q inside [p,r]? (all segments are closed)
//

// start: fc8ee0aff0d6de74607e95169a31cc7c  -
bool between(point p, point q, point r) {
	return ccw(p, q, r) == 0 && cmp((p - q) * (r - q)) <= 0;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Do segments [p,q] and [r,s] have an intersection?
//

// start: f4f340136d8a306387ae55ab610c8d3b  -
bool seg_intersect(point p, point q, point r, point s) {
	point A = q - p, B = s - r, C = r - p, D = s - q;
	int a = cmp(A % C) + 2 * cmp(A % D);
	int b = cmp(B % C) + 2 * cmp(B % D);
	if (a == 3 || a == -3 || b == 3 || b == -3) return false;
	if (a || b || p == r || p == s || q == r || q == s) return true;
	int t = (p < r) + (p < s) + (q < r) + (q < s);
	return t != 0 && t != 4;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Distance from r to [p,q]
//

// start: ba743bfad8dc2b93f3279ca3c4f81c36  -
double seg_distance(point p, point q, point r) {
	point A = r - q, B = r - p, C = q - p;
	double a = A * A, b = B * B, c = C * C;
	if (cmp(b, a + c) >= 0) return sqrt(a);
	else if (cmp(a, b + c) >= 0) return sqrt(b);
	else return fabs(A % B) / sqrt(c);
}
// end

////////////////////////////////////////////////////////////////////////////////
// Classifies p with respect of polygon T (not necessarily convex)
//
// Returns -1 if on border, 0 if outside, 1 if inside
//

// start: 6c0f6be91a4c1ae192a1e0c14ef8b4cd  -
int in_poly(point p, polygon& T) {
	double a = 0; int N = T.size();
	for (int i = 0; i < N; i++) {
		if (between(T[i], p, T[(i+1) % N])) return -1;
		a += angle(T[i], p, T[(i+1) % N]);
	}
	return cmp(a) != 0;
}
// end
////////////////////////////////////////////////////////////////////////////////
// Convex Hull! O(nlogn), *alters the input T*
//

// start: 0ba3640bb5232244bc3107fd3fd6642f  -
bool radial_lt(point p, point q) {
	point P = p - point::pivot, Q = q - point::pivot;
	double R = P % Q;
	if (cmp(R)) return R > 0;
	return cmp(P * P, Q * Q) < 0;
}
// end

// start: 19eefce7fd0da19cd5bbeda9c8a27434  -
polygon convex_hull(vector<point>& T) {
	int j = 0, k, n = T.size(); polygon U(n);

	point::pivot = *min_element(all(T));
	sort(all(T), radial_lt);
	// If colinear points are part of hull, insert next two lines
	//for (k = n-2; k >= 0 && ccw(T[0], T[n-1], T[k]) == 0; k--);
	//reverse((k+1) + all(T));

	for (int i = 0; i < n; i++) {
		// If colinear points are part, change >= to >
		while (j > 1 && ccw(U[j-1], U[j-2], T[i]) >= 0) j--;
		U[j++] = T[i];
	}
	U.erase(j + all(U));
	return U;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Computes the (oriented) area of T. Positive if counterclockwise
//
// start: 37d19b20a94a6b9a6533390401df9180  -
double poly_area(const polygon& T) {
	double s = 0; int n = T.size();
	for (int i = 0; i < n; i++)
		s += T[i] % T[(i+1) % n];
	return s / 2;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Finds intersection between lines (p,q) and (r,s)
// (Warning: Divides by zero if parallel! Will return NaN or INF in this case)

// start: 93b5d67b7a884a1b2c740dc7433a7e68  -
point line_intersect(point p, point q, point r, point s) {
	point a = q - p, b = s - r, c = point(p % q, r % s);
	return point(point(a.x, b.x) % c, point(a.y, b.y) % c) / (a % b);
}
// end

////////////////////////////////////////////////////////////////////////////////
// Rectangle intersection

// start: 9a397eefeac94189d6e74b2d3ad497f8  -
struct rect{
    double ax, ay, bx, by; // (ax, ay) lower left, (bx, by) top right
    explicit rect(double ax = 0, double ay = 0,
         double bx = 0, double by = 0): ax(ax), ay(ay),
                                        bx(bx), by(by) {}
};
// end

// Returns the intersection of r and s. (0, 0, 0, 0) if area zero.
// start: 3f02dbe7ed1b001d72a628b6a5a5ee50  -
rect inter(rect r, rect s){
    rect ans(max(s.ax, r.ax), max(s.ay, r.ay), min(s.bx, r.bx), min(s.by, r.by));
    if (cmp(ans.xa, ans.xb) > 0 || cmp(ans.ya, ans.yb) > 0)
      return rect(INFINITY);
    return ans;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Line-circle and circle-circle intersections

// Returns the foot of the perpendicular from P to line AB
// start: 380f956eb4ee49b2ab164bcaa333af1e  -
point foot(point P, point A, point B) {
    point dir = B-A;
    return (dir*((P-A)*dir))/(dir*dir) + A;
}
// end

typedef pair<point, point> ppt;

// Returns the two points R1, R2 at distance d from q
// such that PQR1 = PQR2 = 90. Assumes p != q.
// start: e99967d99c166b27cb008d60b1c0dbf1  -
ppt disppt(point P, point Q, double d){
    point dir = P - Q;
    point r = point(dir.y, -dir.x);
    point k = r * (d/norm(r));
    return ppt{Q + k, Q - k};
}
// end

// Intersection of two circles.
// If circles are tangent, second point inf true. If disjoint both inf true.
// start: d96b52879809fe722958e28c2d3854a9  -
ppt circint(point o1, double r1, point o2, double r2){
    point dir = o2 - o1;
    point in(INFINITY, INFINITY);
    double d = norm(dir);
    // tangent
    if(cmp(r1 + r2, d) == 0 || cmp(d + r2, r1) == 0 || cmp(d + r1, r2) == 0)
        return ppt{o1 + dir * r1/(r1 + r2), in};
    // no inter
    if(cmp(r1 + r2, d) == -1 || cmp(d + r2, r1) == -1 || cmp(d + r1, r2) == -1)
        return ppt{in, in};
    // two inters
    double x = (d*d - r2*r2 + r1*r1)/(2*d);
    return disppt(o1, o1 + dir * x/d, sqrt(r1*r1 - x*x));
}
// end

// Intersect line and circle.
// If  tangent, second point inf true. If disjoint both inf true.
// start: 0481f79a0ad23651515291bda03ee30f  -
ppt cline(point o, double r, point a, point b){
    point h = foot(o, a, b);
    point in(INFINITY, INFINITY);
    double d = norm(h - o);
    if(cmp(d, r) == 0) return ppt{h, in};
    if(cmp(d, r) == 1) return ppt{in, in};
    return disppt(o, h, sqrt(r*r - d*d));
}
// end

// Inverse of p in circle. Assumes o != p.
// start: 954cd892ee2eafb05877d8713cb637e1  -
point cinv(point o, double r, point p){
    point p0 = p - o;
    return o + p0 * (r*r/(p0 * p0));
}
// end

////////////////////////////////////////////////////////////////////////////////
// Find the smallest circle containing all the points in expected time O(N)
//

typedef pair<point, double> circle; // center, radius

// start: 03277515521375890c85075639ad1631  -
bool in_circle(circle C, point p) { // is point p in circle C?
	return cmp((p - C.first)*(p - C.first), C.second*C.second) <= 0;
}
// end

// Given three (not colinear) points, finds center of circle containing them
// start: 3028444d6729168187b966e9c984bab3  -
point circumcenter(point p, point q, point r) {
	point a = p - r, b = q - r, c = point(a * (p + r) / 2, b * (q + r) / 2);
	return point(c % point(a.y, b.y), point(a.x, b.x) % c) / (a % b);
}
// end

// start: 37087a20364e8a888b128348f9524d90  -
circle spanning_circle(vector<point>& T) {
	int n = T.size();
	random_shuffle(all(T)); // Extremely important!
	circle C(point(), -INFINITY);
	for (int i = 0; i < n; i++) if (!in_circle(C, T[i])) {
		C = circle(T[i], 0);
		for (int j = 0; j < i; j++) if (!in_circle(C, T[j])) {
			C = circle((T[i] + T[j]) / 2, abs(T[i] - T[j]) / 2);
			for (int k = 0; k < j; k++) if (!in_circle(C, T[k])) {
				point o = circumcenter(T[i], T[j], T[k]);
				C = circle(o, abs(o - T[k]));
			}
		}
	}
	return C;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Find the intersection of two CONVEX polygons in LINEAR TIME
//
// P and Q should be in counterclockwise order (their areas should be positive)

// start: 6de0b0d721ecf8df485bbfcf37a29f84  -
polygon poly_intersect(polygon& P, polygon& Q) {
	int m = Q.size(), n = P.size();
	if (m == 0 || n == 0) return polygon();
	int a = 0, b = 0, aa = 0, ba = 0, inflag = 0;
	polygon R;
// 		start: d79769b383c55421195ea286f25dd01b  -
	while ((aa < n || ba < m) && aa < 2*n && ba < 2*m) {
		point p1 = P[a], p2 = P[(a+1) % n], q1 = Q[b], q2 = Q[(b+1) % m];
		point A = p2 - p1, B = q2 - q1;
		int cross = cmp(A % B), ha = ccw(p2, q2, p1), hb = ccw(q2, p2, q1);
// 				start: 9d1ce61202c43c81711c0070f0ba761a  -
		if (cross == 0 && ccw(p1, q1, p2) == 0 && cmp(A * B) < 0) {
			if (between(p1, q1, p2)) R.push_back(q1);
			if (between(p1, q2, p2)) R.push_back(q2);
			if (between(q1, p1, q2)) R.push_back(p1);
			if (between(q1, p2, q2)) R.push_back(p2);
			if (R.size() < 2) return polygon();
			inflag = 1; break;
		} else if (cross != 0 && seg_intersect(p1, p2, q1, q2)) {
			if (inflag == 0) aa = ba = 0;
			R.push_back(line_intersect(p1, p2, q1, q2));
			inflag = (hb > 0) ? 1 : -1;
		}
// 				end
		if (cross == 0 && hb < 0 && ha < 0) return R;
		bool t = cross == 0 && hb == 0 && ha == 0;
		if (t ? (inflag == 1) : (cross >= 0) ? (ha <= 0) : (hb > 0)) {
			if (inflag == -1) R.push_back(q2);
			ba++; b++; b %= m;
		} else {
			if (inflag == 1) R.push_back(p2);
			aa++; a++; a %= n;
		}
	}
// 		end
	if (inflag == 0) {
		if (in_poly(P[0], Q)) return P;
		if (in_poly(Q[0], P)) return Q;
	}
	R.erase(unique(all(R)), R.end());
	if (R.size() > 1 && R.front() == R.back()) R.pop_back();
	return R;
}
// end
