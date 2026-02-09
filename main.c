#include <raylib.h>
#include <stdbool.h>


#define WIN_W 800
#define WIN_H 600


typedef struct{
    Rectangle board_1;
    Rectangle board_2;
    Rectangle ball;
    int score;
} State;



int main(){
    InitWindow(WIN_W, WIN_H, "pong");
    SetTargetFPS(60);
    
    // game objects
    Color gameObjectColor = WHITE;
    int playerScore = 0;
    int cpuScore = 0;
    int scoreFontSize = 50;
    
    // board
    int boardWidth = 20;
    int boardHeight = 100;
    float boardCenterY = ((float)WIN_H / 2) - ((float)boardHeight / 2);
    int boardSpeed = 10;
    int cpuBoardSpeed = 7;

    // ball
    int ballWidth = 15;
    int ballHeight = 15;
    float ballCenterX = ((float)WIN_W / 2) - ((float)ballWidth / 2);
    float ballCenterY = ((float)WIN_H / 2) - ((float)ballHeight / 2);
    int ballVelX = 10;
    int ballVelY = 10;
    bool ballIsMoving = false;
    bool ballMoveStraight = true;

    // middle dash lines
    int dlW = 5;
    int dlH = 13;
    int dlPosX = ((float)WIN_W / 2) - ((float)dlW / 2);
    int dlGap = 13;
    int dlCount = WIN_H / (dlH + 2);

    // player and cpu board declaration
    Rectangle playerBoard = {
        .x = WIN_W - 20 - 20, // player located on right 10px padding + 20 px width
        .y = boardCenterY, //  start at the middle of screen
        .width = boardWidth,
        .height = boardHeight,
    };

    Rectangle cpuBoard = {
        .x = 0 + 20, // cpu located on left 10px padding
        .y = boardCenterY, //  start at the middle of screen
        .width = boardWidth,
        .height = boardHeight,
    };

    Rectangle ball = {
        .x = ballCenterX,
        .y = ballCenterY,
        .width = ballWidth,
        .height = ballHeight,
    };

    while(!WindowShouldClose()){
        // player movement
        if(IsKeyDown(KEY_UP)){
            if(playerBoard.y >= 0){
                playerBoard.y -= boardSpeed;
            }
            if(!ballIsMoving){
                ballIsMoving = true;
            }
        }
        if(IsKeyDown(KEY_DOWN)){
            if(playerBoard.y <= ((float)WIN_H - (playerBoard.height))){
                playerBoard.y += boardSpeed;
            }
            if(!ballIsMoving){
                ballIsMoving = true;
            }
        }
        
        // ball movement
        if(ballIsMoving){
            ball.x += ballVelX;
            if(!ballMoveStraight){  // at game start, ball should move straight for fairness
                ball.y += ballVelY;
            }
            
            // cpu movement
            // only move if ball is coming towards cpu
            if(ball.x < ((float)WIN_W / 2)){
                if((cpuBoard.y + (cpuBoard.height / 2)) > (ball.y + (ball.height / 2))){
                    if(cpuBoard.y >= 0) cpuBoard.y -= cpuBoardSpeed;
                } else {
                    if(cpuBoard.y <= (WIN_H - cpuBoard.height)) cpuBoard.y += cpuBoardSpeed;
                }
            }
        }

        // x axis outbound calculation
        if(ball.x > (WIN_W - ball.width)){
            ballIsMoving = false;
            ballMoveStraight = true;
            playerBoard.y = boardCenterY;
            cpuBoard.y = boardCenterY;
            ball.x = ballCenterX;
            ball.y = ballCenterY;
            cpuScore += 1;
        }
        if(ball.x < 0){
            ballIsMoving = false;
            ballMoveStraight = true;
            playerBoard.y = boardCenterY;
            cpuBoard.y = boardCenterY;
            ball.x = ballCenterX;
            ball.y = ballCenterY;
            playerScore += 1;
        }

        // y axis bounching calculation
        if(ball.y > (WIN_H - ball.height)){
            ballVelY = -ballVelY;
        }
        if(ball.y < 0){
            ballVelY = -ballVelY;
        }

        // board & ball colision
        if(CheckCollisionRecs(playerBoard, ball)){
            if(ballVelX > 0){
                ballVelX = -ballVelX;
            }
            ballMoveStraight = false;   
        }
        if(CheckCollisionRecs(cpuBoard, ball)){
            if(ballVelX < 0){
                ballVelX = -ballVelX;
            }
            ballMoveStraight = false;
        }

        // Draw 
        BeginDrawing();
            ClearBackground(BLACK);

            // draw game object
            DrawRectangle(playerBoard.x, playerBoard.y, playerBoard.width, playerBoard.height, gameObjectColor);
            DrawRectangle(cpuBoard.x, cpuBoard.y, cpuBoard.width, cpuBoard.height, gameObjectColor);
            DrawRectangle(ball.x, ball.y, ball.width, ball.height, gameObjectColor);

            // draw middle dash lines
            for(int i=0; i<dlCount; i++){
                // Calculate Y based on 'i' so it's always correct every frame
                int currentY = i * (dlH + dlGap); 
                DrawRectangle(dlPosX, currentY, dlW, dlH, gameObjectColor);
            }

            // draw cpu & player score
            DrawText(TextFormat("%d" ,cpuScore), 300, 10, scoreFontSize, gameObjectColor);
            DrawText(TextFormat("%d", playerScore), (500-20), 10, scoreFontSize, gameObjectColor);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}