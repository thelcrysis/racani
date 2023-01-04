import glm

def getVectorAligningRotationMatrix(vec1, vec2):
    """ 
    Finds rotation matrix that aligns vec1 to vec2; rodrigues formula
    """
    a = glm.normalize(vec1)
    b = glm.normalize(vec2)
    v = glm.cross(a, b)
    c = glm.dot(a, b)
    s = glm.length(v)
    kmat = glm.fmat3x3([[0, -v[2], v[1]], [v[2], 0, -v[0]], [-v[1], v[0], 0]])
    rotation_matrix = glm.fmat3() + kmat + ((1 - c) / (s**2)) * kmat * kmat
    return rotation_matrix
