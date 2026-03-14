#ifndef CONSTANTS
#define CONSTANTS

struct ball{
    float positionX;
    float positionY;

    float velocityX;
    float velocityY;

    ball(float x, float y, float vX, float vY){
        positionX = x;
        positionY = y;

        velocityX = vX;
        velocityY = vY;
    }
};

struct paddle{
    float positionX;
    float positionY;

    paddle(float x, float y){
        positionX = x;
        positionY = y;
    }
};

inline constexpr const int screenWidth = 480;
inline constexpr const int screenHeight = 360;
inline constexpr const int paddleLength = 50;
inline constexpr const int paddleWidth = 10;
inline constexpr const float ballRadius = 10.0f;

#endif