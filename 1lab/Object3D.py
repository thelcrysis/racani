from Polygon import Polygon
from Vertex import Vertex
from typing import List
import glm


class Object3D:
    def __init__(self, polygons):
        self._polygons = polygons
        self._vertices = set()
        for p in self._polygons:
            for v in p.getVertices():
                self._vertices.add(v)

        # used in duplicate method
        self.allVertices = []
        self.polygonLinking = []

    def getPolygons(self):
        return self._polygons

    def getVertices(self):
        self._vertices.clear()
        for p in self._polygons:
            for v in p.getVertices():
                self._vertices.add(v)
        return list(self._vertices)

    def getCoordinatesRanges(self):
        '''returns x,y and z ranges'''
        x_collection = []
        y_collection = []
        z_collection = []
        for v in self.getVertices():
            x_collection.append(v.x)
            y_collection.append(v.y)
            z_collection.append(v.z)
        return {'x': (min(x_collection), max(x_collection)),
                'y': (min(y_collection), max(y_collection)),
                'z': (min(z_collection), max(z_collection))}

    def getCenter(self):
        ranges = self.getCoordinatesRanges()
        x_mean = (ranges['x'][1]+ranges['x'][0])/2.0
        y_mean = (ranges['y'][1]+ranges['y'][0])/2.0
        z_mean = (ranges['z'][1]+ranges['z'][0])/2.0
        return Vertex(x_mean, y_mean, z_mean)

    def translate(self, dx, dy, dz):
        for v in self.getVertices():
            v.translate(dx, dy, dz)

    def scale(self, scaleFactor):
        for v in self.getVertices():
            v.scale(scaleFactor)

    def rotate(self, angle, axis):
        for v in self.getVertices():
            v.rotate(angle, axis)

    def transform(self, matrix:glm.fmat3x3):
        for v in self.getVertices():
            v.transform3D(matrix)

    def normalize(self):
        old_center_x, old_center_y, old_center_z = self.getCenter().getCoordinates()
        ranges = self.getCoordinatesRanges()
        xmin, xmax = ranges['x']
        ymin, ymax = ranges['y']
        zmin, zmax = ranges['z']
        x_size = xmax - xmin
        y_size = ymax - ymin
        z_size = zmax - zmin
        size = {'x': x_size, 'y': y_size, 'z': z_size}
        max_size_dim = max(size)

        if max_size_dim == 'y':
            # ako je tijelo vece u y smjeru
            scale_factor = 2 / y_size
        elif max_size_dim == 'x':
            # ako je tijelo vece u x smjeru
            scale_factor = 2 / x_size
        else:
            scale_factor = 2 / z_size
        self.translate(-old_center_x, -old_center_y, -old_center_z)
        self.scale(scale_factor)

    def getTransformedPolygonsList(self, transformMatrix: glm.fmat4x4):
        '''Returns dict of all polygons transformed using transformMatrix'''
        out = {}
        for p in self._polygons:
            for v in p.getVertices():
                print(v)
            out[p] = p.getTransformedVertices(transformMatrix)
        return out

    def __str__(self):
        str_ = ""
        for p in self._polygons:
            str_ += str(p) + " -> "
            for v in p.getVertices():
                str_ += str(v) + "; "
            str_ += "\n"
        return str_

    @staticmethod
    def loadObjectFromFile(file):
        vertices = []
        polygons = []
        polygonLinking = []
        with open(file, 'r') as f:
            for line in f:
                if line[-1] == '\n':
                    line = line[:-1]
                if len(line) == 0:
                    continue
                if line[0] not in ['g', 'v', 'f'] or line[0] == '#':
                    continue
                type_, x, y, z = line.split()
                if type_ == 'v':
                    v = Vertex(float(x), float(y), float(z))
                    vertices.append(Vertex(float(x), float(y), float(z)))

                elif type_ == 'f':
                    # x,y,z su ovdje reference na id triju tocaka
                    polygonLinking.append((int(x)-1, int(y)-1, int(z)-1))
                    v1 = vertices[int(x)-1]
                    v2 = vertices[int(y)-1]
                    v3 = vertices[int(z)-1]

                    polygons.append(Polygon(v1, v2, v3))
        newObject = Object3D(polygons)
        newObject.allVertices = vertices.copy()
        newObject.polygonLinking = polygonLinking.copy()
        return newObject

    def duplicate(self):
        dup_vertices = [v.duplicate() for v in self.allVertices]
        dup_polygons = []
        # recreating polygons with new vertices (dup_vertices and 'recipe' in polygonLinking linked by index!)
        for p in self.polygonLinking:
            v1 = dup_vertices[p[0]]
            v2 = dup_vertices[p[1]]
            v3 = dup_vertices[p[2]]
            dup_polygons.append(Polygon(v1, v2 ,v3))
        
        return Object3D(dup_polygons)
