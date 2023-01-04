
from OpenGL import *
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

class GLDrawing:
    @staticmethod
    def gl_draw_line_(vertex_0, vertex_1):
        glBegin(GL_LINES)
        glVertex2i(int(vertex_0[0]), int(vertex_0[1]))
        glVertex2i(int(vertex_1[0]), int(vertex_1[1]))
        glEnd()

    @staticmethod
    def gl_draw_triangle(v1, v2, v3):
        GLDrawing.gl_draw_line_(v1, v2)
        GLDrawing.gl_draw_line_(v2, v3)
        GLDrawing.gl_draw_line_(v1, v3)
