#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include <vector>
#include <Vector3.h>
#include <algorithm>

typedef Vector3<double> Vector3d;
const int X = Vector3d::X;
const int Y = Vector3d::Y;
const int Z = Vector3d::Z;

class ConvexPolygon {
    public:
        ConvexPolygon();
        ConvexPolygon(std::vector<Vector3d> _vertices);
        virtual ~ConvexPolygon();

        std::vector<Vector3d> getVertices() { return vertices; }
        void push_vertex(Vector3d v) {
                vertices.push_back(v);
        }
        void clear_vertices() { vertices.clear(); }
        Vector3d back() { return vertices.back(); }
        Vector3d get(int index) { return vertices[index]; }
        void remove(int index) { vertices.erase(vertices.begin() + index); }
        unsigned int size() { return vertices.size(); }

        Vector3d getFarthestPointInDirection(Vector3d direction);
        Vector3d getFarthestPointInDirection(Vector3d direction, Vector3d translation);
        ConvexPolygon convexHull();
        bool contains(Vector3d testPoint);
    protected:

    private:
        std::vector<Vector3d> vertices;
};

#endif // CONVEXPOLYGON_H
