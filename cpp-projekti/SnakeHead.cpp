#include "SnakeHead.h"
#include <string.h>
#include <iostream>

#define LEFT 1
#define UP  2
#define RIGHT 3
#define DOWN 0

// SnakeHead on se mitä pelaaja liikuttaa, ja muu keho seuraa päätä
SnakeHead::SnakeHead(int screenWidth, int screenHeight, wchar_t* screen) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->screen = screen;
    positionX = 30;
    positionY = 10;
    direction = LEFT;
    body = nullptr;
    tail = nullptr;
}

// liikkuu suuntaan direction-arvon mukaan
// jos SnakeHeadilta löytyy body-arvo
// liikutetaan se sijaintiin jossa pää oli viimeksi
void SnakeHead::move() {
    if (body != nullptr) {
        body->moveNext(positionX, positionY);
    }
    //move head based on direcion and speed
    switch (direction)
    {
    case LEFT:
        positionX -= 1;
        break;
    
    case RIGHT:
        positionX += 1;
        break;

    case UP:
        positionY -= 1;
        break;

    case DOWN:
        positionY += 1;
        break;
    }
}

// muuttaa suuntaa getInputista saadun inputin mukaan
void SnakeHead::changeDirection(int newDirection) {
    switch (newDirection)
    {
    case 1:
        if (direction != RIGHT) direction = LEFT;
        break;

    case 2:
        if (direction != DOWN) direction = UP;
        break;

    case 3:
        if (direction != LEFT) direction = RIGHT;
        break;

    case 4:
        if (direction != UP) direction = DOWN;
        break;
    }
}

// palauttaa arvon riippuen mihin
// suuntaan pää liikkuu y-akselilla
int SnakeHead::getYDir(){
    if(direction==UP)
    {
        return -1;
    } 
    else if(direction==DOWN)
    {
        return 1;
    }
    else return 0;
}

// sama mutta katsotaan x-akselia
int SnakeHead::getXDir(){
    if(direction==LEFT)
    {
        return -1;
    } 
    else if(direction==RIGHT)
    {
        return 1;
    }
    else return 0;
}

// annetaan snakeHeadille body-arvo
void SnakeHead::setNext(SnakeBody* next){
    body = next;
}

// katsotaan törmääkö SnakeHead mihinkään direction-arvon perusteella
// ja palautetaan numero sen perusteella mihin se törmää
int SnakeHead::checkCollision() {
    int collision = 0;
    // tässä käytetään x ja y suunat-arvoja, joiden avulla
    // katsotaan onko seuraavassa sijainnissa mitään esteitä
    int xDirection = getXDir();
    int yDirection = getYDir();
    if (positionX <= 1 || positionX >= screenWidth-2 || positionY <= 1 || positionY >= screenHeight - 2) 
    {
        // osu seinään
        collision = 1;
    }
    else if (screen[((positionY + yDirection) * screenWidth) + (positionX + xDirection)]== L'O')
    {
        // osu itseensä
        collision = 2;
    }
    else if (screen[((positionY + yDirection) * screenWidth) + (positionX + xDirection)]== L'*')
    {
        // osui hedelmään
        collision = 3;
    }
    
    return collision;
}

// lisätään Snakebody viimeisimmän perään
// tail-arvon avulla
void SnakeHead::addBodyPart() {
    if (tail == nullptr) {
        tail = new SnakeBody(positionX, positionY);
        body = tail;
    } else {
        SnakeBody* newTail = new SnakeBody(tail->getX(), tail->getY());
        tail->setNext(newTail);
        tail = newTail;
    }
}

// kirjoittaa screen-taulukkoon O-merkin pään sijainnin mukaan
// jonka jälkeen kutsuu bodyn paint-funktiota (jos on body)
void SnakeHead::paint(){
    screen[positionY * screenWidth + positionX] = L'O';
    if(body != nullptr){
        body->paint(screenWidth, screenHeight, screen);
    }
}
