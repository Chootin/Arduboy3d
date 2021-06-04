#ifndef _RENDEREDOBJECT_H_
#define _RENDEREDOBJECT_H_

class RenderedObject {
  private:
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

      // Order of rotations x, y, z
      Matrix4f rotationMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(rotation.x), -sin(rotation.x), 0.0f,
        0.0f, sin(rotation.x), cos(rotation.x), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      };

      rotationMatrix = multiply({
        cos(rotation.y), 0.0f, sin(rotation.y), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(rotation.y), 0.0f, cos(rotation.y), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      }, rotationMatrix);

      rotationMatrix = multiply({
        cos(rotation.z), -sin(rotation.z), 0.0f, 0.0f,
        sin(rotation.z), cos(rotation.z), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      }, rotationMatrix);

      transformMatrix = multiply(transformMatrix, rotationMatrix);
    }
};

#endif
