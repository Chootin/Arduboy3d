#include <Arduboy2.h>
#include "VectorMaths.h"
#include "Mesh.h"

#define NEAR 0.01f
#define FAR 100.0f
#define DEG_2_RAD 0.01745329f
#define FOV_RADIANS (60.0f * DEG_2_RAD)

class RenderedObject {
  Mesh* mesh;  
  Matrix4f transformMatrix;

  bool updated = true;

  public:
  Vector3f position;
  Vector3f rotation;
  
  RenderedObject(Vector3f _position, Vector3f _rotation, Mesh* _mesh) {
    position = _position;
    rotation = _rotation;
    mesh = _mesh;
  }

  void markUpdated() {
    updated = true;
  }

  void onPreRender() {
    if (updated) {
      updateTransformMatrix();
      updated = false;
    }
  }

  const Matrix4f& TransformMatrix() {
    return transformMatrix;
  }

  const Mesh* Mesh() {
    return mesh;
  }

  private:
  void updateTransformMatrix() {
    transformMatrix = {
      1.0f, 0.0f, 0.0f, position.x,
      0.0f, 1.0f, 0.0f, position.y,
      0.0f, 0.0f, 1.0f, position.z,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    Matrix4f rotationMatrix = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, cos(rotation.x), -sin(rotation.x), 0.0f,
      0.0f, sin(rotation.x), cos(rotation.x), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    rotationMatrix = multiply(rotationMatrix, {
      cos(rotation.y), 0.0f, sin(rotation.y), 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      -sin(rotation.y), 0.0f, cos(rotation.y), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    });

    rotationMatrix = multiply(rotationMatrix, {
      cos(rotation.z), -sin(rotation.z), 0.0f, 0.0f,
      sin(rotation.z), cos(rotation.z), 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    });

    transformMatrix = multiply(transformMatrix, rotationMatrix);
  }
};

Arduboy2 arduboy;

Matrix4f projectionMat;

RenderedObject* testObject;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(50);
  setupProjectionMatrix();

  //Test
  testObject = new RenderedObject({0.0f, -0.5f, 5.0f}, {0.0f, 0.0f, 0.0f}, &primativePyramid);
}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }

  testObject->rotation.y += (30.0f / 50.0f) * DEG_2_RAD;
  testObject->rotation.x += (45.0f / 50.0f) * DEG_2_RAD;
  testObject->markUpdated();

  //Serial.println("Rendering...");
  render();
}

void printVertex(char msg[], Vector4f vec) {
  Serial.print(msg);
  Serial.print(vec.x);
  Serial.print(", ");
  Serial.print(vec.y);
  Serial.print(", ");
  Serial.print(vec.z);
  Serial.print(", ");
  Serial.println(vec.w);
}

void printMatrix(char msg[], Matrix4f mat) {
  Serial.print(msg);
  Serial.print(mat.m00);
  Serial.print(", ");
  Serial.print(mat.m01);
  Serial.print(", ");
  Serial.print(mat.m02);
  Serial.print(", ");
  Serial.print(mat.m03);
  Serial.println(", ");
  Serial.print(mat.m10);
  Serial.print(", ");
  Serial.print(mat.m11);
  Serial.print(", ");
  Serial.print(mat.m12);
  Serial.print(", ");
  Serial.print(mat.m13);
  Serial.println(", ");
  Serial.print(mat.m20);
  Serial.print(", ");
  Serial.print(mat.m21);
  Serial.print(", ");
  Serial.print(mat.m22);
  Serial.print(", ");
  Serial.print(mat.m23);
  Serial.println(", ");
  Serial.print(mat.m30);
  Serial.print(", ");
  Serial.print(mat.m31);
  Serial.print(", ");
  Serial.print(mat.m32);
  Serial.print(", ");
  Serial.print(mat.m33);
  Serial.println(", ");
}

void render() {
  arduboy.clear();
  
  testObject->onPreRender();

  Matrix4f transformProjMatrix = multiply(projectionMat, testObject->TransformMatrix());

  //printMatrix("Matrix: ", transformProjMatrix);
  
  Mesh* mesh = testObject->Mesh();

  Vector3f* meshVerts = mesh->vertices;

  Vector3f verts[mesh->vertLength];

  //Serial.print("Mesh vert length: ");
  //Serial.println(mesh->vertLength);
  
  for (int i = 0; i < mesh->vertLength; i++) {
    Vector4f result = multiply(transformProjMatrix, Vector4f(meshVerts[i]));
    if (result.w == 0.0f) {
      verts[i] = {0.0f, 0.0f, 0.0f};
      //printVertex("Clipped Vertex: ", verts[i]);
      continue;
    }
    //printVertex("Vertex: ", result);
    verts[i] = {WIDTH * (result.x / result.w + 0.5f), HEIGHT * (result.y / result.w + 0.5f), result.z / result.w};
    //printVertex("Vertex: ", verts[i]);
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
  
  arduboy.display();
}

void setupProjectionMatrix() {
  float aspect = WIDTH / HEIGHT;
  float tanHalfFov = tan(FOV_RADIANS / 2.0f);

  projectionMat = {
    1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
    0.0f, 0.0f, -(FAR + NEAR) / (FAR - NEAR), -(2.0f * FAR * NEAR) / (FAR - NEAR),
    0.0f, 0.0f, -1.0f, 0.0f
  };
}
