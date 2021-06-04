#ifndef _RENDERDITHERED_H_
#define _RENDERDITHERED_H_

struct Dither {
  int width;
  int height;
  bool pattern[];
};

Dither ditherPatterns[] = {
  {1, 1,
  {
    0
  }},
  
  {2, 2,
  {
    0, 1,
    1, 0
  }},
  
  {1, 1,
  {
    1
  }}
};

// TODO: to implement shading, I intend to use dithering patterns.
// These patterns should be definited and referenced in an ordered array.
// Take in a light level from 0 - 1, then multiply that by the total number of dither patterns.
// The integer value of that will be the index in the array of the pattern.
// We can index that array using the screen coordinates of the fragment.

void renderDithered(const Arduboy2& arduboy, const Matrix4f& projectionMat, RenderedObject* renderedObject) {
  renderedObject->onPreRender();

  //Matrix4f transformProjMatrix = multiply(projectionMat, renderedObject->TransformMatrix());

  Mesh* mesh = renderedObject->Mesh();

  Vector3f* meshVerts = mesh->vertices;

  Vector3f world[mesh->vertLength];
  Vector3f projected[mesh->vertLength];

  float intensities[mesh->indicesLength / 3];

  for (int i = 0; i < mesh->vertLength; i++) {
    world[i] = multiply(renderedObject->TransformMatrix(), Vector4f(meshVerts[i]));
    Vector4f result = multiply(projectionMat, world[i]);
    
    if (result.w == 0.0f) {
      projected[i] = {0.0f, 0.0f, 0.0f};
      continue;
    }
    
    projected[i] = {WIDTH * (result.x / result.w + 0.5f), HEIGHT * (result.y / result.w + 0.5f), result.z / result.w};
  }

  uint8_t* indices = mesh->indices;
  for (int i = 0; i < mesh->indicesLength; i += 3) {
    int i0 = indices[i];
    int i1 = indices[i + 1];
    int i2 = indices[i + 2];
    
    const Vector3f& vec0 = subtract(world[i1], world[i0]);
    const Vector3f& vec1 = subtract(world[i2], world[i1]);

    const Vector3f& cross = crossProduct(vec0, vec1);
    
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
          arduboy.drawPixel(x, y);
        }
      }
    }
  }
}

#endif
