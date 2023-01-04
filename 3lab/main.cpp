#include "include/TwoDimRastriginFunction.h"
#include "include/TwoDimGeneticAlg.h"
#include "include/GeneticAlgIterator.h"
#include "include/ColorGradient.h"
#include "include/Vertex.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <GL/glut.h>

GLuint window;
GLuint width = 1000, height = 1000;

typedef struct _Ociste {
    GLdouble x;
    GLdouble y;
    GLdouble z;
} Ociste;

enum Axis {
    x, y, z
};
GeneticAlgIterator *geneticAlgIterator;
TwoDimOptProblem *twoDimOptProblem;
std::vector<individual *> last_generation;
std::vector<individual *> current_generation;
std::vector<individual *> display_generation;
// interpolation time for each generation
int period_time = 2000;

bool start = false;
int previous_time = 0;
Ociste last_POV;
bool top_view = false;
Ociste ociste = {23.0, 15.0, 10.0};

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();

void myReshape(int width, int height);

void myMouse(int button, int state, int x, int y);

void idle();

void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

void drawGrid();

void drawCube(float x, float y, float z, float edgeLength, ColorTuple color);

void drawSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, ColorTuple color);

void
drawWireframePlane(Axis axis, float constant_axis_value, float start_vertex_x, float start_vertex_z, float end_vertex_x,
                   float end_vertex_z);

void drawIndividuals(std::vector<individual *> individuals, float scale = 1 / 20.f);

void draw3dFunction(TwoDimOptProblem *function, float scale);

void updatePerspective();

//*********************************************************************************
//	Main program.
//*********************************************************************************

int main(int argc, char **argv) {
    srand((unsigned) time(NULL));
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);
    window = glutCreateWindow("Genetic alg");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glDepthFunc(GL_LESS);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutIdleFunc(idle);

    // set Rastrigin function as the optimization problem
    twoDimOptProblem = new TwoDimRastriginFunction(-10, 10);
    // create genetic algorithm
    auto *tdga = new TwoDimGeneticAlg(200, twoDimOptProblem, 0.2f);
    geneticAlgIterator = new GeneticAlgIterator(1000, tdga, false);

    printf("Tipka: u, j - move center of projection on x axis (+,-)\n");
    printf("Tipka: i, k - move center of projection on y axis (+,-)\n");
    printf("Tipka: o, l - move center of projection on z axis (+,-)\n");
    printf("Tipka: r - top-down view toggle\n");
    printf("space: Start/Pause\n");
    printf("esc: exit\n");

    glutMainLoop();
    return 0;
}

void myDisplay(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGrid();
    drawIndividuals(display_generation, 1.f / 20);
    draw3dFunction(twoDimOptProblem, 1.f / 20);
    glutSwapBuffers();
}

//*********************************************************************************
//		View tranformation and projection.
//*********************************************************************************

void myReshape(int w, int h) {
    // printf("MR: width=%d, height=%d\n",w,h);
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    updatePerspective();
}

void updatePerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (float) width / height, 0.5, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0,
              0.0);
}

//*********************************************************************************
//	Drawing functions.
//*********************************************************************************

void
drawWireframePlane(Axis axis, float constant_axis_value, float start_vertex_x, float start_vertex_z, float end_vertex_x,
                   float end_vertex_z) {
    glBegin(GL_LINE_LOOP);
    glColor3ub(54, 54, 54);
    switch (axis) {
        case y:
            glVertex3f(start_vertex_x, constant_axis_value, start_vertex_z);
            glVertex3f(start_vertex_x, constant_axis_value, end_vertex_z);
            glVertex3f(end_vertex_x, constant_axis_value, end_vertex_z);
            glVertex3f(end_vertex_x, constant_axis_value, start_vertex_z);
            glEnd();
            break;
        case x:
            glVertex3f(constant_axis_value, start_vertex_x, start_vertex_z);
            glVertex3f(constant_axis_value, start_vertex_x, end_vertex_z);
            glVertex3f(constant_axis_value, end_vertex_x, end_vertex_z);
            glVertex3f(constant_axis_value, end_vertex_x, start_vertex_z);
            glEnd();
            break;
        case z:
            glVertex3f(start_vertex_x, start_vertex_z, constant_axis_value);
            glVertex3f(start_vertex_x, end_vertex_z, constant_axis_value);
            glVertex3f(end_vertex_x, end_vertex_z, constant_axis_value);
            glVertex3f(end_vertex_x, start_vertex_z, constant_axis_value);
            glEnd();
            break;
    }

}

void drawGrid() {
    float start_x = -10.f;
    float start_y = -10.f;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            float curr_x = start_x + (float) i;
            float curr_y = start_y + (float) j;
            drawWireframePlane(y, -10.f, curr_x, curr_y, curr_x + 1, curr_y + 1);
            drawWireframePlane(x, -10.f, curr_x, curr_y, curr_x + 1, curr_y + 1);
            drawWireframePlane(z, -10.f, curr_x, curr_y, curr_x + 1, curr_y + 1);
        }
    }
}

void drawSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, ColorTuple color) {
    glBegin(GL_QUADS);
    glColor3ub(color.r, color.b, color.g);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}

void drawCube(float x, float y, float z, float edgeLength, ColorTuple color) {
    float hf = edgeLength / 2.f; //half length
    Vertex v1{x - hf, y - hf, z - hf};
    Vertex v2{x + hf, y - hf, z - hf};
    Vertex v3{x + hf, y + hf, z - hf};
    Vertex v4{x - hf, y + hf, z - hf};
    Vertex v5{x - hf, y - hf, z + hf};
    Vertex v6{x + hf, y - hf, z + hf};
    Vertex v7{x + hf, y + hf, z + hf};
    Vertex v8{x - hf, y + hf, z + hf};

    drawSquare(v1, v5, v8, v4, color);
    drawSquare(v5, v6, v7, v8, color);
    drawSquare(v2, v3, v7, v5, color);
    drawSquare(v1, v2, v3, v4, color);
    drawSquare(v1, v5, v6, v2, color);
    drawSquare(v4, v8, v7, v3, color);
}

void draw3dFunction(TwoDimOptProblem *function, float scale) {
    ColorGradient colorGradient(std::vector<ColorTuple>{
            ColorTuple{61, 1, 98},
            ColorTuple{28, 0, 97},
            ColorTuple{10, 24, 74},
            ColorTuple{13, 86, 15},
            ColorTuple{59, 97, 1},
            ColorTuple{99, 93, 3},
            ColorTuple{100, 63, 0},
            ColorTuple{100, 31, 0},
            ColorTuple{97, 1, 1}
    });
    float start_x = -10.f;
    float start_y = -10.f;

    float max_z;
    bool max_set = false;
    float min_z;
    bool min_set = false;

    // Calculate min and max z values as a preparation for gradient calculation
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            float curr_x = start_x + (float) i * 0.1f;
            float curr_y = start_y + (float) j * 0.1f;
            float curr_z = function->getOutput(curr_x, curr_y);
            if (!max_set || curr_z > max_z) {
                max_z = curr_z;
                max_set = true;
            }
            if (!min_set || curr_z < min_z) {
                min_z = curr_z;
                min_set = true;
            }
        }
    }

    start_x = -10.f;
    start_y = -10.f;
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            float curr_x = start_x + (float) i * 0.1f;
            float curr_y = start_y + (float) j * 0.1f;
            // draw function surface
            glBegin(GL_QUADS);
            float alpha = ((function->getOutput(curr_x, curr_y) - min_z) / (max_z - min_z));
            ColorTuple color = colorGradient.getInterpolatedColor(alpha);
            glColor3ub(color.r + 30, color.g + 30, color.b + 30);

            glVertex3f(curr_x, function->getOutput(curr_x, curr_y) * scale - 10.f, curr_y);
            alpha = ((function->getOutput(curr_x, curr_y + 0.1f) - min_z) / (max_z - min_z));
            color = colorGradient.getInterpolatedColor(alpha);
            glColor3ub(color.r + 30, color.g + 30, color.b + 30);

            glVertex3f(curr_x, function->getOutput(curr_x, curr_y + 0.1f) * scale - 10.f, curr_y + 0.1f);
            alpha = ((function->getOutput(curr_x + 0.1f, curr_y + 0.1f) - min_z) / (max_z - min_z));
            color = colorGradient.getInterpolatedColor(alpha);
            glColor3ub(color.r + 30, color.g + 30, color.b + 30);

            glVertex3f(curr_x + 0.1f, function->getOutput(curr_x + 0.1f, curr_y + 0.1f) * scale - 10.f, curr_y + 0.1f);
            alpha = ((function->getOutput(curr_x + 0.1f, curr_y) - min_z) / (max_z - min_z));
            color = colorGradient.getInterpolatedColor(alpha);
            glColor3ub(color.r + 30, color.g + 30, color.b + 30);
            glVertex3f(curr_x + 0.1f, function->getOutput(curr_x + 0.1f, curr_y) * scale - 10.f, curr_y);
            glEnd();

            // Wireframe
            glLineWidth(2.5f);
            glBegin(GL_LINE_LOOP);
            glColor3ub(0, 0, 0);
            glVertex3f(curr_x, function->getOutput(curr_x, curr_y) * scale - 10.f, curr_y);
            glVertex3f(curr_x, function->getOutput(curr_x, curr_y + 0.1f) * scale - 10.f, curr_y + 0.1f);
            glVertex3f(curr_x + 0.1f, function->getOutput(curr_x + 0.1f, curr_y + 0.1f) * scale - 10.f, curr_y + 0.1f);
            glVertex3f(curr_x + 0.1f, function->getOutput(curr_x + 0.1f, curr_y) * scale - 10.f, curr_y);
            glEnd();
        }
    }
}

void drawIndividuals(std::vector<individual *> individuals, float scale) {
    // draw individuals from a genetic alg population
    for (auto indi: individuals) {
        if (indi->values.x <= -10.f || indi->values.x >= 10.f || indi->values.y <= -10.f ||
            indi->values.y >= 10.f)
            continue;
        drawCube(indi->values.x, twoDimOptProblem->getOutput(indi->values.x, indi->values.y) * scale - 10.f,
                 indi->values.y,
                 0.1, ColorTuple{0, 0, 255});
    }
}

void idle() {
    if (start) {
        if (previous_time == 0) {
            previous_time = glutGet(GLUT_ELAPSED_TIME);
            last_generation = geneticAlgIterator->getCurrentPopulation();
            geneticAlgIterator->step();
            current_generation = geneticAlgIterator->getCurrentPopulation();
        }
        int delta_time = glutGet(GLUT_ELAPSED_TIME) - previous_time;

        if (delta_time < period_time) {
            // interpolated between two generations
            float alpha = (float) delta_time / period_time;
            display_generation.clear();
            for (int i = 0; i < last_generation.size(); i++) {
                auto *interpolated_indi = new individual();
                individual *first = last_generation.at(i);
                individual *second = current_generation.at(i);
                interpolated_indi->values.x = first->values.x + (second->values.x - first->values.x) * alpha;
                interpolated_indi->values.y = first->values.y + (second->values.y - first->values.y) * alpha;
                display_generation.push_back(interpolated_indi);
            }
        } else {
            // calculate next generation
            previous_time = glutGet(GLUT_ELAPSED_TIME);
            last_generation = current_generation;
            geneticAlgIterator->step();
            current_generation = geneticAlgIterator->getCurrentPopulation();
        }
    }
    myDisplay();
}

void myMouse(int button, int state, int x, int y) {
    //	Desna tipka - brise canvas.
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        ociste.x = 0;
        updatePerspective();
        glutPostRedisplay();
    }
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
    switch (theKey) {
        case 'u':
            ociste.x = ociste.x + 0.5;
            break;

        case 'j':
            ociste.x = ociste.x - 0.5;
            break;

        case 'i':
            ociste.y = ociste.y + 0.5;
            break;

        case 'k':
            ociste.y = ociste.y - 0.5;
            break;

        case 'o':
            ociste.z = ociste.z + 0.5;
            break;

        case 'l':
            ociste.z = ociste.z - 0.5;
            break;

        case 'r':
            if (not top_view) {
                last_POV = ociste;
                ociste.x = 0.1;
                ociste.y = 20.0;
                ociste.z = 0.0;
            } else ociste = last_POV;
            top_view = not top_view;
            break;

        case 'z':
            ociste.x = 0.1;
            ociste.y = 10.0;
            ociste.z = 0.0;
            break;

        case 32:
            // space
            start = not start;
            break;
        case 27:
            exit(0);
            break;
    }
    updatePerspective();
    glutPostRedisplay();
}
