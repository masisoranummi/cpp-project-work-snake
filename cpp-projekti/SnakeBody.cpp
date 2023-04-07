#include "SnakeBody.h"

// Tehdään null seuravaasta koska bodyt
// luodaan aina siten että ne ovat viimeisenä jonossa
SnakeBody::SnakeBody(int x, int y) {
    positionX = x;
    positionY = y;
    next = nullptr; 
}

// liikutetaan bodya
void SnakeBody::move(int x, int y) {
    positionX = x;
    positionY = y;
}

// jos ei ole viimeinen "jonossa"
// kutsutaan seuraavan osan tätä funktiota
void SnakeBody::moveNext(int x, int y) {            
    if (next != nullptr) {
        next->moveNext(positionX, positionY); 
    }
    move(x, y);  
}

// pistetään seuraavaksi snakebody-osa
void SnakeBody::setNext(SnakeBody* next){
    this->next = next;
}

// getterit x ja y arvoille
int SnakeBody::getX(){
    return positionX;
}

int SnakeBody::getY(){
    return positionY;
}

// pistetään bodyn merkki screen-taulukkoon ja kutsutaan seuraavan
// body-osan paint-funktiota
void SnakeBody::paint(int screenWidth, int screenHeight, wchar_t* screen){
    screen[positionY * screenWidth + positionX] = L'O';
    if(next != nullptr){
        next->paint(screenWidth, screenHeight, screen);
    }
}

