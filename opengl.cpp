#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <random>

#include <RigidBody.h>
#include <Mesh.h>
#include "src/CorpoCeleste.h"

using namespace std;

const double DISTANCE_DIVIDER = 35;//15;
const double SIZE_MULTIPLIER  = 15;
const double FPS_CONST = 0.017;
#define ORTHO_WIDTH  (orthoHalfWidth*2)
#define ORTHO_HEIGHT (orthoHalfHeight*2)

#define KEY_ESC 27
#define MULTIPLIER 0.5
#define HUD_MARGIN 20
#define BG_MOVEMENT_DELAY 25

RigidBody triangulo, quadrilatero;

bool* keyStates = new bool[127];
bool* specialKeyStates = new bool[127];
bool colidiu = false;

int windowWidth,
    windowHeight;

double angleX = 0.0,
       angleY = 0.0;

double x = 0.0, y = 0.0, z = 0.0;

double orthoHalfWidth, orthoHalfHeight;

Vector3d position(0.0, 0.0, 0.0);
Vector3d paceX(0.1, 0.0, 0.0);
Vector3d paceY(0.0, 0.1, 0.0);

CorpoCeleste sun(PlanetEnum::SUN()),
             mercury(PlanetEnum::MERCURY()),
             venus(PlanetEnum::VENUS()),
             earth(PlanetEnum::EARTH()),
                moon(PlanetEnum::MOON()),
                    rocket(PlanetEnum::ROCKET()),
                atmosphere(PlanetEnum::EARTH_ATMOSPHERE()),
             mars(PlanetEnum::MARS()),
             jupiter(PlanetEnum::JUPITER()),
             saturn(PlanetEnum::SATURN()),
                saturnRing(PlanetEnum::SATURN_RING()),
             uranus(PlanetEnum::URANUS()),
             neptune(PlanetEnum::NEPTUNE()),
             //pluto(PlanetEnum::PLUTO()),
             sky(PlanetEnum::STARS());

Mesh plane;

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt( 0,  0,  -1500/DISTANCE_DIVIDER,
               0,  0,  0,
               0,  1,  0);

    glRotated(angleX, 1.0, 0.0, 0.0);
    glRotated(angleY, 0.0, 1.0, 0.0);
    glCullFace(GL_FRONT);
    sky.draw();
    glCullFace(GL_BACK);
    glRotated(-angleX, 1.0, 0.0, 0.0);
    glRotated(-angleY, 0.0, 1.0, 0.0);

    glTranslated(x, y, z);
    glRotated(angleX, 1.0, 0.0, 0.0);
    glRotated(angleY, 0.0, 1.0, 0.0);
    sun.draw();
    /*mercury.draw();
    venus.draw();
    earth.draw();
    mars.draw();
    jupiter.draw();
    saturn.draw();
    uranus.draw();
    neptune.draw();*/
    //pluto.draw();

    /*glDisable(GL_CULL_FACE);
    plane.draw();
    glColor3d(0.0, 0.0, 0.0);
    plane.drawEdges();
    glEnable(GL_CULL_FACE);*/
    /*glDisable(GL_CULL_FACE);
    mercury.drawOrbit();
    earth.drawOrbit();
    venus.drawOrbit();
    mars.drawOrbit();
    jupiter.drawOrbit();
    saturn.drawOrbit();
    uranus.drawOrbit();
    neptune.drawOrbit();
    glEnable(GL_CULL_FACE);*/

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

void restart() {}

// Inicia algumas vari�veis de estado
void inicializa(void) {

    // cor para limpar a tela
    glClearColor(0, 0, 0.0, 0);
    glEnable(GL_DEPTH_TEST);

    //anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Esconder o ponteiro do mouse quando dentro da janela
    glutSetCursor(GLUT_CURSOR_NONE);

    sun.loadFromFile("../src/objects/Sphere.obj");
    mercury.loadFromFile("../src/objects/Sphere.obj");
    venus.loadFromFile("../src/objects/Sphere.obj");
    earth.loadFromFile("../src/objects/Sphere.obj");
        moon.loadFromFile("../src/objects/Sphere.obj");
        atmosphere.loadFromFile("../src/objects/Sphere.obj");
    mars.loadFromFile("../src/objects/Sphere.obj");
    sky.loadFromFile("../src/objects/Sphere.obj");
    jupiter.loadFromFile("../src/objects/Sphere.obj");
    saturn.loadFromFile("../src/objects/Sphere.obj");
        saturnRing.loadFromFile("../src/objects/Ring.obj");
    uranus.loadFromFile("../src/objects/Sphere.obj");
    neptune.loadFromFile("../src/objects/Sphere.obj");
    rocket.loadFromFile("../src/objects/Rocket.obj");
    //pluto.loadFromFile("../src/objects/Earth3.obj");

    plane.loadFromFile("../src/objects/Plane.obj");
    plane.scale(50);

    sun.loadTexture();

    mercury.setOrbitAngle(0.0);
    mercury.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    mercury.loadTexture();

    venus.setOrbitAngle(0.0);
    venus.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    venus.loadTexture();

    earth.setOrbitAngle(0.0);
    earth.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    moon.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    earth.loadTexture();
    moon.loadTexture();
    rocket.loadTexture();
    rocket.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    moon.pushChildren(rocket);
    atmosphere.loadTexture();
    earth.pushChildren(moon);
    //earth.pushChildren(atmosphere);

    mars.setOrbitAngle(0.0);
    mars.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    mars.loadTexture();

    jupiter.loadTexture();
    jupiter.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    saturn.loadTexture();
    saturn.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    saturnRing.loadTexture();
    saturn.pushChildren(saturnRing);

    uranus.loadTexture();
    uranus.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    neptune.loadTexture();
    neptune.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    /*pluto.loadTexture();
    pluto.setOrbitVector(Vector3d(0.0, -1.0, 0.0));*/

    sky.translate(Vector3d(0.0, 0.0, -100));
    sky.loadTexture();

    sun.pushChildren(mercury);
    sun.pushChildren(venus);
    sun.pushChildren(earth);
    sun.pushChildren(mars);
    sun.pushChildren(jupiter);
    sun.pushChildren(saturn);
    sun.pushChildren(uranus);
    sun.pushChildren(neptune);

    glPointSize(4);
    glLineWidth(5);
    glPolygonMode(GL_FRONT, GL_FILL);
}

// Callback de redimensionamento
void resizeScreen(int w, int h) {
    double windowAspectRatio = ((double)w)/h;
    double worldAspectRatio = ((ORTHO_WIDTH)/(ORTHO_HEIGHT));
    if (windowAspectRatio < worldAspectRatio) {
        double viewportHeight = w / worldAspectRatio;
        double viewportY = (h - viewportHeight)/2;
        glViewport(0, viewportY, w, viewportHeight);
    }
    else if (windowAspectRatio > worldAspectRatio) {
        double viewportWidth = (h) * worldAspectRatio;
        double viewportX = (w - viewportWidth)/2;
        glViewport(viewportX, 0, viewportWidth, h);
    } else {
        glViewport(0, 0, w, h);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w/(float)h, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboardHandle() {
    if(keyStates['d']) {
        x += 1.0;
        //position += paceX;
    }
    if(keyStates['a']) {
        x -= 1.0;
        //position -= paceX;
    }
    if(keyStates['w']) {
        z += 1.0;
        //position += paceY;
    }
    if(keyStates['s']) {
        z -= 1.0;
        //position -= paceY;
    }
    if(keyStates['[']) {
        y += 1.0;
        //position += paceY;
    }
    if(keyStates[']']) {
        y -= 1.0;
        //position -= paceY;
    }
    if(keyStates['+']) {
        angleX += 1.0;
    }
    if(keyStates['-']) {
        angleX -= 1.0;
    }
    if(keyStates['q']) {
        angleY -= 1.0;
    }
    if(keyStates['e']) {
        angleY += 1.0;
    }
    triangulo.setPosition(position);
    glutPostRedisplay();
}

void atualiza(int time) {
    glutTimerFunc(time, atualiza, time);
    keyboardHandle();

    sun.atualiza();
    /*mercury.atualiza();
    venus.atualiza();
    earth.atualiza();
    mars.atualiza();
    jupiter.atualiza();
    saturn.atualiza();
    uranus.atualiza();
    neptune.atualiza();*/

    colidiu = triangulo.collidesWith(quadrilatero.getPolygons(), quadrilatero.getPosition());
}


void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = true;

    if(key == KEY_ESC) {
        exit(0);
    }
}

void specialKeyDown(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void specialKeyUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

// Rotina principal
int main(int argc, char **argv) {
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a vers�o do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    windowWidth  = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Configura��o inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);

    // Abre a janela
    glutCreateWindow("GameBoilerplate");
    glutFullScreen();

    // Registra callbacks para alguns eventos
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resizeScreen);

    // Keyboard up and down callbacks
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    // Special keyboard up and down callbacks
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    inicializa();
    glutTimerFunc(0, atualiza, 17);

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
