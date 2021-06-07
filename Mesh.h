#ifndef _MESH_H_
#define _MESH_H_

struct Mesh {
  uint16_t vertLength;
  float* vertices;
  
  uint16_t indicesLength;
  uint8_t* indices;
};

const float squareVertices[] PROGMEM = {
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  0.5f, 0.5f, 0.0f,
  -0.5f, 0.5f, 0.0f
};

const uint8_t squareIndices[] PROGMEM = {
  0, 1, 2,
  0, 2, 3
};

const float pyramidVertices[] PROGMEM = {
  -0.5f, 0, -0.5f,
  0.5f, 0, -0.5f,
  0.5f, 0, 0.5f,
  -0.5f, 0, 0.5f,
  0, 1.0f, 0
};

const uint8_t pyramidIndices[] PROGMEM = {
  0, 2, 1,
  0, 3, 2,
  0, 1, 4,
  1, 2, 4,
  2, 3, 4,
  3, 0, 4
};

const float cubeVertices[] PROGMEM = {
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, 0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  -0.5f, 0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f
};

const uint8_t cubeIndices[] PROGMEM = {
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
  12, squareVertices,
  6, squareIndices
};

const Mesh primitivePyramid = {
  15, pyramidVertices,
  18, pyramidIndices
};

const Mesh primitiveCube = {
  24, cubeVertices,
  36, cubeIndices
};

#endif
