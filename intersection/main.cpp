#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

/*
 * This Geom so we must use eps to be sure that we count accurately with some
 * precision
 */
const double eps = 1e-10;

/*
 * include to not write them often
 *
 */
using std::cin;
using std::cout;
using std::endl;

/*
 * RadiusVector with to double coordinates
 * Is equal to vector that was put from zero, zero
 */
struct RadiusVector {
    double x, y;

    /*
     * Initilizing with nulls
     */
    RadiusVector() : x(eps), y(eps) {}
    /*
     * Initilizing with given values
     */
    RadiusVector(double _x, double _y) : x(_x), y(_y) {}
    /*
     * Initlize with given point (Structure RadiusVector)
     */
    RadiusVector(const RadiusVector& point);

    /*
     * Scalar Multiply with point
     */
    double scalarMult(const RadiusVector& point) const;
    /*
     * length of the vector that was maken from this coordinates
     */
    double length() const;

    /*
     * Check if point lays on the segment
     */
    bool isLayOnSegment(const RadiusVector& pointA, const RadiusVector& pointB) const;

    /*
     * Vectot multiply in 2d dimonsion
     */
    double operator * (const RadiusVector& point) const;
    /*
     * Add coordinates from one point to another
     */
    RadiusVector operator + (const RadiusVector& point) const;
    /*
     * Minus coordiantes of some point to another point
     */
    RadiusVector operator - (const RadiusVector& point) const;

};

double RadiusVector::operator*(const RadiusVector &point) const {
    return x * point.y - y * point.x;
}

RadiusVector::RadiusVector(const RadiusVector &point) {
    x = point.x;
    y = point.y;
}

RadiusVector RadiusVector::operator-(const RadiusVector &point) const {
    return RadiusVector(x - point.x, y - point.y);
}

RadiusVector RadiusVector::operator+(const RadiusVector &point) const {
    return RadiusVector(x + point.x, y + point.y);
}

double RadiusVector::scalarMult(const RadiusVector &point) const {
    return x * point.x + y * point.y;
}

double RadiusVector::length() const {
    return sqrt(x * x + y * y);
}

bool RadiusVector::isLayOnSegment(const RadiusVector &pointA, const RadiusVector &pointB) const {
    RadiusVector VectorA = *this - pointA;
    RadiusVector VectorB = *this - pointB;
    return fabs(VectorA.scalarMult(VectorB) + VectorA.length() * VectorB.length()) <= eps;
}

/*
 * Convex Poligon that helps to solve problem
 */

class Poligon {
public:

    /*
     * Intilize poligon with points, that are not be used in hte future
     */
    void setPoints(std::vector<RadiusVector> &&points);

    /*
     * reverse Poligon from zero, zero
     */
    void reverse();

    /*
     * Make the first point in poligon be the lowest with Y-coordinte and X
     */
    void moveMinAsFirst();

    /*
     * Check if two poligons intersects
     */
    bool isIntersect(const Poligon &poligon) const;

    /*
     * Check if point is in the poligon
     */
    bool isPointIn(const RadiusVector &point) const;

private:
    /*
     * Points of the poligon
     */
    std::vector<RadiusVector> points;

    /*
     * Cont Minkovski sum - POligon
     */
    Poligon findMinkovskiiSum(const Poligon &poligon) const;
};

void Poligon::setPoints(std::vector<RadiusVector> &&points) {
    this->points = points;
}

void Poligon::reverse() {

    for (RadiusVector& point: points) {
        point.x = - point.x;
        point.y = - point.y;
    }
}

bool Poligon::isIntersect(const Poligon &poligon) const {
    Poligon MinkovskiiSum = findMinkovskiiSum(poligon);

    RadiusVector nulPoint = RadiusVector(eps, eps);
    return MinkovskiiSum.isPointIn(nulPoint);
}

Poligon Poligon::findMinkovskiiSum(const Poligon &poligon) const {
    Poligon polig = Poligon();

    std::vector<RadiusVector> newPoints;

    size_t curI, curJ;
    for (size_t i = 0, j = 0; i < points.size() ||  j < poligon.points.size(); ) {
        curI = i % points.size();
        curJ = j % poligon.points.size();

        newPoints.push_back(points[curI] + poligon.points[curJ]);

        double Angle = ((points[(curI + 1) % points.size()] - points[curI]) *
                        (poligon.points[(curJ + 1) % poligon.points.size()] - poligon.points[curJ]));

        if (Angle > eps) {
            if (i != points.size()) {
                i++;
            } else {
                j++;
            }
        } else if (Angle < -eps) {
            if (j != poligon.points.size()) {
                j++;
            } else {
                i++;
            }
        } else {
            i++, j++;
        }

    }

    polig.setPoints(std::move(newPoints));

    return polig;
}

bool Poligon::isPointIn(const RadiusVector &point) const {

    double VectMult = (point - points[0]) * (points[1] - points[0]);
    bool isInside = true;

    if (fabs(VectMult) < eps) {
        isInside = false;
    }

    for (int i = 0; i < points.size(); ++i) {

        if (VectMult * ((point - points[i]) * (points[(i + 1) % points.size()] - points[i])) < -eps  ||
            fabs(((point - points[i]) * (points[(i + 1) % points.size()] - points[i]))) < eps) {
            isInside = false;
        }
    }

    bool isAtBorder = false;

    for (int i = 0; i < points.size(); ++i) {
        if (point.isLayOnSegment(points[i], points[(i + 1) % points.size()])) {
            isAtBorder = true;
        }
    }

    return isInside || isAtBorder;
}

void Poligon::moveMinAsFirst() {
    if ((points[1] - points[0]) * (points[2] - points[1]) < -eps) {
        std::reverse(points.begin(), points.end());
    }

    size_t curI = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].y < points[curI].y) {
            curI = i;
        } else if (fabs(points[i].y - points[curI].y) <= eps && points[i].x < points[curI].x) {
            curI = i;
        }
    }

    std::vector<RadiusVector> temp(points.size());

    for (size_t i = 0; i < points.size(); i++) {
        size_t tempI = (curI + i) % points.size();
        temp[i] = points[tempI];
    }
    points = temp;
}


int main() {
    size_t n;

    cin >> n;

    Poligon firstPoligon = Poligon();
    std::vector<RadiusVector> points(n);

    for (size_t i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }

    firstPoligon.setPoints(std::move(points));
    firstPoligon.moveMinAsFirst();

    cin >> n;
    Poligon secondPoligon = Poligon();
    std::vector<RadiusVector> point(n);

    for (int i = 0; i < n; i++) {
        cin >> point[i].x >> point[i].y;
    }

    secondPoligon.setPoints(std::move(point));
    secondPoligon.reverse();

    secondPoligon.moveMinAsFirst();

    if (firstPoligon.isIntersect(secondPoligon)) {
        cout << "YES";
    } else {
        cout << "NO";
    }
}
