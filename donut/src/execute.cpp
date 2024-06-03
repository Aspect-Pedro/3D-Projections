#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;

void execute() {

    const int screenWidth = 1920;
    const int screenHeight = 1090;

    InitWindow(screenWidth, screenHeight, "Donut");
    ToggleFullscreen();
    SetTargetFPS(60);

    int pixelSize = 10;
    int gridWidth = screenWidth / pixelSize;
    int gridHeight = screenHeight / pixelSize;
    
    char grid[gridHeight][gridWidth];


    int thetaSpacing = 1;
    int phiSpacing = 1;

    float A = 0, B = 0;

    float radius1 = 10;
    float radius2 = 20;
    int k2 = 50; // Z of the real donut
    int k1 = gridHeight * k2 / (3 * (radius1 + radius2)); // Z of the projection plane


    while (WindowShouldClose() == false) {

        for (int i = 0; i < gridHeight; i++){
            for (int j = 0; j < gridWidth; j++){
                grid[i][j] = ' ';
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);


        for (int theta = 0; theta < 628; theta += thetaSpacing) {
            for (int phi = 0; phi < 628; phi += phiSpacing) {
                
                // Cos and sin of theta and phi
                float cosA = cos(A);
                float sinA = sin(A);
                float cosB = cos(B);
                float sinB = sin(B);
                float costheta = cos(theta);
                float sintheta = sin(theta);
                float cosphi = cos(phi);
                float sinphi = sin(phi);

                float circlex = radius2 + radius1 * costheta;
                float circley = radius1 * sintheta;

                // This is matrix multiplication but in a more optimized way
                float x = circlex * (cosB * cosphi + sinA * sinB * sinphi) - circley * cosA * sinB;
                float y = circlex * (sinB * cosphi - sinA * cosB * sinphi) + circley * cosA * cosB;
                float z = k2 + cosA * circlex * sinphi + circley * sinA;
                float ooz = 1 / z;

                int xp = (int) (gridWidth / 2 + k1 * ooz * x);
                int yp = (int) (gridHeight / 2 + k1 * ooz * y);

                grid[yp][xp] = '*';
            }
        }
        
        for (int i = 0; i < gridHeight; i++){
            for (int j = 0; j < gridWidth; j++){
                if (grid[i][j] == '*'){
                    DrawText("*", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                }
            }
        }


        EndDrawing();

        A += 0.04;
        B += 0.02;
    }

    CloseWindow();
}
