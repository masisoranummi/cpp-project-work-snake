#include <iostream>
#include <Windows.h>
#include "SnakeHead.h"
#include "SnakeBody.h"
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// määritellään ruudunkoko ja buffer johon kirjoitetaan
// merkit jotka halutaan näkyvän konsolissa
// käytän yksiulotteista taulukkoa kaksiulotteisen sijaan
// koska silloin elementteihin pääsee helpommin käsiksi
// ja halusin koittaa pystyykö sillä tekemään tässä tapauksessa samat asiat kuin kaksiulotteisella
// (kaksiulotteisten taulukoitten kanssa on muutenkin pelleilty ihan tarpeeksi tässä opiskelun yhteydessä)
const int nScreenWidth = 60;
const int nScreenHeight = 20;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];

DWORD dwBytesWritten = 0;
INPUT_RECORD irInBuf[128];
DWORD nEventsRead = 0;
boolean end = false;

void drawMainMenu(wchar_t screen[nScreenHeight * nScreenWidth]);
void writeMessage(wchar_t screen[nScreenHeight * nScreenWidth], std::wstring message, int row);
void startGame(wchar_t screen[nScreenHeight * nScreenWidth], HANDLE hConsole);
void write(HANDLE hConsole);
void clearScreen();
void getInput(SnakeHead &player);
void printApple(int *fruitPosition);
void recordScore(int points, HANDLE hConsole);
void getPlayerName(std::wstring &playerName, HANDLE hConsole);
void writeToFile(int points, std::wstring playerName);
void sortScores();
void showHighScores(HANDLE hConsole);
bool listenForInput(char c);

int main()
{
    // nämä tässä liittyvät inputin saamiseen,
    // en rehellisesti sanottuna osaa tarkallen mitä kaikki nämä tekevät
    // mutta muutaman tunnin googlettelun jälkeen päädyin käyttämään näitä
    // inputin saamiseen koska se toimi kuten halusin
    // nämä toistuvat täällä muutamaan otteeseen, koska en
    // saanut kaikkia funktioita toimimaan haluamallani tavalla
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsoleIn, &dwMode);
    SetConsoleMode(hConsoleIn, dwMode & ~ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);

    // Tekee konsolista oikean kokoisen
    SMALL_RECT windowSize = {0, 0, 59, 19};
    SetConsoleWindowInfo(hConsoleOut, TRUE, &windowSize);


    // Tämän avulla tehdään uusi konsolinäkymä, joka toimii paremmin tässä projektissa
    // perinteisen konsolin sijaan
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    // Pistetään reunat #-merkeiksi
    // käytän tässä sovelluksen toteutuksessa suurimmaksi osaksi
    // wide-merkkejä (wchar_t ja wstring), koska tarkoitus oli 
    // alunperin käyttää unicode merkkejä, mutta command prompt
    // ei niistä hirveästi tykännyt, niin päädyin takaisin normimerkkeihin
    // jos joskus haluaa kuitenkin selvittää miten ne saisi toimivaan
    // ei (toivottavasti) tarvitse  hirveästi säätää ainakaan näiden kanssa
    const wchar_t borderChar = L'#';
    for (int x = 0; x < nScreenWidth; x++) {
        screen[0 * nScreenWidth + x] = borderChar;              
        screen[(nScreenHeight-1) * nScreenWidth + x] = borderChar; 
    }
    for (int y = 1; y < nScreenHeight-1; y++) {
        screen[y * nScreenWidth + 0] = borderChar;              
        screen[y * nScreenWidth + nScreenWidth-1] = borderChar; 
    }



    while (!end)
    {
        drawMainMenu(screen);

        write(hConsole);

        // odotetaan sekunti
        Sleep(1000);

        // toivoin että olisin pystyny käyttään tuota listenforinput
        // funktiota näin, mutta se ei näytä toimivan monen inputin kanssa
        // if (listenForInput('X'))
        // {
        //     startGame(screen, hConsole);
        // }
        // if (listenForInput('Z'))
        // {
        //     showHighScores(hConsole);
        // }
        // if (listenForInput('C'))
        // {
        //     end = true;
        // }
        
        // näitten ideana on, että kuunnellaan inputtia
        // ja käydään ne kaikki läpi, ja jos inputeista 
        // löytyy x z tai c, tehdään eri asioita
        DWORD dwEvents = 0;
        GetNumberOfConsoleInputEvents(hConsoleIn, &dwEvents);
        if (dwEvents > 0)
        {
            ReadConsoleInput(hConsoleIn, irInBuf, 128, &nEventsRead);
            for (int i = 0; i < nEventsRead; i++)
            {
                if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
                {
                    if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == 'X')
                    {
                        startGame(screen, hConsole);
                    }
                    else if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == 'Z')
                    {
                        showHighScores(hConsole);
                    }
                    else if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == 'C')
                    {
                        end = true;
                    }
                    
                }
            }
        }
        
    }
    return 0;
}

//piirretään mainmenun ulkoasu screen-bufferiin
void drawMainMenu(wchar_t screen[nScreenHeight * nScreenWidth]){
    // for-loopin avulla päivitetään ruutua sekunnin välein, lisätään randomisti x-merkkejä 
    // ja tehdään muista elementeistä "tyhjät" välilyönneillä 
    for (int y = 1; y < nScreenHeight-1; y++) {
        for (int x = 1; x < nScreenWidth-1; x++) {
            if((y < 4 || y > 15 || x < 11 || x > 49) && rand() % 10 == 5) {
                screen[y * nScreenWidth + x] = L'X';
            } else {
                screen[y * nScreenWidth + x] = L' ';
            }
        }
    }
    // käytetään writeMessage-funktiota main menun kirjoittamiseen
    writeMessage(screen, L"Snake", 5);
    writeMessage(screen, L"By Masi Soranummi", 7);
    writeMessage(screen, L"Press [x] to start game", 10);
    writeMessage(screen, L"Press [z] to see high scores", 12);
    writeMessage(screen, L"Press [c] to exit game", 14);
}

// writeMessage-funktion ideana on vähentää koodintoistoa, 
// jolloin pystyn kirjoittamaan screen-taulukkoon haluamalleni
// riville, haluamani merkkijonon. Kirjoittaa kaiken keskitettynä taulukkoon
// eikä kirjoita mitään itse ruudulle, siihen tarvii kutsua write()-funktiota
void writeMessage(wchar_t screen[nScreenHeight * nScreenWidth], std::wstring message, int row){
    int messageLength = message.length();
    int messageStart = (nScreenWidth - messageLength) / 2;
    for (int i = 0; i < messageLength; i++) { 
        screen[row * nScreenWidth + messageStart + i] = message[i];
    }
}

// kutsutaan kun tarvitsee ottaa kaikki muut paitsi reunat
// pois bufferista
void clearScreen(){
    for (int y = 1; y < nScreenHeight-1; y++) {
        for (int x = 1; x < nScreenWidth-1; x++) {
            screen[y * nScreenWidth + x] = L' ';
        }
    }
}

//itse matopelin pyörättiminen tapahtuu täällä funktiossa
void startGame(wchar_t screen[nScreenHeight * nScreenWidth], HANDLE hConsole){
    // ensin kirjoitetaan ohjeet ruudulle
    clearScreen();
    writeMessage(screen, L"Use the arrow keys to change direction", 8);
    write(hConsole);
    Sleep(2000);
    writeMessage(screen, L"Collect Fruit to gain points", 10);
    write(hConsole);
    Sleep(2000);
    writeMessage(screen, L"Starting in 3", 11);
    write(hConsole);
    Sleep(1000);
    writeMessage(screen, L"Starting in 2", 11);
    write(hConsole);
    Sleep(1000);
    writeMessage(screen, L"Starting in 1", 11);
    write(hConsole);
    Sleep(1000);
    clearScreen();
    write(hConsole);
    // itse peli alkaa 
    int points = 0;
    // ensin oli vain yksi nopeusarvo
    // mutta koska merkit eivät ole pituudelta ja leveydeltään täysin samanmittaiset
    // nopeus on nopeampi jos liikutaan sivulta sivulle
    int speed  = 100;
    int hSpeed = 60;
    // tehdään uusi snakehead-olio jota pelaaja liikuttaa
    // ja jolle lisätään snakebody-olio joka seuraa snakehead-oliota
    SnakeHead player = SnakeHead(nScreenWidth, nScreenHeight, screen);
    player.addBodyPart();
    boolean alive = true;
    std::wstring message = L"";
    // "hedelmän" aloituspaikka
    int fruitPosition = 5 * nScreenWidth + 5;
    // looppi joka pyörii niin kauan kunnes pelaaja törmää
    // seinään tai itseensä
    while (alive)
    {
        // otetaaan kaikki pois screen-taulukosta (paitsi reunat)
        // ja piiretään hedelmä ja pelaajaa
        clearScreen();
        screen[fruitPosition] = L'*';
        player.paint();
        getInput(player);
        // katsotaan törmäsikö pelaaja johonkin 
        // jos ei, liikutetaan pelaajaa
        int result = player.checkCollision();
        if(result == 0){
            player.move();
            player.paint();
            write(hConsole);
        } 
        // jos osuu seinään tai itseensä lopetetaan looppi
        else if (result==1)
        {
            message = L"You ran into a wall";
            alive = false;
        } 
        else if (result==2)
        {
            message = L"You collided with yourself";
            alive = false;
        }
        // jos edessä on hedelmä, lisätään snakebody-olio
        // "jonon perään", vaihdetaan omenan paikkaa, nostetaan nopeutta
        // ja liikutetaan pelaajaa
        else if (result==3)
        {
            player.addBodyPart();
            printApple(&fruitPosition);
            points++;
            if(speed > 10){
                speed -= 5;
                hSpeed -= 3;
            }
            player.move();
            player.paint();
            write(hConsole);
        }
        // katsotaan liikkuuko pelaaja x- vai y-suunnassa
        // ja katsotaan sen mukaan paljon odotetaan ennen
        // seuraavaa looppia
        if(player.getXDir() != 0){
            Sleep(hSpeed);
        } else {
            Sleep(speed);
        }
    }
    Sleep(2000);
    clearScreen();
    //pistetään häviämisen syy ja pisteet ruudulle
    writeMessage(screen, message, 8);
    writeMessage(screen, L"Points: " + std::to_wstring(points), 10);
    write(hConsole);
    Sleep(2000);
    // tallennetaan tulos
    recordScore(points, hConsole);
}

// kirjoittaa kaiken tällä hetkellä screen-taulukosta löytyvän
// konsoliin
void write(HANDLE hConsole){
    screen[nScreenWidth * nScreenHeight - 1] = L'\0';
    WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
}

// käytetään pelaajan inputin hakemisen jonka mukaan käännetään 
// mato oikeaan suuntaan
// vähän koodintoistoa noiden inputtien kanssa valitettavasti
// oisin halunnut käyttää listenforinput näihin kaikkiin inputteihin
// mutta en saanut sitä oikein toimimaan kuuntelemaan montaa eri inputtia kerralla
void getInput(SnakeHead &player){
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsoleIn, &dwMode);
    SetConsoleMode(hConsoleIn, dwMode & ~ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    DWORD dwEvents = 0;
    GetNumberOfConsoleInputEvents(hConsoleIn, &dwEvents);
    if (dwEvents > 0)
    {
        ReadConsoleInput(hConsoleIn, irInBuf, 128, &nEventsRead);
        for (int i = 0; i < nEventsRead; i++)
        {
            if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
            {
                if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
                {
                    player.changeDirection(2);
                    break;
                } 
                else if(irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) 
                {
                    player.changeDirection(3);
                    break;
                }
                else if(irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN) 
                {
                    player.changeDirection(4);
                    break;
                }
                else if(irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_LEFT) 
                {
                    player.changeDirection(1);
                    break;
                }
            }
        }
    }
}  

// kutsutaan kun hedelmä on "napattu"
void printApple(int *fruitPosition){
    // edellisen omenan tilalle laitetaan tyhjä paikka
    screen[*fruitPosition] = L' ';
    // etsitään uusi paikka, katsotaan samalla, että pelaaja ei ole siinä
    *fruitPosition = (rand() % 15 + 3) * nScreenWidth + (rand() % 55 + 3);
    while(screen[*fruitPosition] != L' '){
        *fruitPosition = (rand() % 15 + 3) * nScreenWidth + (rand() % 55 + 3);
    }
    // pistetään hedelmä uudelle paikalle
    screen[*fruitPosition] = L'*'; 
}

// kysytään pelaajan nimeä jolloin se voidaan tallentaa tuloksiin
void recordScore(int points, HANDLE hConsole){
    clearScreen();
    writeMessage(screen, L"Please input your name: ", 8);
    write(hConsole);
    std::wstring name = L"";
    //kysytään pelaajalta nimeä ja tallennetaan se scores.txt tiedostoon
    getPlayerName(name, hConsole);
    writeToFile(points, name);
    Sleep(2000);
    clearScreen();
    writeMessage(screen, L"Score saved!", 8);
    write(hConsole);
    //pistetään tulokset scores.txt järjestykseen pisteiden mukaan
    sortScores();
    Sleep(2000);
    // näytetään 5 korkeinta tulosta
    showHighScores(hConsole);
}

//kysytään pelaajan nimeä, jälleen koodintoistoa näiden kanssa
void getPlayerName(std::wstring &playerName, HANDLE hConsole) {
    bool done = false;
    while(!done){
        HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
        HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hConsoleIn, &dwMode);
        SetConsoleMode(hConsoleIn, dwMode & ~ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsoleOut, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
        DWORD dwEvents = 0;
        GetNumberOfConsoleInputEvents(hConsoleIn, &dwEvents);
        if (dwEvents > 0)
        {
            ReadConsoleInput(hConsoleIn, irInBuf, 128, &nEventsRead);
            for (int i = 0; i < nEventsRead; i++)
            {
                if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
                {
                    wchar_t c = irInBuf[i].Event.KeyEvent.uChar.UnicodeChar;
                    // jos pelaajan kirjoittama kirjain on 
                    // enter --> poistutaan loopista
                    // backspace --> poistetaan merkkijonosta
                    // joku muu kirjain --> lisätään merkkijonoon
                    if (c == '\b' && playerName.length() != 0)
                    {
                        std::wstring temp = playerName.substr(0,playerName.length()-1);
                        playerName = temp;
                        clearScreen();
                        writeMessage(screen, L"Please input your name: ", 8);
                    }
                    else if (c != '\r') { 
                        playerName += c;
                    } else {
                        done = true;
                    }
                }
            }
        }
        writeMessage(screen, playerName, 9);
        write(hConsole);
        Sleep(50);
    }
}

// lisätään pelaajan tulos tekstitiedoston loppuun
void writeToFile(int points, std::wstring playerName){
    std::wofstream file(L"scores.txt", std::ofstream::app);
    file << std::to_wstring(points) << L","<< playerName << L"\n";
    file.close();
}

// struct johon tallennetaan tulos ja nimi, operator()-funktiota
// käytetään tulosteen sorttaamiseen, ja tulosten ruudulle kirjoittamiseen
struct Score {
    int score;
    std::wstring name;
    bool operator() (const Score &a, const Score &b) { return a.score > b.score;}
};


void sortScores(){
    // luetaan tulokset tekstitiedostosta ja tallennetaan
    // ne Score-tyyppisinä vectoriin, joka on vähänkuin taulukko,
    // mutta sen kokoa ei tarvi määritellä alussa
    std::vector<Score> scores;
    std::wifstream infile("scores.txt");
    int score;
    std::wstring name;
    while (infile >> score >> name) {
        scores.push_back({score, name});
    }

    // järjestetään scores-vectorin Scoret
    // järjestykseen suurimmasta pienimpään Scoren
    // operator-funktiota hyödyksi käyttäen
    std::sort(scores.begin(), scores.end(), Score());

    // kirjoitetaan loopilla kaikki tulokset takaisin tiedostoon
    // sorttauksen jälkeen
    std::wofstream outfile("scores.txt");
    for (const Score &s : scores) {
        outfile << s.score << s.name << L"\n";
    }
}

// Luetaan tulokset tiedostosta ja kirjoitetaan viisi
// parasta tulosta ruudulle
void showHighScores(HANDLE hConsole){
    // luetaan taas tulokset ja pistetään ne
    // Score-muodossa vectoriin
    std::vector<Score> scores;
    std::wifstream infile("scores.txt");
    int score;
    std::wstring name;
    while (infile >> score >> name) {
        //poistetaan pilkku välistä
        name.erase(0,1);
        scores.push_back({score, name});
    }
    clearScreen();
    writeMessage(screen, L"Top 5 scores: ", 5);
    // koska tulokset on jo sortattuna, kirjoitetaan
    // viisi ensimmäistä tulosta ruudulle,
    // jos tuloksia on vähemmän kuin 5, min katsoo
    // että kirjoitetaan kaikki tulokset ruudulle
    for (int i = 0; i < std::min(5, (int)scores.size()); i++) {
        std::wstring message = L"Score: " + std::to_wstring(scores[i].score) + L" Name: " + scores[i].name;
        writeMessage(screen, message, 7+i);
    }
    writeMessage(screen, L"Press 'x' to go back", 13);
    write(hConsole);
    // odotetaan inputtia
    while(!listenForInput('X')){
        Sleep(50);
    }
}

// tosiaan tarkoitus oli tätä olisi käytetty kaikkiin inputteihin,
// jotta olisi vähemmän koodintoistoa, mutta en saanut sitä toimimaan
// tilanteissa jossa kuunnellaan montaa inputtia kerralla
// jos joskus palaan tähän projektiin niin tämä olisi varmaan ensimmäinen asia mitä
// yrittäisin korjata
bool listenForInput(char c){
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsoleIn, &dwMode);
    SetConsoleMode(hConsoleIn, dwMode & ~ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    DWORD dwEvents = 0;
    GetNumberOfConsoleInputEvents(hConsoleIn, &dwEvents);
    if (dwEvents > 0)
    {
        ReadConsoleInput(hConsoleIn, irInBuf, 128, &nEventsRead);
        for (int i = 0; i < nEventsRead; i++)
        {
            if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == c){
                return true;
            }
        }
    }
    return false;
}