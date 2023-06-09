#ifndef SNAKEHEAD_H
#define SNAKEHEAD_H

#include "SnakeBody.h"
#include "SnakePart.h"

class SnakeHead: public SnakePart {
public:
    SnakeHead(int screenWidth, int screenHeight, wchar_t* screen);
    void move();
    void changeDirection(int newDirection);
    int checkCollision();
    void setNext(SnakeBody* next);
    void addBodyPart();
    int getYDir();
    int getXDir();
    void paint();

private:
    int screenWidth;
    int screenHeight;
    wchar_t* screen;
    int direction;
    SnakeBody* body;
    SnakeBody* tail;
};

#endif 
