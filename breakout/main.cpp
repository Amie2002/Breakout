//
//  main.cpp
//  RaylibExample
//
//  Created by Aditi on 06/08/24.
//

// main.cpp
#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
#include <ctime>

const int screenWidth=500;
const int screenHeight=600;

struct Ball {
    Vector2 pos;
    Vector2 acceleration{1.0f, 1.0f};
    float velocity;
    float radius=5.0f;
};

struct Brick {
    Rectangle rect;
    Color color;
    float w=50.0f;
    float h=20.0f;
};

struct Player {
    Rectangle rect;
    float velocity=250.0f;
    int score=0;
    float w=75.0f;
    float h=10.0f;
};

Texture2D textBackground;
Player player;
Ball ball;

std::vector<Brick> bricks;
Sound fxBrickHit[2];
Sound fxPaddleHit;

void GameStartup(){
    //loading background image
    std::string filePath = "/Users/aditi/Documents/my-c++_codes/brickout/breakout/assets/background2.png";
    Image imgBackground=LoadImage(filePath.c_str());
    textBackground=LoadTextureFromImage(imgBackground);
    UnloadImage(imgBackground);
    
    //loading player paddle data
    player.rect=Rectangle{250.0f,540.0f, player.w, player.h};
    player.score=0;
    
    //load ball data
    ball.pos=Vector2({300,300});
    ball.velocity=300.0f;
    
    //load brick data
    std::vector<Color> COLORS{ORANGE,PINK,MAGENTA,BLUE,DARKBLUE,DARKPURPLE};
    
    Brick newBrick;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            newBrick.rect=Rectangle({
                float(40+(i*55)),
                float(50+(j*26)),
                newBrick.w,
                newBrick.h
            });
            newBrick.color=COLORS[rand()%COLORS.size()];
            bricks.push_back(newBrick);
        }
    }
    std::string soundPathbrick1 = "/Users/aditi/Documents/my-c++_codes/brickout/breakout/assets/brick-hit-1.wav";
    std::string soundPathbrick2 = "/Users/aditi/Documents/my-c++_codes/brickout/breakout/assets/brick-hit-2.wav";
    std::string soundPathpaddle = "/Users/aditi/Documents/my-c++_codes/brickout/breakout/assets/paddle-hit-1.wav";
    fxBrickHit[0]=LoadSound(soundPathbrick1.c_str());
    fxBrickHit[1]=LoadSound(soundPathbrick2.c_str());
    fxPaddleHit=LoadSound(soundPathpaddle.c_str());
}

void GameUpdate(){
    float frameTime=GetFrameTime();
    
    //update player position
    if (IsKeyDown(KEY_LEFT)){
        player.rect.x-=player.velocity*frameTime;
    }
    if (IsKeyDown(KEY_RIGHT)){
        player.rect.x=player.rect.x+(player.velocity*frameTime);
    }
    
    //update ball position
    ball.pos.x+=(ball.velocity*ball.acceleration.x)*frameTime;
    ball.pos.y+=(ball.velocity*ball.acceleration.y)*frameTime;
    
    //check collision for each brick and ball
    Brick brick;
    int randomnu;
    for (int i=0; i<bricks.size(); i++) {
        brick=bricks[i];
        if(CheckCollisionCircleRec(ball.pos, ball.radius, brick.rect)){
            ball.acceleration.y= - ball.acceleration.y;
            bricks.erase(bricks.begin()+i);
            player.score=player.score+10;
            randomnu = rand()%2;
            PlaySound(fxBrickHit[randomnu]);
            break;
        }
    }
    
    
    //check collision for wall and ball
    if (ball.pos.x>screenWidth || ball.pos.x<10){
        ball.acceleration.x= -ball.acceleration.x;
    }
    if (ball.pos.y>screenHeight || ball.pos.y<10){
        ball.acceleration.y= -ball.acceleration.y;
    }
    
    //check collision for wall and player
    if (player.rect.x<0){
        player.rect.x=0;
    }
    if (player.rect.x>(screenWidth-player.rect.width)){
        player.rect.x=(screenWidth-player.rect.width);
    }
    
    //check collision for ball and player
    if (CheckCollisionCircleRec(ball.pos, ball.radius, player.rect)){
        ball.acceleration.x= -ball.acceleration.x;
        ball.acceleration.y= -ball.acceleration.y;
        PlaySound(fxPaddleHit);
    }
    
}

void GameRender(){
    //render background
    DrawTexture(textBackground, 0, 0, RAYWHITE);
    
    //render bricks
    Brick brick;
    for (int i=0; i<bricks.size(); i++) {
        brick = bricks[i];
        DrawRectangle(brick.rect.x, brick.rect.y, brick.rect.width, brick.rect.height, brick.color);
    }
    
    //render ball
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, RAYWHITE);
    
    //render player
    DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height, PURPLE);
    
    //render score
    std::string Score=std::to_string(player.score);
    std::string templ="SCORE : ";
    std::string temp2=templ+Score;
    char const* text=temp2.c_str();
    DrawText(text, 10, 10, 30, PURPLE);
}
void GameShutdown(){
    UnloadSound(fxBrickHit[0]);
    UnloadSound(fxBrickHit[1]);
    UnloadSound(fxPaddleHit);
    
    UnloadTexture(textBackground);
    
    CloseAudioDevice();
}

int main() {
    
    srand(static_cast<unsigned int>(time(NULL)));
    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Raylib Template");
    InitAudioDevice();
    
    // Set the target frame rate
    SetTargetFPS(60);
    GameStartup();

    // Main game loop
    while (!WindowShouldClose()) {
        // Draw a red square
        GameUpdate();
        
        BeginDrawing();
        ClearBackground(GRAY);
        
        GameRender();
        
        EndDrawing();
    }
    
    GameShutdown();

    // De-initialize the window
    CloseWindow();

    return 0;
}
