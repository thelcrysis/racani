from typing import List, Tuple
import glm

class BsplineIteration:
    def __init__(self, posChange, orientationChange) -> None:
        self.posChange = posChange
        self.orientationChange = orientationChange

class BsplineIterator:
    def __init__(self, bspline, increment:float) -> None:
        self.bspline = bspline
        self.segments = bspline.N_segments
        self.maximum = float(bspline.N_segments)
        self.increment = increment
        
    def __iter__(self):
        self.i = 0
        return self

    def __next__(self):
        if self.i < self.maximum:
            posChange = self.bspline.calculatePositionForT(int(self.i//1), self.i%1)
            orientChange = self.bspline.calculateOrientationForT(int(self.i//1), self.i%1)
            self.i += self.increment
            return BsplineIteration(posChange, orientChange)
        else:
            raise StopIteration



class Bspline:
    def __init__(self, path: List[Tuple[float, float, float]]) -> None:
        self.path = [glm.fvec4(*p, 1) for p in path]
        print(self.path)
        self.N_segments = len(self.path) - 3

    @staticmethod
    def createFromPathFile(filePath):
        out = []
        with open(filePath, 'r') as f:
            for line in f:
                if '\n' in line:
                    line = line[:-1]
                if len(line) == 0:
                    continue
                out.append(tuple([float(x) for x in line.split()]))
        return Bspline(out)

    def calculateOrientationForT(self, segment:int, t:float):
        T = glm.fvec3((t**2, t, 1))
        B = glm.fmat3x4([[-1, 3, -3, 1],
                        [2, -4, 2, 0],
                        [-1, 0, 1, 0]])


        R = glm.fmat4x4([[*self.path[segment]],
                        [*self.path[segment+1]],
                        [*self.path[segment+2]],
                        [*self.path[segment+3]]])

        return R * B * 1/2 * T

    def calculatePositionForT(self, segment:int, t:float):
        '''get iteration i out of N total'''

        T = glm.fvec4(t**3, t**2, t, 1)
        B = glm.fmat4x4([[-1, 3, -3, 1],
                        [3, -6, 3, 0],
                        [-3, 0, 3, 0],
                        [1, 4, 1, 0]])
            
        R = glm.fmat4x4([[*self.path[segment]],
                        [*self.path[segment+1]],
                        [*self.path[segment+2]],
                        [*self.path[segment+3]]])
        return R * B * 1/6 * T


# bs = Bspline.createFromPathFile("spiral.path")
# print(bs.calculatePositionForT(0))
# print(bs.calculateOrientationForT(0.5))
