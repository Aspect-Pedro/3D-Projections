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

    int pixelSize = 5;
    int gridWidth = screenWidth / pixelSize;
    int gridHeight = screenHeight / pixelSize;
    
    char grid[gridHeight][gridWidth];
    int zBuffer[gridHeight][gridWidth];
    char luminance[] = ".,-~:;=!*#$@";

    Font arialFont = LoadFontEx("fonts/Arial.ttf", 10, 0, 250);
    bool isFontLoaded = IsFontReady(arialFont);
    TraceLog(LOG_INFO, isFontLoaded ? "Font loaded successfully" : "Font not loaded");

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

        for (int i = 0; i < gridHeight; i++){
            for (int j = 0; j < gridWidth; j++){
                zBuffer[i][j] = 0;
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

                float L = cosphi * costheta * sinB - cosA * costheta * sinphi - sinA * sintheta + cosB * (cosA * sintheta - costheta * sinA * sinphi);

                if (ooz > zBuffer[yp][xp]) {
                    zBuffer[yp][xp] = ooz;
                    grid[yp][xp] = luminance[(int) (L * 8) > 0 ? (int) (L * 8) : 0];
                }
            }
        }
        
        for (int i = 0; i < gridHeight; i++){
            for (int j = 0; j < gridWidth; j++){
                // if (grid[i][j] == '*'){
                //     DrawText("*", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '#'){
                //     DrawText("#", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '@'){
                //     DrawText("@", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '$'){
                //     DrawText("$", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '!'){
                //     DrawText("!", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '='){
                //     DrawText("=", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == ':'){
                //     DrawText(":", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == ';'){
                //     DrawText(";", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '~'){
                //     DrawText("~", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == '-'){
                //     DrawText("-", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // } else if (grid[i][j] == ','){
                //     DrawText(",", (j * pixelSize) + (pixelSize / 2), (i * pixelSize) + pixelSize/2, 10, WHITE);
                // }
                char c[2];
                c[0] = grid[i][j];
                c[1] = '\0';
                DrawTextEx(arialFont, c, {(float) (j * pixelSize) + (pixelSize / 2), (float) (i * pixelSize) + pixelSize/2}, arialFont.baseSize, 2, WHITE);
                
            }
        }


        EndDrawing();

        A += 0.04;
        B += 0.02;
    }

    CloseWindow();
}
