#include "CorpoCeleste.h"

void CorpoCeleste::draw() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glRotated(orbitAngle, orbitVector[X], orbitVector[Y], orbitVector[Z]);
    glPushMatrix();
        glTranslated(position[X], position[Y], position[Z]);
        glRotated(rotation[X], 1.0, 0.0, 0.0);
        glScaled(size, size, size);
        glRotated(rotation[Z]-23.439, 0.0, 0.0, 1.0);
        glRotated(rotation[Y], 0.0, 1.0, 0.0);
        Mesh::draw();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    rotation[Y] += 0.465;
    if(++orbitAngle > 360) orbitAngle = 0;
}

int CorpoCeleste::getTextureId() const {
    return textureId;
}

void CorpoCeleste::setTextureId(int textureId) {
    CorpoCeleste::textureId = textureId;
}

double CorpoCeleste::getOrbitAngle() const {
    return orbitAngle;
}

void CorpoCeleste::setOrbitAngle(double orbitAngle) {
    CorpoCeleste::orbitAngle = orbitAngle;
}

const Vector3d &CorpoCeleste::getRotation() const {
    return rotation;
}

void CorpoCeleste::setRotation(const Vector3d &rotation) {
    CorpoCeleste::rotation = rotation;
}

const Vector3d &CorpoCeleste::getOrbitVector() const {
    return orbitVector;
}

void CorpoCeleste::setOrbitVector(const Vector3d &orbitVector) {
    CorpoCeleste::orbitVector = orbitVector;
}

double CorpoCeleste::getSize() const {
    return size;
}

void CorpoCeleste::setSize(double size) {
    CorpoCeleste::size = size;
}
