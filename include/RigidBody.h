#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <ConvexPolygon.h>

class RigidBody {
    public:
        RigidBody();
        RigidBody(std::vector<ConvexPolygon> _polygons);
        RigidBody(double _x, double _y, double _z);
        RigidBody(std::vector<ConvexPolygon> _polygons, double _x, double _y, double _z);
        virtual ~RigidBody();

        Vector3d getPosition() { return position; }
            void setPosition(Vector3d _position) { position = _position; }

        void translate(Vector3d translation);
        std::vector<ConvexPolygon> getPolygons() { return polygons; }

        void push_polygon(ConvexPolygon cp) { polygons.push_back(cp); }
        void clear_polygons() { polygons.clear(); }

        Vector3d support(ConvexPolygon myPolygon, ConvexPolygon otherPolygon, Vector3d d);
        Vector3d support(ConvexPolygon myPolygon, ConvexPolygon otherPolygon, Vector3d d, Vector3d translation);
        bool collidesWith(std::vector<ConvexPolygon> otherPolygons);
        bool collidesWith(std::vector<ConvexPolygon> otherPolygons, Vector3d otherPosition);
        ConvexPolygon lastSimplex, lastCollisionTriangle;
    protected:
        Vector3d position;
        std::vector<ConvexPolygon> polygons;
    private:
};

#endif // RIGIDBODY_H
