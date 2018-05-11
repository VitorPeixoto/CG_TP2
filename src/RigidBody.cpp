#include "RigidBody.h"
#include <iostream>
using namespace std;

RigidBody::RigidBody() : position(0.0, 0.0, 0.0) {}

RigidBody::RigidBody(double _x, double _y, double _z) : position(_x, _y, _z) {}

RigidBody::~RigidBody(){}

void RigidBody::translate(Vector3d translation) {
    position += translation;
}

Vector3d RigidBody::support(ConvexPolygon myPolygon, ConvexPolygon otherPolygon, Vector3d d) {
    Vector3d p1 = myPolygon.getFarthestPointInDirection(d);
    Vector3d p2 = otherPolygon.getFarthestPointInDirection(-d);
    return p1 - p2;
}

Vector3d RigidBody::support(ConvexPolygon myPolygon, ConvexPolygon otherPolygon, Vector3d d, Vector3d translation) {
    Vector3d p1 = myPolygon.getFarthestPointInDirection(d, position);
    Vector3d p2 = otherPolygon.getFarthestPointInDirection(-d, translation);
    return p1 - p2;
}

bool containsOrigin(ConvexPolygon& simplex, Vector3d& d) {
    Vector3d A = simplex.back();

    Vector3d AO = -A;
    if(simplex.size() == 3) {
        Vector3d B = simplex.get(1);
        Vector3d C = simplex.get(0);

        Vector3d AB = B - A;
        Vector3d AC = C - A;

        Vector3d ACNormal = AB.tripleProduct(AC, AC);

        if (ACNormal.dotProduct(AO) >= 0.0) {
            simplex.remove(1);
            d.set(ACNormal);
        } else {
            Vector3d ABNormal = AC.tripleProduct(AB, AB);
            if (ABNormal.dotProduct(AO) < 0) {
                return true;
            } else {
                simplex.remove(0);
                d.set(ABNormal);
            }
        }
    } else {
        Vector3d B = simplex.get(0);
        Vector3d AB = B - A;
        Vector3d ABNormal = AB.tripleProduct(AO, AB);

        d.set(ABNormal);
    }
    return false;
}

bool RigidBody::collidesWith(std::vector<ConvexPolygon> otherPolygons, Vector3d otherPosition) {
    ConvexPolygon simplex;
    Vector3d origin (0.0, 0.0, 0.0);

    for(auto& myPolygon : polygons) {
        for(auto& otherPolygon : otherPolygons) {
            Vector3d d(0.0, 1.0, 0.0);
            simplex.clear_vertices();
            simplex.push_vertex(support(myPolygon, otherPolygon, d, otherPosition));
            d = -d;

            ConvexPolygon p;
            for(auto& otherVertex : otherPolygon.getVertices()) {
                    for(auto& myVertex : myPolygon.getVertices()) {
                        p.push_vertex((otherVertex + otherPosition) - (myVertex + position));
                    }
            }
            lastSimplex = p.convexHull();

            while (true) {
                simplex.push_vertex(support(myPolygon, otherPolygon, d, otherPosition));
                if (simplex.back().dotProduct(d) <= 0) break;
                else {
                    if (containsOrigin(simplex, d)) {
                        lastCollisionTriangle = simplex;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
