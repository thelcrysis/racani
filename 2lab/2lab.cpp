//*************************************************************************************************************
//	Particles in OpenGL, skeleton inherited from http://www.zemris.fer.hr/predmeti/irg/labosi/proba/cub.cpp
//
//  Billboard can be demonstrated by changing Center of Projection (ociste) position with k and l keys.
//
//  Compile and execute:  g++ 2lab.cpp -lGL -lglut -lGLU -lX11 -lm -lrt && ./a.out
//*************************************************************************************************************

#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>

using namespace std;
//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

class Particle;
class Vertex;
class ParticleSource;

GLuint window;
GLuint width = 700, height = 700;

vector<Particle *> particles;

int current_time = 0;
int last_time = 0;
int last_gen_time = 0;
GLuint texture;

typedef struct _Ociste
{
    GLdouble x;
    GLdouble y;
    GLdouble z;
} Ociste;

Ociste ociste = {0, 0, 5.0};

class Vertex
{
public:
    float x, y, z;
    Vertex()
    {
        this->x = this->y = this->z = 0.0f;
    }
    Vertex(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void translate(float dx, float dy, float dz)
    {
        this->x = x + dx;
        this->y = y + dy;
        this->z = z + dz;
    }
};

class Particle
{
public:
    Vertex position;
    float size;

    float random;
    int lifestart;
    float mileage;

    Vertex rotationAxis;
    float rotationAngle;
    Particle(Vertex position, float size)
    {
        this->position = position;
        this->size = size;
        random = ((double)rand() / RAND_MAX) / 4;
        lifestart = current_time;
    }

    void translate(float dx, float dy, float dz)
    {
        this->position.translate(dx, dy, dz);
        mileage += abs(dy);
        cout << "mill" << mileage << endl;
    }

    void draw()
    {

        glColor4f(0.8, 0.8, 0.8, 0.6 + random);
        glBindTexture(GL_TEXTURE_2D, texture);

        glPushMatrix();
        glTranslatef(position.x + (0.1 * sin(random * ((current_time - lifestart) / 100))), position.y, 0);
        glRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z); // boring rotate
        glRotatef(1 * random * (current_time - lifestart) / 10, 0, 0, 1);         // fun rotate
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex3f(-size, -size, 0.0);
        glTexCoord2d(1.0, 0.0);
        glVertex3f(-size, +size, 0.0);
        glTexCoord2d(1.0, 1.0);
        glVertex3f(+size, +size, 0.0);
        glTexCoord2d(0.0, 1.0);
        glVertex3f(+size, -size, 0.0);

        glEnd();
        glPopMatrix();
    }
};

class ParticleSource
{
public:
    Vertex v1, v2;
    ParticleSource(Vertex a, Vertex b)
    {
        this->v1 = a;
        this->v2 = b;
    }

    Vertex getSourcePoint()
    {
        double alpha = ((double)rand() / RAND_MAX);
        float new_x = (1 - alpha) * v1.x + alpha * v2.x;
        float new_y = (1 - alpha) * v1.y + alpha * v2.y;
        float new_z = (1 - alpha) * v1.z + alpha * v2.z;
        return Vertex(new_x, new_y, new_z);
    }
};

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void updatePerspective();
void myIdle();
GLuint LoadTextureRAW(const char *filename, int wrap);

//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char **argv)
{
    srand(time(NULL));

    // postavljanje dvostrukog spremnika za prikaz (zbog titranja)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = glutCreateWindow("Sustav cestica");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutIdleFunc(myIdle);
    texture = LoadTextureRAW("snow.bmp", 0);
    printf("Tipka: l - pomicanje ocista po x os +\n");
    printf("Tipka: k - pomicanje ocista po x os -\n");
    printf("Tipka: r - pocetno stanje\n");
    printf("esc: izlaz iz programa\n");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Set The Blending Function For Translucency
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glutMainLoop();
    return 0;
}

//*********************************************************************************
//	Osvjezavanje prikaza.
//*********************************************************************************

void myDisplay()
{
    // printf("Pozvan myDisplay()\n");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // boja pozadine - bijela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Particle *p : particles)
    {
        p->draw();
    }
    glutSwapBuffers(); // iscrtavanje iz dvostrukog spemnika (umjesto glFlush)
    // glutPostRedisplay();
}

//*********************************************************************************
//	Promjena velicine prozora.
//	Funkcija gluPerspective i gluLookAt se obavlja
//		transformacija pogleda i projekcija
//*********************************************************************************

void myReshape(int w, int h)
{
    // printf("MR: width=%d, height=%d\n",w,h);
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    updatePerspective();
}

void updatePerspective()
{
    glMatrixMode(GL_PROJECTION); // aktivirana matrica projekcije
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 0.5, 8.0); // kut pogleda, x/y, prednja i straznja ravnina odsjecanja
    glMatrixMode(GL_MODELVIEW);                            // aktivirana matrica modela
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // ociste x,y,z; glediste x,y,z; up vektor x,y,z
}

pair<Vertex, float> determineRotationAxisAndAngle(Vertex startingOrientation, Vertex goalOrientation)
{
    // renamed in order to shorten + align myself with lab exercise 1
    Vertex s = startingOrientation;
    Vertex e = goalOrientation;
    Vertex os; // rotation axis

    os.x = s.y * e.z - e.y * s.z;
    os.y = e.x * s.z - s.x * e.z;
    os.z = s.x * e.y - s.y * e.x;

    float norm_s = sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
    float norm_e = sqrt(e.x * e.x + e.y * e.y + e.z * e.z);
    float s_dot_e = s.x * e.x + s.y * e.y + s.z * e.z;

    float angle = acos(s_dot_e / (norm_s * norm_e));
    // returns angles in degrees
    return pair<Vertex, float>(Vertex(os.x, os.y, os.z), (angle / (2 * M_PI)) * 360);
}

void myIdle(void)
{
    current_time = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = current_time - last_time;
    if (current_time - last_gen_time > 1000 || last_gen_time == 0)
    {
        ParticleSource ps(Vertex(-2.2, 2.1, 0), Vertex(2.2, 2.1, 0));
        cout << "generate" << endl;
        for (int i = 0; i<3; i++)
        {
            Particle *p = new Particle(ps.getSourcePoint(), 0.1);
            particles.push_back(p);
        }
        last_gen_time = current_time;
        cout << current_time << ":"<<last_gen_time<<":"<<particles.size() << endl;

    }

    vector<Particle *> remaining_particles;
    for (Particle *p : particles)
    {
        if (p->mileage < 4)
        {
            remaining_particles.push_back(p);
        }
    }
    particles = remaining_particles;
    for (Particle *p : particles)
    {
        // billboard
        pair<Vertex, float> axis_rotation_and_angle = determineRotationAxisAndAngle(Vertex(0, 0, -1), Vertex(p->position.x - ociste.x,
                                                                                                             p->position.y - ociste.y, p->position.z - ociste.z));
        p->rotationAxis = axis_rotation_and_angle.first;
        p->rotationAngle = axis_rotation_and_angle.second;

        // move the particle
        p->translate(0.0, -4 * 1e-3, 0.0);
    }
        cout << current_time << ":"<<last_gen_time<<":"<<particles.size() << endl;

    myDisplay();
}

//*********************************************************************************
//	I/O
//*********************************************************************************

void myMouse(int button, int state, int x, int y)
{
    //	Desna tipka - brise canvas.
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        ociste.x = 0;
        updatePerspective();
        // glutPostRedisplay();
    }
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'l':
        ociste.x = ociste.x + 0.1;
        break;

    case 'k':
        ociste.x = ociste.x - 0.1;
        break;

    case 'r':
        ociste.x = 0.0;
        break;

    case 27:
        exit(0);
        break;
    }
    updatePerspective();
    // glutPostRedisplay();
}
//*********************************************************************************
//  instead of using SOIL, DeviL or other libs for texture loading
//  https://cboard.cprogramming.com/c-programming/79699-trying-load-raw-file-textures-please-help-c-opengl.html
//
//*********************************************************************************

GLuint LoadTextureRAW(const char *filename, int wrap)
{
    GLuint t;
    int width, height;
    unsigned char *data;
    FILE *file;
    // open texture data
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        return 0;
    }

    // allocate buffer
    width = 256;
    height = 256;
    data = (unsigned char *)malloc(width * height * 3);

    // read texture data
    fread(data, width * height * 3, 1, file);
    fclose(file);

    // allocate a texture name
    glGenTextures(1, &t);

    // select our current texture
    glBindTexture(GL_TEXTURE_2D, t);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    wrap ? GL_REPEAT : GL_CLAMP);

    // build our texture mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
                      GL_RGB, GL_UNSIGNED_BYTE, data);

    // free buffer
    free(data);

    return t;
}