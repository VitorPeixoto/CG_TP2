#include <iostream>
#include "CorpoCeleste.h"

void CorpoCeleste::draw() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    if(!cullFace) glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
        // Movimento de translação (Rotação em volta do pai)
        glRotated(orbitAngle, orbitVector[X], orbitVector[Y], orbitVector[Z]);
        glPushMatrix();
            //Posição do astro em relação ao sol
            glTranslated(position[X], position[Y], position[Z]);
            //Rotação no eixo X
            glRotated(rotation[X], 1.0, 0.0, 0.0);

            //Escala o astro para o seu tamanho relativo ao sol
            glPushMatrix();
            glScaled(size, size, size);
            //Rotação no eixo Z
            //glRotated(rotation[Z]-23.439, 0.0, 0.0, 1.0);

            //Rotação no eixo Y (em torno de si mesmo)
            glRotated(-rotation[Y], 0.0, 1.0, 0.0);
            Mesh::simpleDraw();
            glPopMatrix();
            if(!childrens.empty()) {
                for(CorpoCeleste children : childrens)
                    children.draw();
            }
        glPopMatrix();
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
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

void CorpoCeleste::loadTexture() {
    textureId = SOIL_load_OGL_texture(
            texturePath,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << ": " << this->texturePath << endl;
        exit(1);
    }
}

void CorpoCeleste::pushChildren(CorpoCeleste children) {
    this->childrens.push_back(children);
}

void CorpoCeleste::atualiza() {
    if((rotation[Y] += rotationAngle) >= 360.0) rotation[Y] -= 360.0;
    if((orbitAngle += orbitStep) >= 360) orbitAngle -= 360.0;

    for(auto& children : childrens)
        children.atualiza();
}

void CorpoCeleste::drawOrbit() {
    glPushMatrix();
        glScaled(this->position[X], 5, this->position[X]);
        orbit.simpleDraw();
    glPopMatrix();
}
