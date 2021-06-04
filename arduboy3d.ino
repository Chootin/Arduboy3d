#include <Arduboy2.h>
#include "VectorMaths.h"
#include "Mesh.h"
#include "Camera.h"
#include "RenderedObject.h"
#include "RenderDithered.h"

#define TARGET_FRAMERATE 50
#define NEAR 0.01f
#define FAR 100.0f
#define DEG_2_RAD 0.01745329f
#define FOV_RADIANS (60.0f * DEG_2_RAD)

Arduboy2 arduboy;

Matrix4f projectionMat;

Vector3f lightVector;

RenderedObject* testObject;

float delta = 0.0f;

bool justRenderedFlag = false;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(TARGET_FRAMERATE);
  delta = 1 / 50.0f;
  setupProjectionMatrix();

  lightVector = {-120.0f * DEG_2_RAD, -120.0f * DEG_2_RAD, 0.0f};

  //Test
  testObject = new RenderedObject({0.0f, -0.5f, 5.0f}, {0.0f, 0.0f, 0.0f}, &primitivePyramid);
}

void loop() {
  if (!arduboy.nextFrame()) {
    justRenderedFlag = false;
    return;
  }

  Arduboy2Base::pollButtons();

  int xAxis = 0;
  int yAxis = 0;
  int zAxis = 0;

  if (Arduboy2Base::pressed(DOWN_BUTTON)) {
    xAxis = -1;
  } else if (Arduboy2Base::pressed(UP_BUTTON)) {
    xAxis = 1;
  }

  if (Arduboy2Base::pressed(LEFT_BUTTON)) {
    yAxis = -1;
  } else if (Arduboy2Base::pressed(RIGHT_BUTTON)) {
    yAxis = 1;
  }

  if (Arduboy2Base::pressed(A_BUTTON)) {
    zAxis = -1;
  } else if (Arduboy2Base::pressed(B_BUTTON)) {
    zAxis = 1;
  }

  testObject->rotation.x += xAxis * (60.0f * delta) * DEG_2_RAD;
  testObject->rotation.y += yAxis * (60.0f * delta) * DEG_2_RAD;
  testObject->rotation.z += zAxis * (60.0f * delta) * DEG_2_RAD;
  testObject->markUpdated();

  if (justRenderedFlag) {
    Serial.println("May not be hitting the target framerate!");
  }
  
  render();
  
  justRenderedFlag = true;
}

void render() {
  arduboy.clear();

  renderDithered(arduboy, projectionMat, testObject);

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
