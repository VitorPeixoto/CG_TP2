#ifndef TP2_CORPOCELESTE_H
#define TP2_CORPOCELESTE_H


#include <RigidBody.h>
#include <Mesh.h>
#include "PlanetEnum.h"

class CorpoCeleste : public Mesh {
    public:
        void draw();

        void drawOrbit();

        CorpoCeleste(PlanetEnum planet) : size(planet.getSize()), texturePath(planet.getTexturePath()), rotationAngle(planet.getRotationAngle()), orbitStep(planet.getTranslationAngle()), cullFace(planet.getCullFace()){
            this->translate(Vector3d(-planet.getDistance(), 0.0, 0.0));
            orbit.loadFromFile("../src/objects/Orbit3.obj");
        }

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

    void loadTexture();
    void pushChildren(CorpoCeleste children);

    void atualiza();

private:
        bool cullFace;
        int textureId;
        const char* texturePath;
        double rotationAngle, orbitAngle, orbitStep, size;
        Vector3d rotation, orbitVector;
        vector<CorpoCeleste> childrens;
        Mesh orbit;
};


#endif //TP2_CORPOCELESTE_H
