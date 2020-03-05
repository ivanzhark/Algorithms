#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip>

//Нахождение выпуклой оболочки на плоскости
//Алгоритм Джарвиса
double const epsilon = 1.0e-6;

bool equal(double x, double y) { return fabs(x - y) < epsilon; };

struct point2d {
	point2d(double x_, double y_) : x(x_), y(y_) {};
	inline double norm2() const { return sqrt(x * x + y * y); };
	inline point2d operator-(const point2d& w) { return point2d(this->x - w.x, this->y - w.y); };
	double x;
	double y;
};

enum class sign {neg, zero, pos};

sign Orientation(point2d u, point2d v, point2d w)
{
	double orient = (v.x - u.x)*(w.y - v.y) - (v.y - u.y)*(w.x - v.x);
	if (equal(orient, 0)) return sign::zero;
	if (orient>0) return sign::pos;
	return sign::neg;
}

inline double distance(const point2d& v1, const point2d& v2) {
	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

int main() {
	std::ifstream file("input.txt");

	int N;
	file >> N;
	std::vector<point2d> points;
	for (int i = 0; i < N; i++) {
		double coord_x, coord_y;
		file >> coord_x >> coord_y;
		points.push_back(point2d(coord_x, coord_y));
	}
	//находим самую нижнюю левую точку
	auto u0 = points[0];
	int init = 0;
	for (int i = 1; i < points.size(); i++) {
		auto v = points[i];
		if ((v.y < u0.y) || ((equal(v.y, u0.y)) && (v.x < u0.x))) {
			u0 = points[i]; init = i;
		}
	}

	double perimeter = 0.;
	std::vector<int> convex_hull;
	convex_hull.emplace_back(init);
	int cur = init;
	int next;

	do {
		next = 0;
		for (int i = 0; i < N; i++)
		{
			if (Orientation(points[cur], points[next], points[i]) == sign::neg){
				next = i;
			}
			else {
				if (Orientation(points[cur], points[next], points[i]) == sign::zero)
					if ( (points[cur]-points[i]).norm2() > (points[cur] - points[next]).norm2())
						next = i;
			}
		}
		convex_hull.emplace_back(next);
		perimeter += distance(points[cur], points[next]);
		cur = next;
	} while (init != next);

	std::ofstream out_file("output.txt");
	out_file << std::setprecision(11) << perimeter;
	file.close();
	out_file.close();
}