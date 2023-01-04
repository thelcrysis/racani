import glm


class Vertex:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.polygons = []  # polygons that contain this vertex

    def getCoordinates(self):
        return self.x, self.y, self.z

    def getVector(self):
        return glm.fvec3(self.x, self.y, self.z)

    def getVector4D(self):
        return glm.fvec4(self.x, self.y, self.z, 1)

    def registerPolygon(self, polygon):
        self.polygons.append(polygon)

    def transform(self, t):
        return t * glm.vec4(*self.getVector(), 1)

    def transform3D(self, t):
        self.x, self.y, self.z = (t * self.getVector())

    def translate(self, dx, dy, dz):
        self.x += dx
        self.y += dy
        self.z += dz

    def scale(self, scaleFactor):
        self.x *= scaleFactor
        self.y *= scaleFactor
        self.z *= scaleFactor

    def rotate(self, angle, axis):
        '''angle is in radians'''
        assert axis in ['x', 'y', 'z']
        R = None
        if axis == 'x':
            R = glm.fmat3x3([[1, 0, 0],
                             [0, glm.cos(angle), -glm.sin(angle)],
                             [0, glm.sin(angle), glm.cos(angle)]])
        elif axis == 'y':
            R = glm.fmat3x3([[glm.cos(angle), 0, glm.sin(angle)],
                             [0, 1, 0],
                             [-glm.sin(angle), 0, glm.cos(angle)]])
        elif axis == 'z':
            R = glm.fmat3x3([[glm.cos(angle), -glm.sin(angle), 0],
                             [glm.sin(angle), glm.cos(angle), 0],
                             [0, 0, 1]])
        self.x, self.y, self.z = self.getVector() * R

    def projectSelfTo2D(self, p: glm.fmat4x4, translate_x=0, translate_y=0, scale=1):
        x_ph, y_ph, _, h_ph = p * self.getVector4D()
        x_p = x_ph / h_ph
        y_p = y_ph / h_ph
        return (x_p * scale + translate_x, y_p * scale + translate_y)
        

    def __str__(self):
        return f"Vertex({self.x},{self.y},{self.z})"

    @staticmethod
    def projectVertex(v: glm.fvec4, p: glm.fmat4x4, translate_x=0, translate_y=0, scale=1):
        x_ph, y_ph, _, h_ph = p * v
        x_p = x_ph / h_ph
        y_p = y_ph / h_ph
        return x_p * scale + translate_x, y_p * scale + translate_y

    def duplicate(self):
        return Vertex(self.x, self.y, self.z)
