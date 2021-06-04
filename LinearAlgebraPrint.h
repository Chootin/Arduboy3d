#ifndef _LINEARALGEBRAPRINT_H_
#define _LINEARALGEBRAPRINT_H_

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

#endif
