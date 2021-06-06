#ifndef _MESH_H_
#define _MESH_H_

struct Mesh {
  uint8_t vertLength;
  Vector3f* vertices;
  
  uint8_t indicesLength;
  uint8_t* indices;
};

const Vector3f squareVertices[] = {
  {-0.5f, -0.5f, 0.0f},
  {0.5f, -0.5f, 0.0f},
  {0.5f, 0.5f, 0.0f},
  {-0.5f, 0.5f, 0.0f}
};

const uint8_t squareIndices[] = {
  0, 1, 2,
  0, 2, 3
};

const Vector3f pyramidVertices[] = {
  {-0.5f, 0, -0.5f},
  {0.5f, 0, -0.5f},
  {0.5f, 0, 0.5f},
  {-0.5f, 0, 0.5f},
  {0, 1.0f, 0}
};

const uint8_t pyramidIndices[] = {
  0, 2, 1,
  0, 3, 2,
  0, 1, 4,
  1, 2, 4,
  2, 3, 4,
  3, 0, 4
};

const Vector3f cubeVertices[] = {
  {0.5f, 0.5f, -0.5f},
  {0.5f, -0.5f, -0.5f},
  {0.5f, 0.5f, 0.5f},
  {0.5f, -0.5f, 0.5f},
  {-0.5f, 0.5f, -0.5f},
  {-0.5f, -0.5f, -0.5f},
  {-0.5f, 0.5f, 0.5f},
  {-0.5f, -0.5f, 0.5f}
};

const uint8_t cubeIndices[] = {
  4, 0, 2,
  2, 3, 7,
  6, 7, 5,
  1, 5, 7,
  0, 1, 3,
  4, 5, 1,
  4, 2, 6,
  2, 7, 6,
  6, 5, 4,
  1, 7, 3,
  0, 3, 2,
  4, 1, 0
};

const Mesh primitiveSquare = {
  4,
  squareVertices,

  6,
  squareIndices
};

const Mesh primitivePyramid = {
  5,
  pyramidVertices,

  18,
  pyramidIndices
};

const Mesh primitiveCube = {
  8,
  cubeVertices,

  36,
  cubeIndices
};

#endif
