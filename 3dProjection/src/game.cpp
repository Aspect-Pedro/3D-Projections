#include "../includes/global.h"


void printMatrix(Matrix mat) {
    DrawText(TextFormat("%f %f %f %f\n", mat.m0, mat.m1, mat.m2, mat.m3), 10, 10 + 20, 10, WHITE);
    DrawText(TextFormat("%f %f %f %f\n", mat.m4, mat.m5, mat.m6, mat.m7), 10, 20 + 20, 10, WHITE);
    DrawText(TextFormat("%f %f %f %f\n", mat.m8, mat.m9, mat.m10, mat.m11), 10, 30 + 20, 10, WHITE);
    DrawText(TextFormat("%f %f %f %f\n", mat.m12, mat.m13, mat.m14, mat.m15), 10, 40 + 20, 10, WHITE);
}


int main(void) {
  int screenWidth = 1920;
  int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "Projections");
  SetTargetFPS(60);
  ToggleFullscreen();

  const float middleX = screenWidth / 2;
  const float middleY = screenHeight / 2;

  Vector3 dotArray[8];
  dotArray[0] = {-1, -1, 1};
  dotArray[1] = {1, -1, 1};
  dotArray[2] = {1, 1, 1};
  dotArray[3] = {-1, 1, 1};
  dotArray[4] = {-1, -1, -1};
  dotArray[5] = {1, -1, -1};
  dotArray[6] = {1, 1, -1};
  dotArray[7] = {-1, 1, -1};

  float angle = 0;
  std::vector<Vector3> projectionArray(8);

  while (!WindowShouldClose()) {
    BeginDrawing();
    angle += 10 * GetFrameTime();

    Matrix rotation = MatrixRotateX(angle * DEG2RAD);
    rotation = MatrixMultiply(rotation, MatrixRotateY(angle * DEG2RAD));
    rotation = MatrixMultiply(rotation, MatrixRotateZ(angle * DEG2RAD));

    for (int i = 0; i < 8; i++) {    
      Vector3 ola = Vector3Transform(dotArray[i], rotation);
      float distance = 3;
      float z = 1 / (distance - ola.z);
      Matrix projection = {
        z,  0,  0,  0,
        0,  z,  0,  0,
        0,  0,  z,  0,
        0,  0,  0,  0
      };

      Matrix projected = MatrixMultiply(projection, rotation);
      projected = MatrixMultiply(projected, MatrixScale(200, 200, 1));
      Vector3 rotated = Vector3Transform(dotArray[i], projected);



      Vector3 finalPosition = { rotated.x + middleX, rotated.y + middleY, rotated.z };

      projectionArray[i] = finalPosition;
    }

    // Connecting
    for (int i = 0; i < 4; i++) {
      DrawLine(projectionArray[i].x, projectionArray[i].y, projectionArray[(i + 1) % 4].x, projectionArray[(i + 1) % 4].y, WHITE);
      DrawLine(projectionArray[i + 4].x, projectionArray[i + 4].y, projectionArray[((i + 1) % 4) + 4].x, projectionArray[((i + 1) % 4) + 4].y, WHITE);
      DrawLine(projectionArray[i].x, projectionArray[i].y, projectionArray[i + 4].x, projectionArray[i + 4].y, WHITE);
    }
    
    ClearBackground(BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

