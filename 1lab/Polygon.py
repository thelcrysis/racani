from math import sqrt
from typing import List, Set
from Vertex import Vertex
import glm
from math import pi


class Polygon:
    def __init__(self, v1, v2, v3):
        self.v1 = v1
        self.v2 = v2
        self.v3 = v3
        
        self.v1.registerPolygon(self)
        self.v2.registerPolygon(self)
        self.v3.registerPolygon(self)
        
        

    def translate(self, dx, dy, dz):
        for v in self.getVertices():
            v.translate(dx, dy, dz)

    def scale(self, scaleFactor):
        for v in self.getVertices():
            v.scale(scaleFactor)

    def getVertices(self):
        return [self.v1, self.v2, self.v3]

    def getTransformedVertices(self, transformMatrix: glm.fmat4x4):
        '''Returns all vertices transformed using transformMatrix'''
        print(transformMatrix)
        new_v1 = Vertex(*transformMatrix * self.v1.getVector())
        new_v2 = Vertex(*transformMatrix * self.v2.getVector())
        new_v3 = Vertex(*transformMatrix * self.v3.getVector())
        return new_v1, new_v2, new_v3

    def getCenter(self) -> Vertex:
        x1, y1, z1 = self.v1.getCoordinates()
        x2, y2, z2 = self.v2.getCoordinates()
        x3, y3, z3 = self.v3.getCoordinates()
        x_center = (x1+x2+x3)/3.0
        y_center = (y1+y2+y3)/3.0
        z_center = (z1+z2+z3)/3.0
        return Vertex(x_center, y_center, z_center)

    def getNormal(self):
        p1 = self.v1.getVector()
        p2 = self.v2.getVector()
        p3 = self.v3.getVector()
        v13 = p3-p1
        v12 = p2-p1
        return glm.fvec3(glm.normalize(glm.cross(v12, v13)))

    def isVisibleFrom(self, O: Vertex):
        v_O = glm.vec3(O.getVector()-self.getCenter().getVector())
        v_N = glm.vec3(self.getNormal())
        dot_product = glm.dot(v_O, v_N)/((glm.length(v_O)*glm.length(v_N)))
        return True if dot_product > 0 else False


    def projectSelfTo2D(self, p: glm.fmat4x4, translate_x=0, translate_y=0, scale=1):
        projected_vecs = []
        for vec in [self.v1, self.v2, self.v3]:
            x_ph, y_ph, _, h_ph = p * vec.getVector4D()
            x_p = x_ph / h_ph
            y_p = y_ph / h_ph
            projected_vecs.append((x_p * scale + translate_x, y_p * scale + translate_y))
        return projected_vecs

    def __str__(self):
        return f"Polygon(Normal:{str(self.getNormal())})"
