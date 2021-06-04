#ifndef _VECTORMATHS_H_
#define _VECTORMATHS_H_

struct Matrix4f {
  float m00, m01, m02, m03,
  m10, m11, m12, m13,
  m20, m21, m22, m23,
  m30, m31, m32, m33;
};

struct Vector3f {
  float x;
  float y;
  float z;
};

struct Vector4f : Vector3f {
  float w;
  
  Vector4f(float _x, float _y, float _z, float _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  Vector4f(const Vector3f& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = 1.0f;
  }
};

struct Line {
  const int p0X, p0Y;
  const int p1X, p1Y;

  Line(int _p0X, int _p0Y, int _p1X, int _p1Y): 
  p0X(_p0X), p0Y(_p0Y), p1X(_p1X), p1Y(_p1Y)
  {
    a = p0Y - p1Y;
    b = p1X - p0X;
    c = (a * (p0X + p1X) + b * (p0Y + p1Y)) / -2;

    int xDiff = p1X - p0X;
    int yDiff = p1Y - p0Y;
    sqrMagnitude = xDiff * xDiff + yDiff * yDiff;
  }

  const int evaluate(int x, int y) {
    return a * x + b * y + c;
  }

  const int solveX(int y) {
    if (a == 0) {
      return p0X;
    }

    return (-b * y - c) / a;
  }

  const int solveY(int x) {
    if (b == 0) {
      return p0Y;
    }

    return (-b * x - c) / b;
  }

  const int getSqrMagnitude() {
    return sqrMagnitude;
  }

  private:
  int a;
  int b;
  int c;

  int sqrMagnitude;
};

struct Triangle {
  const int minX, minY;
  const int maxX, maxY;

  Triangle(const Line& line0, const Line& line1, const Line& line2) :
    minX(min(min(min(min(min(line0.p0X, line0.p1X), line1.p0X), line1.p1X), line2.p0X), line2.p1X)),
    minY(min(min(min(min(min(line0.p0Y, line0.p1Y), line1.p0Y), line1.p1Y), line2.p0Y), line2.p1Y)),
    maxX(max(max(max(max(max(line0.p0X, line0.p1X), line1.p0X), line1.p1X), line2.p0X), line2.p1X)),
    maxY(max(max(max(max(max(line0.p0Y, line0.p1Y), line1.p0Y), line1.p1Y), line2.p0Y), line2.p1Y))
  {
  }
};

Vector4f multiply(const Matrix4f& mat, const Vector4f& vec) {
  return Vector4f {
    mat.m00 * vec.x + mat.m01 * vec.y + mat.m02 * vec.z + mat.m03 * vec.w,
    mat.m10 * vec.x + mat.m11 * vec.y + mat.m12 * vec.z + mat.m13 * vec.w,
    mat.m20 * vec.x + mat.m21 * vec.y + mat.m22 * vec.z + mat.m23 * vec.w,
    mat.m30 * vec.x + mat.m31 * vec.y + mat.m32 * vec.z + mat.m33 * vec.w
  };
}

Matrix4f multiply(const Matrix4f& a, const Matrix4f& b) {
  return Matrix4f {
    a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30,
    a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31,
    a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32,
    a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33,
    
    a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30,
    a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,
    a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
    a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,
    
    a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30,
    a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,
    a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
    a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,
    
    a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30,
    a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,
    a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
    a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33
  };
}

Vector3f crossProduct(const Vector3f& vec0, const Vector3f& vec1) {
  //TODO:
}

int dotProduct(const Vector3f& vec0, const Vector3f& vec1) {
  return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z;
}

Vector3f subtract(const Vector3f& from, const Vector3f& value) {
  return {from.x - value.x, from.y - value.y, from.z - value.z};
}

#endif
