import time
from math import pi
from copy import deepcopy

from misc import *

from OpenGL import *
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import glm

from Vertex import Vertex
from Transformations import Transformations
from Object3D import Object3D
from GLDrawing import GLDrawing
from Polygon import Polygon
from BSpline import Bspline, BsplineIterator


class PolygonVisibilityPair:
    def __init__(self, poly, visibility) -> None:
        self.polygon = poly
        self.visibility = visibility


def get_camera_location(obj):
    ranges = obj.getCoordinatesRanges()
    x_min, x_max = ranges['x']
    if abs(x_min) > abs(x_max):
        x = abs(x_min)
    else:
        x = abs(x_max)

    y_min, y_max = ranges['y']
    if abs(y_min) > abs(y_max):
        y = abs(y_min)
    else:
        y = abs(y_max)

    z_min, z_max = ranges['z']
    if abs(z_min) > abs(z_max):
        z = abs(y_min)
    else:
        z = abs(y_max)

    return Vertex(x * 3, y * 3, z)


def drawScene():
    global pos_change, originalObject, bsPoints
    obj = originalObject.duplicate()
    obj.normalize()

    G = Vertex(0, 0, 0)
    O = Vertex(30, 0, 0)

    T = Transformations.all_transformations_matrix(O, G)
    obj.transform(getVectorAligningRotationMatrix(glm.normalize(glm.fvec3(glm.fvec3(pos_change.posChange) + glm.fvec3(0,0,-1))), glm.normalize(glm.fvec3(pos_change.posChange+pos_change.orientationChange))-glm.normalize(glm.fvec3(pos_change.posChange) + glm.fvec3(0,0,-1))))
    obj.translate(*glm.fvec3(pos_change.posChange))
    transformedVertices = []

    # gathers all vertices, transforms and stores them in transfomedVertices
    for p in obj.getPolygons():
        curr_p = []
        for v in p.getVertices():
            curr_p.append(v.transform(T))
        # appending polygon' vertices and is polygon visible
        transformedVertices.append((curr_p, p.isVisibleFrom(O)))

    # creates new Vertex and Polygon objects based on transformed verticess
    transformedPolygons = []
    for p in transformedVertices:
        currentPolygonVertices = []
        for v in p[0]:
            currentPolygonVertices.append(Vertex(*glm.vec3(v)))
        transformedPolygons.append(PolygonVisibilityPair(
            Polygon(*currentPolygonVertices), p[1]))  # p[1] is passed polygon visibility

    bsVertices = []
    for point in bsPoints:
        currentPoint = Vertex(*glm.vec3(point))
        bsVertices.append(Vertex(*glm.fvec3(currentPoint.transform(T))))

    # projecting to 2D and drawing
    P = Transformations.perspective_projection(O, G)

    for p in transformedPolygons:
        if p.visibility == False:
            continue
        vecs = p.polygon.projectSelfTo2D(P, 300, 0, 20)
        GLDrawing.gl_draw_triangle(*vecs)

    last_point = None
    for i, point in enumerate(bsVertices):
        if i+1 == len(bsVertices):
              break
        current_point = point.projectSelfTo2D(P, 300, 0, 20)
        first = Vertex(*glm.fvec3(pos_change.posChange))
        tangent_tip_point = Vertex(*glm.fvec3(pos_change.posChange + glm.normalize(pos_change.orientationChange)))
        first_tangent_point = Vertex(
            *glm.fvec3(first.transform(T))).projectSelfTo2D(P, 300, 0, 20)
        second_tangent_point = Vertex(
            *glm.fvec3(tangent_tip_point.transform(T))).projectSelfTo2D(P, 300, 0, 20)

        if i != 0:
            glColor3f(1, 0, 0)
            GLDrawing.gl_draw_line_(last_point, current_point)
            glColor3f(0, 1, 0)
            GLDrawing.gl_draw_line_(first_tangent_point, second_tangent_point)
        
            glColor3f(1, 1, 1)

        last_point = point.projectSelfTo2D(P, 300, 0, 20)


def myDisplay():
    global pos_change, originalObject, bsPoints
    global objectFilePath, pathFile
    originalObject = Object3D.loadObjectFromFile(objectFilePath)
    originalObject.normalize()
    originalObject.rotate(pi/2, 'z')
    originalObject.rotate(pi, 'x')

    
    bs = Bspline.createFromPathFile(pathFile)
    lastPoint = None
    bsPoints = []
    for i in BsplineIterator(bs, 0.1):
        glClearColor(0, 0, 0, 0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        # draw function
        bsPoints.append(i.posChange)
        pos_change = i
        drawScene()
        glutSwapBuffers()
        time.sleep(1e-3)


def myReshape(width, height):
    glDisable(GL_DEPTH_TEST)
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(0, width - 1, height - 1, 0, 0, 1)
    glMatrixMode(GL_MODELVIEW)


def main():
    global objectFilePath, pathFile
    if len(sys.argv) != 3:
        print('usage: py main.py objectFilePath pathFile')
    objectFilePath, pathFile = sys.argv[1:]
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(1000, 1000)
    glutInitWindowPosition(0, 0)
    glutInit()
    window = glutCreateWindow("OpenGL")
    glutReshapeFunc(myReshape)
    glutDisplayFunc(myDisplay)

    # glutKeyboardFunc(myKeyboard)
    glutMainLoop()


if __name__ == "__main__":
    main()
