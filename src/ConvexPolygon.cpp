#include "ConvexPolygon.h"
#include <iostream>

using namespace std;

ConvexPolygon::ConvexPolygon() {}

ConvexPolygon::ConvexPolygon(std::vector<Vector3d> _vertices) {
    vertices = _vertices;
}

ConvexPolygon::~ConvexPolygon() {}

Vector3d ConvexPolygon::getFarthestPointInDirection(Vector3d direction) {
    Vector3d vertex = vertices[0];
    double max = direction.dotProduct(vertex);

    int len = vertices.size();
    for (int i = 1; i < len; i++) {
        Vector3d v = vertices[i];
        double projection = direction.dotProduct(v);
        if (projection > max) {
            vertex = v;
            max = projection;
        }
    }
    return vertex;
}

Vector3d ConvexPolygon::getFarthestPointInDirection(Vector3d direction, Vector3d translation) {
    Vector3d vertex = vertices[0] + translation;
    double max = direction.dotProduct(vertex);

    int len = vertices.size();
    for (int i = 1; i < len; i++) {
        Vector3d v = vertices[i] + translation;
        double projection = direction.dotProduct(v);
        if (projection > max) {
            vertex = v;
            max = projection;
        }
    }
    return vertex;
}

bool ConvexPolygon::contains(Vector3d testPoint) {
    if (vertices.size() < 3) {
        return false;
    }

    Vector3d current = vertices[0],
                next = vertices[1];

    double last = (next[X] - current[X])      * (testPoint[Y] - current[Y]) -
                  (testPoint[X] - current[X]) * (next[Y] - current[Y]);
    for(int i = 0; i < vertices.size(); i++) {
        if(testPoint == vertices[i]) {
            // É uma quina
            return true;
        }
        current = vertices[i],
           next = vertices[(i+1) % vertices.size()];

        double value  = (next[X] - current[X])      * (testPoint[Y] - current[Y]) -
                        (testPoint[X] - current[X]) * (next[Y] - current[Y]);
        if (last * value < 0) {
            return false;
        }
    }
    return true;
}

double cross(const Vector3d &O, const Vector3d &A, const Vector3d &B) {
	return (A[X] - O[X]) * (B[Y] - O[Y]) - (A[Y] - O[Y]) * (B[X] - O[X]);
}

bool comparison(Vector3d v1, Vector3d v2) {
    return v1[X] < v2[X] || (v1[X] == v2[X] && v1[Y] < v2[Y]);
}

ConvexPolygon ConvexPolygon::convexHull() {
	int n = vertices.size(), k = 0;
	if (n <= 3) return *this;
	vector<Vector3d> hull(2*n);

	// Sort points lexicographically
	sort(vertices.begin(), vertices.end(), comparison);

	// Build lower hull
	for(int i = 0; i < n; ++i) {
		while (k >= 2 && cross(hull[k-2], hull[k-1], vertices[i]) <= 0) k--;
		hull[k++] = vertices[i];
	}

	// Build upper hull
	for (size_t i = n-1, t = k+1; i > 0; --i) {
		while (k >= t && cross(hull[k-2], hull[k-1], vertices[i-1]) <= 0) k--;
		hull[k++] = vertices[i-1];
	}

	hull.resize(k-1);
	ConvexPolygon response(hull);
	return response;
}
