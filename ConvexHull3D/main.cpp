#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <cmath>
#include <queue>
#include <map>
#include <algorithm>
#include <fstream>

/*
 * Frequently used
 */
using std::cin;
using std::cout;
using std::endl;

/*
 * It is geom that's why we use epsilon :)
 */
const double eps = 1e-12;

/*
 * RadiusVector with temlate coordiantes
 */

template <typename T>
struct RadiusVector {
    /*
     * Coordinates of poit
     */
    T x, y, z;

    RadiusVector(T _x, T _y, T _z):x(_x), y(_y), z(_z) {}
    RadiusVector() = default;
    RadiusVector<T> operator * (const RadiusVector<T>& point);
    RadiusVector<T> operator -();
    RadiusVector<T> operator -(const RadiusVector<T>& point);
    RadiusVector<T> operator +(const RadiusVector<T>& point);
    RadiusVector<T> operator *(double alpha);
    /*
     * Scalar multiply of two points (as from vectors)
     */
    double scalarMult(const RadiusVector<T>& point);

    /*
     * Distance from point (zero, zero)
     */
    double length();

};

template <typename T>
double RadiusVector<T>::length() {
    return sqrt(static_cast<double >(x * x + y *y + z * z));
}

template <typename T>
RadiusVector<T> RadiusVector<T>::operator*(const RadiusVector &point) {
    return RadiusVector((y * point.z - z * point.y),
                 - (x * point.z - z * point.x),
                 (x * point.y - y * point.x));
}

template <typename T>
double RadiusVector<T>::scalarMult(const RadiusVector &point) {
    return x * point.x + y * point.y + z * point.z;
}

template <typename T>
RadiusVector<T> RadiusVector<T>::operator-() {
    return RadiusVector(-x, -y, -z);
}

template <typename T>
RadiusVector<T> RadiusVector<T>::operator-(const RadiusVector<T> &point) {
    return RadiusVector<T>(x - point.x, y - point.y, z - point.z);
}

template <typename T>
RadiusVector<T> RadiusVector<T>::operator+(const RadiusVector<T> &point) {
    return RadiusVector<T>(x + point.x, y + point.y, z + point.z);
}

template <typename T>
RadiusVector<T> RadiusVector<T>::operator*(double alpha) {
    return RadiusVector<T>(x * alpha, y * alpha, z * alpha);
}

/*
 * Flat of the convex hull
 */
template <typename T>
struct Flat {
    /*
     * Three points that creates thi flat
     */
    size_t firstPoint, secondPoint, thirdPoint;
    /*
     * Normal to the flat, that looks in
     */
    RadiusVector<T> Normal;
    /*
     * just to sort them to give answer in this format
     */
    bool operator < (Flat<T>& flat);

    /*
     * returns tuple from three points, to easy use
     */

    std::tuple<size_t, size_t, size_t> createTuple() const;
};

template <typename T>
bool Flat<T>::operator < (Flat<T> &flat) {
    std::tuple<size_t, size_t, size_t> firstTup = createTuple();
    size_t first = std::get<0>(firstTup) * 1000000 + std::get<1>(firstTup) * 1000 + std::get<2>(firstTup); // Max of each member is 1000
    std::tuple<size_t, size_t, size_t> secondTup = flat.createTuple();
    size_t second = std::get<0>(secondTup) * 1000000 + std::get<1>(secondTup) * 1000 + std::get<2>(secondTup);// Max of each member is 1000
    return first < second;
}

template <typename T>
std::tuple<size_t, size_t, size_t> Flat<T>::createTuple() const {
    return std::make_tuple(firstPoint, secondPoint, thirdPoint);
}

size_t FindLess(std::tuple<size_t, size_t, size_t> &tuple);

size_t FindMax(std::tuple<size_t, size_t, size_t> &tuple);

size_t FindMiddle(std::tuple<size_t, size_t, size_t> &tuple);

std::tuple<size_t, size_t, size_t> sortTuple(std::tuple<size_t, size_t, size_t> &&Tuple) {
    return std::make_tuple(FindLess(Tuple), FindMax(Tuple), FindMiddle(Tuple));
}

size_t FindMiddle(std::tuple<size_t, size_t, size_t> &tuple) {
    size_t first = std::get<0>(tuple);
    size_t second = std::get<1>(tuple);
    size_t third = std::get<2>(tuple);
    size_t sum = first + second + third;

    return sum - FindMax(tuple) - FindLess(tuple);
}


size_t FindMax(std::tuple<size_t, size_t, size_t> &tuple) {
    size_t first = std::get<0>(tuple);
    size_t second = std::get<1>(tuple);
    size_t third = std::get<2>(tuple);
    if (first > second && first > third) {
        return first;
    }
    if (second > first && second > third) {
        return second;
    }

    return third;
}

size_t FindLess(std::tuple<size_t, size_t, size_t> &tuple) {
    size_t first = std::get<0>(tuple);
    size_t second = std::get<1>(tuple);
    size_t third = std::get<2>(tuple);
    if (first < second && first < third) {
        return first;
    }
    if (second < first && second < third) {
        return second;
    }

    return third;
}

/*
 * Edge of convex hull 3d with indexes of two points that structs this edge
 */
template <typename T>
struct Edge {
    /*
     * Two points that structs edge
     */
    size_t a, b;
    /*
     * Normal that looks in
     */
    RadiusVector<T> normal;
    /*
     * RadiusVector that mustn't be used while searching another point
     */
    size_t c;
};

/*
 * Class that structs ConvexHull and gives answer in approtiate way (format)
 */

template <typename T>
struct ConvexHullFinder {
    /*
     * Find Convex Hull
     */
    std::vector<Flat<T>> find(std::vector<RadiusVector<T>> &points);

    /*
     * Find the third poit to the first Flat
     */
    size_t findThirdPoint(std::vector<RadiusVector<T>> &points, size_t firstPoint, size_t secondPoint);

    /*
     * Find first flat of the convex hull
     */
    Flat<T> FindFirstFlat(std::vector<RadiusVector<T>> &points);

    /*
     * Find second, third, ... flates of the convex Hull
     */
    std::vector<Flat<T>> FindOtherFlatesUsingFirstOne(Flat<T> &firstFlat, std::vector<RadiusVector<T>> &points);

    /*
     * Find Second RadiusVector of the first flat of the convexhull
     */
    size_t FindSecondPoint(Edge<T> &edge, std::vector<RadiusVector<T>> &points);

    /*
     * Returns sorted Flates
     */
    Flat<T> sortedFlat(Flat<T> &flat, std::vector<RadiusVector<T>> &points);

    /*
     * Sort Flates inside and outside (means points in the flat, and sort flates as flates)
     */
    void SortFlates(std::vector<Flat<T>> &flates, std::vector<RadiusVector<T>> &points);

    /*
     * Sort all flates points by polar angle
     */
    void sortAllInsideFlates(std::vector<Flat<T>> &flates, std::vector<RadiusVector<T>> &points);
};

template <typename T>
std::vector<Flat<T>> ConvexHullFinder<T>::find(std::vector<RadiusVector<T>> &points) {
    std::vector<Flat<T>> flates;

    Flat<T> firstFlat = FindFirstFlat(points);

    flates = FindOtherFlatesUsingFirstOne(firstFlat, points);

    SortFlates(flates, points);

    return flates;
}

template <typename T>
size_t ConvexHullFinder<T>::findThirdPoint(std::vector<RadiusVector<T>> &points, size_t firstPoint, size_t secondPoint) {
    for (size_t i = 0; i < points.size(); ++i) {
        RadiusVector<T> a = points[secondPoint] - points[firstPoint];
        RadiusVector<T> b = points[secondPoint] - points[i];
        RadiusVector<T> normal = a * b;

        bool CanBeThirdPoint = true;
        if (i != firstPoint && i != secondPoint) {
            double wherePointsAre = 0;
            for (size_t j = 0; j < points.size(); j++) {
                if (j != firstPoint && j != secondPoint && j != i) {
                    if (wherePointsAre == 0) {
                        wherePointsAre = normal.scalarMult(points[firstPoint] - points[j]);
                    } else if (wherePointsAre * normal.scalarMult(points[firstPoint] - points[j]) < 0) {
                        CanBeThirdPoint = false;
                    }
                }
            }
            if (CanBeThirdPoint) {
                return i;
            }
        }
    }
}

template <typename T>
Flat<T> ConvexHullFinder<T>::FindFirstFlat(std::vector<RadiusVector<T>> &points) {
    size_t MinZPoint = 0;
    for (size_t i = 1; i < points.size(); ++i) {
        if (points[i].z < points[MinZPoint].z) {
            MinZPoint = i;
        }
    }

    size_t MaxAnglePoint = (MinZPoint == 0) ? 1 : 0;

    for (size_t i = 0; i < points.size(); ++i) {
        if (i != MaxAnglePoint) {
            double FirstAngle = points[MinZPoint].getAngleFromVertic(points[MaxAnglePoint] - points[MinZPoint]);
            double SecondAngle = points[MinZPoint].getAngleFromVertic(points[i] - points[MinZPoint]);
            if (FirstAngle > SecondAngle) {
                MaxAnglePoint = i;
            } else if (FirstAngle == SecondAngle &&
                       (points[MinZPoint] - points[MaxAnglePoint]).length() < (points[i] - points[MinZPoint]).length()) {
                MaxAnglePoint = i;
            }
        }
    }

    size_t PointOtherPointOnOneSide = findThirdPoint(points, MaxAnglePoint, MinZPoint);
    return Flat<T>{MinZPoint, MaxAnglePoint, PointOtherPointOnOneSide, RadiusVector<T>()};
}

template <typename T>
std::vector<Flat<T>> ConvexHullFinder<T>::FindOtherFlatesUsingFirstOne(Flat<T> &firstFlat, std::vector<RadiusVector<T>> &points) {

    std::vector<Flat<T>> flates;
    RadiusVector<T> Normal = (points[firstFlat.firstPoint] - points[firstFlat.secondPoint]) *
                      (points[firstFlat.thirdPoint] - points[firstFlat.secondPoint]);

    if (Normal.z < 0) {
        Normal = -Normal;
    }

    firstFlat.Normal = Normal;
    flates.push_back(firstFlat);

    std::queue<Edge<T>> edges;

    edges.push({firstFlat.firstPoint, firstFlat.secondPoint, Normal, firstFlat.thirdPoint});
    edges.push({firstFlat.secondPoint, firstFlat.thirdPoint, Normal, firstFlat.firstPoint});
    edges.push({firstFlat.firstPoint, firstFlat.thirdPoint, Normal, firstFlat.secondPoint});

    std::map<std::tuple<size_t, size_t, size_t>, bool> used;
    used[sortTuple(std::make_tuple(firstFlat.firstPoint, firstFlat.secondPoint, firstFlat.thirdPoint))] = true;

    std::map<std::pair<size_t, size_t>, bool> usedEdge;

    while (!edges.empty()) {
        Edge<T> edge = edges.front();
        edges.pop();
        if (usedEdge.find(std::make_pair(std::min(edge.a, edge.b), edge.a + edge.b - std::min(edge.a, edge.b))) != usedEdge.end()) {
            continue;
        }
        usedEdge[std::make_pair(std::min(edge.a, edge.b), edge.a + edge.b - std::min(edge.a, edge.b))] = true;
        size_t secondPoint = FindSecondPoint(edge, points);
        Normal = (points[edge.a] - points[edge.b]) * (points[secondPoint] - points[edge.b]);

        if (Normal.scalarMult(points[firstFlat.firstPoint] - points[secondPoint]) < 0 ||
            Normal.scalarMult(points[firstFlat.secondPoint] - points[secondPoint]) < 0 ||
            Normal.scalarMult(points[firstFlat.thirdPoint] - points[secondPoint]) < 0 ) {
            Normal = -Normal;
        }
        if (used.find(sortTuple(std::make_tuple(edge.a, edge.b, secondPoint))) == used.end()) {
            flates.push_back(Flat<T>{edge.a, edge.b, secondPoint, Normal});
            used[sortTuple(std::make_tuple(edge.a, edge.b, secondPoint))] = true;
            Edge<T> firstEdge{edge.a, secondPoint, Normal, edge.b};
            Edge<T> secondEdge{edge.b, secondPoint, Normal, edge.a};
            edges.push(firstEdge);
            edges.push(secondEdge);
        }

    }

    return flates;
}

template <typename T>
void ConvexHullFinder<T>::SortFlates(std::vector<Flat<T>>& flates, std::vector<RadiusVector<T>> &points) {
    sortAllInsideFlates(flates, points);
    std::sort(flates.begin(), flates.end(), [](const Flat<T>& a, const Flat<T>& b) {
        std::tuple<size_t, size_t, size_t> firstTup = a.createTuple();
        size_t first = std::get<0>(firstTup) * 100000000 + std::get<1>(firstTup) * 10000 + std::get<2>(firstTup);
        std::tuple<size_t, size_t, size_t> secondTup = b.createTuple();
        size_t second = std::get<0>(secondTup) * 100000000 + std::get<1>(secondTup) * 10000 + std::get<2>(secondTup);
        return first < second;
    });
}

template <typename T>
size_t ConvexHullFinder<T>::FindSecondPoint(Edge<T> &edge, std::vector<RadiusVector<T>> &points) {

    size_t PointMinAngle = points.size();
    double minScalar = -1;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i != edge.a && i != edge.b && i != edge.c) {
            RadiusVector<T> normal = (points[edge.a] - points[edge.b]) * (points[i] - points[edge.a]);
            if (normal.scalarMult(points[edge.c] - points[i]) > eps) {
                normal = -normal;
            }
            double tempAngle = normal.scalarMult(edge.normal) / normal.length();
            if (PointMinAngle == points.size() || (PointMinAngle != points.size() && tempAngle < minScalar)) {
                PointMinAngle = i;
                minScalar = tempAngle;
            }
        }
    }
    return PointMinAngle;
}

template <typename T>
void ConvexHullFinder<T>::sortAllInsideFlates(std::vector<Flat<T>>& flates, std::vector<RadiusVector<T>> &points) {
    for (Flat<T>& flat : flates) {
        flat = sortedFlat(flat, points);
    }
}



template <typename T>
Flat<T> ConvexHullFinder<T>::sortedFlat(Flat<T> &flat, std::vector<RadiusVector<T>> &points) {
    std::tuple<size_t, size_t, size_t> tuple = flat.createTuple();
    size_t firstPoint = FindLess(tuple);
    size_t secondPoint = FindMax(tuple);
    size_t thirdPoint= FindMiddle(tuple);

    RadiusVector<T> first = points[secondPoint] - points[firstPoint];
    RadiusVector<T> second = points[thirdPoint] - points[firstPoint];
    if ((first * second).scalarMult(flat.Normal) > eps) {
        std::swap(secondPoint, thirdPoint);
    }

    return Flat<T>{firstPoint, secondPoint, thirdPoint, flat.Normal};
}


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    size_t TestCount;
    cin >> TestCount;

    for (size_t i = 0; i < TestCount; ++i) {
        size_t PountCount;
        cin >> PountCount;

        std::vector<RadiusVector<double>> points(PountCount);

        for (size_t j = 0; j < PountCount; ++j) {
            cin >> points[j].x >> points[j].y >> points[j].z;
        }

        std::shared_ptr<ConvexHullFinder<double >> convexHullFinder(new ConvexHullFinder<double >);

        std::vector<Flat<double>> convexHull = convexHullFinder->find(points);

        cout << convexHull.size() << endl;

        for (size_t j = 0; j < convexHull.size(); ++j) {
            cout << 3 << " ";
            cout << convexHull[j].firstPoint << " " << convexHull[j].secondPoint << " " << convexHull[j].thirdPoint << endl;
        }
    }
}