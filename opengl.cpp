#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <random>

#include <RigidBody.h>
#include <Mesh.h>
#include "src/CorpoCeleste.h"

using namespace std;

#define R 0
#define G 1
#define B 2

#define X 0
#define Y 1
#define Z 2

#define DISTANCE_DIVIDER 15
#define SIZE_MULTIPLIER 60000

#define SUN_SIZE 5
#define SUN_RADIUS 3.5

#define ORTHO_WIDTH  (orthoHalfWidth*2)
#define ORTHO_HEIGHT (orthoHalfHeight*2)

/**
 * Multiplicador do comprimento do mapa para mais telas
 **/
#define SCREENS 4

#define KEY_ESC 27
#define MULTIPLIER 0.5
#define FPS_CONST 0.017*MULTIPLIER
#define HUD_MARGIN 20
#define BG_MOVEMENT_DELAY 25

RigidBody triangulo, quadrilatero;

bool* keyStates = new bool[127];
bool* specialKeyStates = new bool[127];
bool colidiu = false;

int windowWidth,
    windowHeight;

double orthoHalfWidth, orthoHalfHeight;

Vector3d position(0.0, 0.0, 0.0);
Vector3d paceX(0.1, 0.0, 0.0);
Vector3d paceY(0.0, 0.1, 0.0);

CorpoCeleste sun, mercury, venus, earth, mars;

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt( 0,  0,  -1500/DISTANCE_DIVIDER,
               0,  0,  0,
               0,  1,  0);

    sun.draw();
    mercury.draw();
    venus.draw();
    earth.draw();
    mars.draw();

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

    sun.loadFromFile("../src/objects/Earth3.obj");
    mercury.loadFromFile("../src/objects/Earth3.obj");
    venus.loadFromFile("../src/objects/Earth3.obj");
    earth.loadFromFile("../src/objects/Earth3.obj");
    mars.loadFromFile("../src/objects/Earth3.obj");

    int textureId = SOIL_load_OGL_texture(
            "../src/images/Sun.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << endl;
        exit(1);
    }
    sun.setTextureId(textureId);
    sun.setSize(SUN_SIZE);
    textureId = SOIL_load_OGL_texture(
            "../src/images/Mercury.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << endl;
        exit(1);
    }
    mercury.setTextureId(textureId);
    mercury.translate(Vector3d(83.25*SUN_RADIUS/DISTANCE_DIVIDER, 0.0, 0.0));
    mercury.setOrbitAngle(0.01);
    mercury.setSize(SUN_SIZE*0.000003*SIZE_MULTIPLIER);
    mercury.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    textureId = SOIL_load_OGL_texture(
            "../src/images/Venus.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << endl;
        exit(1);
    }
    venus.setTextureId(textureId);
    venus.translate(Vector3d(155.57*SUN_RADIUS/DISTANCE_DIVIDER, 0.0, 0.0));
    venus.setOrbitAngle(0.01);
    venus.setSize(SUN_SIZE*0.000008702*SIZE_MULTIPLIER);
    venus.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    textureId = SOIL_load_OGL_texture(
            "../src/images/Earth.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << endl;
        exit(1);
    }
    earth.setTextureId(textureId);
    Vector3d earthPosition(215.1*SUN_RADIUS/DISTANCE_DIVIDER, 0.0, 0.0);
    earth.translate(earthPosition);
    earth.setOrbitAngle(0.01);
    earth.setSize(SUN_SIZE*0.00000917*SIZE_MULTIPLIER);
    earth.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    textureId = SOIL_load_OGL_texture(
            "../src/images/Mars.jpg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            0
    );
    if (!textureId) {
        cout << "Erro do SOIL: " << SOIL_last_result() << endl;
        exit(1);
    }
    mars.setTextureId(textureId);
    mars.translate(Vector3d((327.67*SUN_RADIUS/DISTANCE_DIVIDER), 0.0, 0.0));
    mars.setOrbitAngle(0.01);
    mars.setSize(SUN_SIZE*0.000003035*SIZE_MULTIPLIER);
    mars.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    glPointSize(4);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        position += paceX;
    }
    if(keyStates['a']) {
        position -= paceX;
    }
    if(keyStates['w']) {
        position += paceY;
    }
    if(keyStates['s']) {
        position -= paceY;
    }
    triangulo.setPosition(position);
    glutPostRedisplay();
}

void atualiza(int time) {
    glutTimerFunc(time, atualiza, time);
    keyboardHandle();
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
