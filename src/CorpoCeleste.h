#ifndef TP2_CORPOCELESTE_H
#define TP2_CORPOCELESTE_H


#include <RigidBody.h>
#include <Mesh.h>

class CorpoCeleste : public RigidBody, public Mesh {
    public:
        void draw();

    int getTextureId() const;

    void setTextureId(int textureId);

    double getOrbitAngle() const;

    void setOrbitAngle(double orbitAngle);

    const Vector3d &getRotation() const;

    void setRotation(const Vector3d &rotation);

    const Vector3d &getOrbitVector() const;

    void setOrbitVector(const Vector3d &orbitVector);

    double getSize() const;

    void setSize(double size);

private:
        int textureId;
        double orbitAngle, size;
        Vector3d rotation, orbitVector;
};


#endif //TP2_CORPOCELESTE_H
