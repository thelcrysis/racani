import glm
from Vertex import Vertex
from math import sqrt

class Transformations:
    z_axis_90deg_rotation = glm.fmat4x4([[0, -1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    x_axis_sign_toggle = glm.fmat4x4([[-1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])

    @staticmethod
    def translation(o: Vertex):
        # T1
        x_o, y_o, z_o = o.getCoordinates()
        t1 = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [-x_o, -y_o, -z_o, 1]]
        return glm.fmat4x4(t1)

    @staticmethod
    def z_axis_rotation(o: Vertex, g: Vertex):
        # T2
        x_g1, y_g1, z_g1, h = Transformations.translation(o) * glm.vec4(g.getVector(),1)
        sin_alpha = y_g1 / sqrt(x_g1 ** 2 + y_g1 ** 2)
        cos_alpha = x_g1 / sqrt(x_g1 ** 2 + y_g1 ** 2)
        t2 = [[cos_alpha, -sin_alpha, 0, 0], [sin_alpha, cos_alpha, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]
        return glm.fmat4x4(t2)

    @staticmethod
    def y_axis_rotation(o: Vertex, g: Vertex):
        g1 = Transformations.translation(o) * glm.vec4(g.getVector(),1)
        x_g2, y_g2, z_g2, h = Transformations.z_axis_rotation(o, g) * g1
        sin_beta = x_g2 / sqrt(x_g2 ** 2 + z_g2 ** 2)
        cos_beta = z_g2 / sqrt(x_g2 ** 2 + z_g2 ** 2)

        t3 = [[cos_beta, 0, sin_beta, 0], [0, 1, 0, 0], [-sin_beta, 0, cos_beta, 0], [0, 0, 0, 1]]
        return glm.fmat4x4(t3)

    @staticmethod
    def perspective_projection(o: Vertex, g: Vertex):
        x_g, y_g, z_g = g.getCoordinates()
        x_o, y_o, z_o = o.getCoordinates()

        H = sqrt((x_o - x_g) ** 2 + (y_o - y_g) ** 2 + (z_o - z_g) ** 2)
        P = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 0, 1 / H], [0, 0, 0, 0]]
        return glm.fmat4x4(P)

    @staticmethod
    def all_transformations_matrix(o: Vertex, g: Vertex):
        T1 = Transformations.translation(o)
        T2 = Transformations.z_axis_rotation(o, g)
        T3 = Transformations.y_axis_rotation(o, g)
        T4 = Transformations.z_axis_90deg_rotation
        T5 = Transformations.x_axis_sign_toggle

        return T5 * T4 * T3 * T2 * T1
