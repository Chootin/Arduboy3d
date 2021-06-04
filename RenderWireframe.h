#ifndef _RENDERWIREFRAME_H_
#define _RENDERWIREFRAME_H_

void renderWireframe(const Arduboy2& arduboy, const Matrix4f& projectionMat, RenderedObject* renderedObject) {
  renderedObject->onPreRender();

  Matrix4f transformProjMatrix = multiply(projectionMat, renderedObject->TransformMatrix());

  Mesh* mesh = renderedObject->Mesh();

  Vector3f* meshVerts = mesh->vertices;

  Vector3f verts[mesh->vertLength];

  for (int i = 0; i < mesh->vertLength; i++) {
    Vector4f result = multiply(transformProjMatrix, Vector4f(meshVerts[i]));
    if (result.w == 0.0f) {
      verts[i] = {0.0f, 0.0f, 0.0f};
      continue;
    }
    verts[i] = {WIDTH * (result.x / result.w + 0.5f), HEIGHT * (result.y / result.w + 0.5f), result.z / result.w};
  }

  uint8_t* indices = mesh->indices;
  for (int i = 0; i < mesh->indicesLength; i += 3) {
    const Vector3f& a = verts[indices[i]];
    const Vector3f& b = verts[indices[i + 1]];
    const Vector3f& c = verts[indices[i + 2]];

    arduboy.drawLine(a.x, a.y, b.x, b.y);
    arduboy.drawLine(b.x, b.y, c.x, c.y);
    arduboy.drawLine(c.x, c.y, a.x, a.y);
  }
}

#endif

#endif
