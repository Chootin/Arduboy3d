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
  {0, 1.0f, 0},
};

const uint8_t pyramidIndices[] = {
  0, 2, 1,
  0, 3, 2,
  0, 1, 4,
  1, 2, 4,
  2, 3, 4,
  3, 0, 4
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

#endif
