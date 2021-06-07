#ifndef _RENDERDITHERED_H_
#define _RENDERDITHERED_H_

struct Dither {
  int width;
  int height;
  bool* pattern;
};

#define NUM_DITHER_PATTERNS 6

Vector3f lightVector;

const bool darker[] = {
  0, 0, 0, 1,
  0, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0
};

const bool dark[] = {
  0, 1,
  0, 0
};

const bool medium[] = {
  0, 1,
  1, 0
};

const bool bright[] {
  1, 0,
  1, 1
};

const bool brighter[] {
  1, 1, 1, 0,
  1, 1, 1, 1,
  1, 0, 1, 1,
  1, 1, 1, 1
};

const bool light[] = {
  1
};

const Dither ditherPatterns[] = {
  {4, 4, darker},
  {2, 2, dark},
  {2, 2, medium},
  {2, 2, bright},
  {4, 4, brighter},
  {1, 1, light}
};

// To implement shading, I am using dithering patterns.
// These patterns are definited and referenced in an ordered array.
// Take in a light level from 0 - 1, then multiply that by the total number of dither patterns.
// The integer value of that is the index in the array of the pattern.
// We can index that array using the screen coordinates of the fragment.

void renderDithered(const Arduboy2& arduboy, const Matrix4f& projectionMat, RenderedObject* renderedObject) {
  renderedObject->onPreRender();

  //Matrix4f transformProjMatrix = multiply(projectionMat, renderedObject->TransformMatrix());

  Mesh* mesh = renderedObject->Mesh();

  float* meshVerts = mesh->vertices;

  uint16_t numVerts = mesh->vertLength / 3;
  Vector3f world[numVerts];
  Vector3f projected[numVerts];

  for (int i = 0, vI = 0; i < numVerts; i++) {
    vI = i * 3;
    const float vx = pgm_read_float_near(meshVerts + vI);
    const float vy = pgm_read_float_near(meshVerts + vI + 1);
    const float vz = pgm_read_float_near(meshVerts + vI + 2);
    
    world[i] = multiply(renderedObject->TransformMatrix(), Vector4f(vx, vy, vz, 1.0f));
    Vector4f result = multiply(projectionMat, world[i]);
    
    if (result.w == 0.0f) {
      projected[i] = {0.0f, 0.0f, 0.0f};
      continue;
    }
    
    projected[i] = {WIDTH * (result.x / result.w + 0.5f), HEIGHT * (result.y / result.w + 0.5f), result.z / result.w};
  }

  uint8_t* indices = mesh->indices;
  for (int i = 0; i < mesh->indicesLength; i += 3) {
    const uint8_t i0 = pgm_read_byte_near(indices + i);
    const uint8_t i1 = pgm_read_byte_near(indices + i + 1);
    const uint8_t i2 = pgm_read_byte_near(indices + i + 2);
    
    const Vector3f& vec0 = subtract(world[i1], world[i0]);
    const Vector3f& vec1 = subtract(world[i2], world[i1]);

    const Vector3f& cross = crossProduct(vec0, vec1);
    float mag = magnitude(cross);
    const Vector3f& normal = {cross.x / mag, cross.y / mag, cross.z / mag};

    float dot = dotProduct(normal, lightVector);
    float lightAngle = acos(dot); // Assuming lightVector has magnitude of 1
    float intensity = (PI - lightAngle) / PI;
    
    int ditherIndex = (int) (intensity * NUM_DITHER_PATTERNS);
    if (ditherIndex >= NUM_DITHER_PATTERNS) {
      ditherIndex = NUM_DITHER_PATTERNS - 1;
    }

    Dither* dither = &ditherPatterns[ditherIndex];
    
    const Vector3f& a = projected[i0];
    const Vector3f& b = projected[i1];
    const Vector3f& c = projected[i2];

    const Line line0 = Line((int) a.x, (int) a.y, (int) b.x, (int) b.y);
    const Line line1 = Line((int) b.x, (int) b.y, (int) c.x, (int) c.y);
    const Line line2 = Line((int) c.x, (int) c.y, (int) a.x, (int) a.y);

    const Triangle triangle = Triangle(line0, line1, line2);

    for (int y = triangle.minY; y <= triangle.maxY; y++) {
      for (int x = triangle.minX; x <= triangle.maxX; x++) {
        if (line0.evaluate(x, y) >= 0 && line1.evaluate(x, y) >= 0 && line2.evaluate(x, y) >= 0) {
          int patternIndex = (y % dither->height) * dither->width + (x % dither->width);

          if (dither->pattern[patternIndex]) {
            arduboy.drawPixel(x, y);
          }
        }
      }
    }
  }
}

#endif
