// linker::system::subsystem - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "cmath"
#include <string>

// секция данных игры  
typedef struct {
    float x, y, width, height, rad, dx, dy, speed;
    int room, hp;
    HBITMAP hBitmap;//хэндл к спрайту
} sprite;

sprite oven;
sprite stove;
sprite fridge;
sprite hero;
sprite statue1;
sprite statue2;
sprite statue3;
sprite table;
sprite enemy;
sprite table2;
sprite man;
sprite pool;
sprite crane;
sprite icestatue;
sprite box13;
sprite aquaman; // enemy1
sprite skeleton; // enemy2
sprite box2;

struct item {
    float x = -100;
    float y = -100;
    int room;
    float width = 70;
    float height = 70;
    int picked = 0;
    HBITMAP hBitmap;//хэндл к спрайту
};

item halva;
item key;
item waffle;

struct {
    int score, balls;//количество набранных очков и оставшихся "жизней"
    bool action = false;//состояние - ожидание (игрок должен нажать пробел) или игра
} game;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна, которое создаст программа
    // x, y for third location;
    int x = 1200;
    int y = -1000;
    int x2 = -1000;
} window;

HBITMAP hBack;// хэндл для фонового изображения
HBITMAP hStove;
HBITMAP hCloset;
HBITMAP hIceroom;
HBITMAP hCloset1;
HBITMAP hCloset2;
HBITMAP hCoridor;
HBITMAP hCoridor2;
HBITMAP hOven;
int g = 0;
bool jumping = false;
DWORD currentTime, lastTime;

void setText() {
    SetTextColor(window.context, RGB(200, 200, 250));
    //SetBkColor(window.context, RGB(0, 0, 0));
    SetBkMode(window.context, TRANSPARENT);
    auto hFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 10, 0, "CALIBRI");
    auto hTmp = (HFONT)SelectObject(window.context, hFont);
}

//cекция кода

void InitGame()
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов
    oven.hBitmap = (HBITMAP)LoadImageA(NULL, "oven.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    stove.hBitmap = (HBITMAP)LoadImageA(NULL, "stove.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    fridge.hBitmap = (HBITMAP)LoadImageA(NULL, "fridge.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hero.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    table.hBitmap = (HBITMAP)LoadImageA(NULL, "table.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue1.hBitmap = (HBITMAP)LoadImageA(NULL, "statue1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue2.hBitmap = (HBITMAP)LoadImageA(NULL, "statue2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue3.hBitmap = (HBITMAP)LoadImageA(NULL, "statue3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    table2.hBitmap = (HBITMAP)LoadImageA(NULL, "table.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    man.hBitmap = (HBITMAP)LoadImageA(NULL, "statue2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    pool.hBitmap = (HBITMAP)LoadImageA(NULL, "pool.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    halva.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    key.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    waffle.hBitmap = (HBITMAP)LoadImageA(NULL, "stove.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    aquaman.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    skeleton.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hStove = (HBITMAP)LoadImageA(NULL, "locfive.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCloset = (HBITMAP)LoadImageA(NULL, "locfour.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "marblefloor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hIceroom = (HBITMAP)LoadImageA(NULL, "iceroom.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCloset1 = (HBITMAP)LoadImageA(NULL, "closet1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCloset2 = (HBITMAP)LoadImageA(NULL, "closet2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCoridor2 = (HBITMAP)LoadImageA(NULL, "coridor2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hOven = (HBITMAP)LoadImageA(NULL, "openoven.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    box13.hBitmap = (HBITMAP)LoadImageA(NULL, "oven.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    fridge.width = 200;
    fridge.height = 200;
    fridge.x = 0;
    fridge.y = 0;

    stove.width = 200;
    stove.height = 200;
    stove.x = 200;
    stove.y = 0;

    oven.width = 200;
    oven.height = 200;
    oven.x = 400;
    oven.y = 0;

    table.width = 600;
    table.height = 300;
    table.x = window.width / 2 - 300;
    table.y = window.height - 300;

    statue1.width = 150;
    statue1.height = 150;
    statue1.x = window.width - 200;
    statue1.y = 0;

    statue3.width = 200;
    statue3.height = 400;
    statue3.x = 0;
    statue3.y = window.height - 400;

    statue2.width = 100;
    statue2.height = 300;
    statue2.x = window.width - 100;
    statue2.y = window.height - 400;

    hero.x = window.width / 4;
    hero.y = window.height / 3 + 20;
    hero.height = 150;
    hero.width = 150;
    hero.speed = 10;
    hero.rad = 75;
    hero.room = 1;
    hero.hp = 51;

    enemy.x = window.width / 4 * 3 - 30;
    enemy.y = window.height - 200;
    enemy.speed = 5;
    enemy.height = 150;
    enemy.width = 150;

    table2.x = window.width / 4;
    table2.y = window.height / 3;
    table2.width = window.width / 2;
    table2.height = window.height / 3;

    man.x = window.width / 4 - 150;
    man.y = window.height / 2;
    man.width = 150;
    man.height = 200;
    man.speed = 6;
    man.hp = 20;

    pool.x = 200;
    pool.y = 200;
    pool.width = window.width - 600;
    pool.height = window.height - 200;

    box13.x = 100;
    box13.y = window.height - 170;
    box13.height = 150;
    box13.width = 150;

    aquaman.x = 100;
    aquaman.y = 100;
    aquaman.width = 150;
    aquaman.height = 150;
    aquaman.hp = 20;
    aquaman.speed = 8;

    skeleton.x = 100;
    skeleton.y = 100;
    skeleton.width = 150;
    skeleton.height = 150;
    skeleton.hp = 15;
    skeleton.speed = 6;

    halva.room = 1;
    halva.x = 767;
    halva.y = 456;

    key.room = 3;
    key.x = 1500;
    key.y = 200;

    waffle.room = 7;
    waffle.x = 111;
    waffle.y = 111;

    game.score = 0;
}

struct Pos {
    int x;
    int y;
};

POINT p;
void mouseInput() {
    GetCursorPos(&p);
    ScreenToClient(window.hWnd, &p);
    //GetAsyncKeyState(VK_LBUTTON);
    /*struct Mouse {
        Pos pos;

        void Input()
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(window.hWnd, &p);
            pos = { p.x, p.y };
        }
    };*/
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // Создаем контекст памяти, совместимый с контекстом отображения
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// Выбираем изображение bitmap в контекст памяти

    if (hOldbm) // Если не было ошибок, продолжаем работу
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // Определяем размеры изображения

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//все пиксели черного цвета будут интепретированы как прозрачные
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // Рисуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// Восстанавливаем контекст памяти
    }

    DeleteDC(hMemDC); // Удаляем контекст памяти
}

void tutorial() {

    if(currentTime - lastTime <= 3000) {
        TextOutA(window.context, window.width / 2, 200, "feed a man", 10);
        TextOutA(window.context, window.width / 2, 400, "e to eat", 8);
        TextOutA(window.context, window.width / 2, 600, "t to take", 9);
        TextOutA(window.context, window.width / 2, 800, "g to pick", 9);
        TextOutA(window.context, window.width / 2, 1000, "u to use", 8);
    }
    
}

void LocOne()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//задний фон
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, oven.x, oven.y, oven.width, oven.height, oven.hBitmap);
    ShowBitmap(window.context, stove.x, stove.y, stove.width, stove.height, stove.hBitmap);
    ShowBitmap(window.context, fridge.x, fridge.y, fridge.width, fridge.height, fridge.hBitmap);
    ShowBitmap(window.context, table.x, table.y, table.width, table.height, table.hBitmap);
    ShowBitmap(window.context, statue1.x, statue1.y, statue1.width, statue1.height, statue1.hBitmap);
    ShowBitmap(window.context, statue2.x, statue2.y, statue2.width, statue2.height, statue2.hBitmap);
    ShowBitmap(window.context, statue3.x, statue3.y, statue3.width, statue3.height, statue3.hBitmap);
    ShowBitmap(window.context, enemy.x, enemy.y, enemy.width, enemy.height, enemy.hBitmap);
    TextOutA(window.context, window.width / 3 * 2, 20, "coridor", 7);
    TextOutA(window.context, 20, window.height / 3, "closet", 6);
    TextOutA(window.context, window.width - 240, window.height / 3, "your master", 11);
    TextOutA(window.context, window.width / 4 * 3, window.height - 100, "utility", 7);
} // kitchen

void LocTwo() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//задний фон
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, table2.x, table2.y, table2.width, table2.height, table2.hBitmap);
    ShowBitmap(window.context, man.x, man.y, man.width, man.height, man.hBitmap);
} // man's room

void LocThree() {
    ShowBitmap(window.context, window.x, window.y, 700, 4000, hBack);//задний фон
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
} // coridor

void LocFour() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hCloset);//задний фон
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
} // closet: x (1100 - ww)

void LocFive() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hStove);
} // brokenstove

void LocSix() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    TextOutA(window.context, window.width / 4 * 3, 100, "kitchen", 7);
} // utility room

void LocSeven() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hCloset1);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, skeleton.x, skeleton.y, skeleton.width, skeleton.height, skeleton.hBitmap);
} // closet1

void LocEight() {
    ShowBitmap(window.context, window.width / 2, 0, window.width / 2, window.height / 2, hCloset2);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
} // closet2

void LocNine() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, pool.x, pool.y, pool.width, pool.height, pool.hBitmap);
    ShowBitmap(window.context, aquaman.x, aquaman.y, aquaman.width, aquaman.height, aquaman.hBitmap);
} // pool

void LocTen() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hIceroom);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
} // ice room

void LocEleven() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
} // fridge

void LocTwelve() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hOven);
} // oven

void LocThirteen() {
    ShowBitmap(window.context, window.x2, 0, window.width * 2, window.height, hCoridor2);
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, box13.x, box13.y, box13.width, box13.height, box13.hBitmap);
} // coridor2

void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
    GetClientRect(window.hWnd, &r);

}

void initItems() {
    if (hero.room == halva.room && halva.picked == 0 || halva.picked == 1) {
        ShowBitmap(window.context, halva.x, halva.y, halva.width, halva.height, halva.hBitmap);
    }
    if (hero.room == key.room && key.picked == 0 || key.picked == 1) {
        ShowBitmap(window.context, key.x, key.y, key.width, key.height, key.hBitmap);
    }
    if (hero.room == waffle.room && waffle.picked == 0 || waffle.picked == 1) {
        ShowBitmap(window.context, waffle.x, waffle.y, waffle.width, waffle.height, waffle.hBitmap);
    }
}

void coll(sprite b) {
    if (hero.x + hero.width >= b.x && hero.y < b.y + b.height && hero.y + hero.height > b.y && hero.x + hero.width <= b.x + hero.speed) {
        hero.x = b.x - 1 - hero.width;
    }
    if (hero.x <= b.x + b.width && hero.y < b.y + b.height && hero.y + hero.height > b.y && hero.x >= b.x + b.width - hero.speed) {
        hero.x = b.x + b.width + 1;
    }
    if (hero.y <= b.y + b.height && hero.x < b.x + b.width && hero.x + hero.width > b.x && hero.y >= b.y + b.height - hero.speed) {
        hero.y = b.y + b.height + 1;
    }
    if (hero.y + hero.height >= b.y && hero.x < b.x + b.width && hero.x + hero.width > b.x && hero.y + hero.height <= b.y + hero.speed) {
        hero.y = b.y - 1 - hero.height;
    }
}

void collCase() {
    if (hero.room == 1) {
        coll(table);
        coll(statue1);
        coll(statue2);
        coll(statue3);
        coll(enemy);
    }
    else if (hero.room == 2) {
        coll(table2);
        coll(man);
    }
    else if (hero.room == 7) {
        coll(skeleton);
    }
    else if (hero.room == 9) {
        coll(pool); coll(aquaman);
    }
    else if (hero.room == 13) {
        coll(box13);
    }
    else {

    }

    /*if (hero.x + hero.width >= oven.x && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x + hero.width <= oven.x + hero.speed ||
    hero.x <= oven.x + oven.width && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x >= oven.x + oven.width - hero.speed ||
    hero.y <= oven.y + oven.height && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y >= oven.y + oven.height - hero.speed ||
    hero.y + hero.height >= oven.y && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y + hero.height <= oven.y + hero.speed) {
        return 1;
    }
    newX = min(hero.x + hero.width - oven.x, oven.x + oven.width - hero.x, hero.y + hero.height - oven.y, oven.y + oven.height - hero.y);
    if (newXY == hero.x + hero.width - oven.x) {
    hero.x = hero.x + hero.width - oven.x;
    }
    else if (newXY == oven.x + oven.width - hero.x) {
    hero.x = oven.x + oven.width - hero.x;
    }
    else if (newXY == hero.y + hero.height - oven.y) {
    hero.y = hero.y + hero.height - oven.y;
    }
    else if (newXY == oven.y + oven.height - hero.y) {
    hero.y = oven.y + oven.height - hero.y;
    }
    etc*/

}

void moveHero() {
    // hero's movement
    if (GetAsyncKeyState('D') && GetAsyncKeyState('W')) {
        hero.x += hero.speed / sqrt(2);
        hero.y -= hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('W') && GetAsyncKeyState('A')) {
        hero.x -= hero.speed / sqrt(2);
        hero.y -= hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('S') && GetAsyncKeyState('A')) {
        hero.x -= hero.speed / sqrt(2);
        hero.y += hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('S') && GetAsyncKeyState('D')) {
        hero.x += hero.speed / sqrt(2);
        hero.y += hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('D')) {
        hero.x += hero.speed;
    }
    else if (GetAsyncKeyState('A')) {
        hero.x -= hero.speed;
    }
    else if (GetAsyncKeyState('W')) {
        if (hero.room == 3 && window.y <= 0) {
            window.y += hero.speed;
            //key.y += hero.speed;
        }
        else {
            hero.y -= hero.speed;
        }
    }
    else if (GetAsyncKeyState('S')) {
        if (hero.room == 3 && window.y >= -4000 + window.height) {
            window.y -= hero.speed;
            //key.y -= hero.speed;
        }
        else {
            hero.y += hero.speed;
        }
    }
    // collision
    collCase();
    // wall collision
    if (hero.x < 0) {
        hero.x = 0;
    }
    if (hero.room == 4 && hero.x < window.height) {
        hero.x = window.height;
    }
    if (hero.room == 8 && hero.x < window.width / 2) {
        hero.x = window.width / 2;
    }
    if (hero.x + hero.width > window.width) {
        hero.x = window.width - hero.width;
    }
    if (hero.room == 7 && hero.x + hero.width > window.width / 2) {
        hero.x = window.width / 2 - hero.width;
    }
    if (hero.room == 10 && hero.x + hero.width > window.height) {
        hero.x = window.height - hero.width;
    }
    if (hero.y < 0) {
        hero.y = 0;
    }
    if (hero.y + hero.height > window.height) {
        hero.y = window.height - hero.height;
    }
    if ((hero.room == 7 || hero.room == 8) && hero.y + hero.height > window.height / 2) {
        hero.y = window.height / 2 - hero.height;
    }
    // room traverse
    if (hero.room == 1 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x >= window.width - 150) {
        hero.room = 2;
        hero.x = 20;
    }
    else if (hero.room == 1 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y <= 0) {
        hero.room = 3;
        hero.y = window.height - 170;
    }
    else if (hero.room == 1 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x <= 0) {
        hero.room = 4;
        hero.x = window.width - 170;
    }
    else if (hero.room == 1 && hero.x >= window.width / 4 * 3 && hero.x <= window.width / 6 * 5 && hero.y >= window.height - 150) {
        hero.room = 6;
        hero.y = 20;
    }
    else if (hero.room == 1 && hero.x > 200 && hero.x + hero.width < 400 && hero.y < 200) {
        hero.room = 5;
    } // stove
    else if (hero.room == 1 && hero.x > 0 && hero.x + hero.width < 200 && hero.y < 200) {
        hero.room = 11;
    } // oven
    else if (hero.room == 1 && hero.x > 400 && hero.x + hero.width < 600 && hero.y < 200) {
        hero.room = 12;
    } // fridge
    if (hero.room == 5 || hero.room == 11 || hero.room == 12) {
        if (p.y >= window.height - 20) {
            hero.room = 1;
            hero.y = 160;
        }
    }
    else if (hero.room == 2 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x <= 0) {
        hero.room = 1;
        hero.x = window.width - 170;
    }
    else if (hero.room == 3 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y >= window.height - 150) {
        hero.room = 1;
        hero.y = 20;
    }
    else if (hero.room == 3 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y <= 0) {
        hero.room = 13;
        hero.y = window.height - 200;
        hero.x = window.width - 170;
    }
    else if (hero.room == 4 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x >= window.width - 150) {
        hero.room = 1;
        hero.x = 20;
    }
    else if (hero.room == 6 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y <= 0) {
        hero.room = 1;
        hero.y = window.height - 170;
    }
    else if (hero.room == 6 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x <= 0) {
        hero.room = 9;
        hero.x = window.width - 170;
    }
    else if (hero.room == 6 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y >= window.height - 150) {
        hero.room = 8;
        hero.y = 20;
    }
    else if (hero.room == 6 && hero.x <= window.width / 3 && hero.x >= window.width / 4 && hero.y >= window.height - 150) {
        hero.room = 7;
        hero.y = 20;
    }
    else if (hero.room == 6 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x >= window.width - 150) {
        hero.room = 10;
        hero.x = 20;
    }
    else if (hero.room == 7 && hero.x <= window.width / 3 && hero.x >= window.width / 4 && hero.y <= 0) {
        hero.room = 6;
        hero.y = window.height - 170;
    }
    else if (hero.room == 8 && hero.x >= window.width / 3 * 2 && hero.x <= window.width / 4 * 3 && hero.y <= 0) {
        hero.room = 6;
        hero.y = window.height - 170;
    }
    else if (hero.room == 9 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x >= window.width - 150) {
        hero.room = 6;
        hero.x = 20;
    }
    else if (hero.room == 10 && hero.y >= window.height / 3 && hero.y <= window.height / 2 && hero.x <= 0) {
        hero.room = 6;
        hero.x = window.width - 170;
    }
    else if ((hero.room == 5 || hero.room == 11 || hero.room == 12) && p.y == window.height - 100) {
        hero.room = 1;
        hero.y = 170;
    }
}

void move13() {

    if (GetAsyncKeyState('D')) {
        if (window.x2 <= -1001) {
            hero.x += hero.speed;
        }
        else {
            window.x2 -= hero.speed;
            box13.x -= hero.speed;
        }
    }
    else if (GetAsyncKeyState('A')) {
        if (window.x2 >= 0) {
            hero.x -= hero.speed;
        }
        else {
            window.x2 += hero.speed;
            box13.x += hero.speed;
        }
    }

    if (hero.y >= window.height - 170) {
        hero.y = window.height - 170;
        g = 0;
        jumping = false;
    }
    if (hero.x < box13.x + box13.width && hero.x + hero.width > box13.x && hero.y >= window.height - 325 && hero.y <= window.height - 200) {
        hero.y = window.height - 320;
        g = 0;
        jumping = false;
    }
    else if (hero.y < window.height - 175 && jumping == false) {
        g = hero.speed * 3;
        jumping = true;
    }
    if (GetAsyncKeyState('W')) {
        hero.y = hero.y - hero.speed * 3 + g;
        jumping = true;
    }
    if (jumping == true) {
        g += 2;
        hero.y = hero.y - hero.speed * 3 + g;
    }
    collCase();
    if (hero.x < 0) {
        hero.x = 0;
    }
    if (hero.x + hero.width >= window.width) {
        hero.room = 3;
        hero.y = 20;
        hero.x = window.width / 3 * 2;
    }
}

void moveEnemy(sprite& enem) {
    if (abs(enem.x - hero.x) > 350 || abs(enem.y - hero.y) > 350) {
        if (enem.x <= window.width / 4) {
            enem.y -= enem.speed;
        }
        if (enem.y <= 200) {
            enem.x += enem.speed;
        }
        if (enem.x >= window.width / 4 * 3) {
            enem.y += enem.speed;
        }
        if (enem.y >= window.height - 450) {
            enem.x -= enem.speed;
        }
    }
    else {
        if (enem.x - hero.x > 180) {
            enem.x -= enem.speed;
        }
        else if (hero.x - enem.x > 180) {
            enem.x += enem.speed;
        }
        else if (enem.y - hero.y > 180) {
            enem.y -= enem.speed;
        }
        else if (hero.y - enem.y > 180) {
            enem.y += enem.speed;
        }
    }
}

int slot() {
    if (GetAsyncKeyState('1')) {
        return 1;
    }
    else if (GetAsyncKeyState('2')) {
        return 2;
    }
    else if (GetAsyncKeyState('3')) {
        return 3;
    }
    else {
        return 4;
    }
}

int pickCase(item& ite, int slot) {
    if (ite.x >= hero.x && ite.x + ite.width <= hero.x + hero.width && ite.y >= hero.y && ite.y + ite.height <= hero.y + hero.height || ite.x <= p.x && ite.x + ite.width >= p.x &&
        ite.y <= p.y && ite.y + ite.height >= p.y) {
        if (slot == 1) {
            ite.x = window.width - 80;
            ite.y = window.height - 80;
            ite.picked = 1;
            return 1;
        }
        else if (slot == 2) {
            ite.x = window.width - 180;
            ite.y = window.height - 80;
            ite.picked = 1;
            return 2;
        }
        else if (slot == 3) {
            ite.x = window.width - 280;
            ite.y = window.height - 80;
            ite.picked = 1;
            return 3;
        }
    }
}

int dropCase(item& ite) {
    if (hero.room != 3 && ite.picked == 1 && (GetAsyncKeyState('1') && ite.x == window.width - 80 && ite.y == window.height - 80 ||
        GetAsyncKeyState('2') && ite.x == window.width - 180 && ite.y == window.height - 80 || GetAsyncKeyState('3') &&
        ite.x == window.width - 280 && ite.y == window.height - 80)) {
        if (hero.room == 5 || hero.room == 11 || hero.room == 12) {
            ite.x = p.x;
            ite.y = p.y;
        }
        else {
            ite.x = hero.x;
            ite.y = hero.y;
        }
        ite.picked = 0;
        ite.room = hero.room;
        if (ite.x == window.width - 80) {
            return 1;
        }
        else if (ite.x == window.width - 180) {
            return 2;
        }
        else if (ite.x == window.width - 280) {
            return 3;
        }
    }
}

void pickItem(int& s1, int& s2, int& s3) {
    int slot0 = 0;
    if (s1 == 0) {
        slot0 = 1;
    }
    else if (s2 == 0) {
        slot0 = 2;
    }
    else if (s3 == 0) {
        slot0 = 3;
    }
    int slot00 = slot();
    if (pickCase(halva, slot0) == slot00 || pickCase(key, slot0) == slot00 || pickCase(waffle, slot0) == slot00) {
        if (slot00 == 1) {
            s1 = 1;
        }
        else if (slot00 == 2) {
            s2 = 1;
        }
        else if (slot00 == 3) {
            s3 = 1;
        }
    }
}

int dropItem(int& s1, int& s2, int& s3) {
    while (not(GetAsyncKeyState('1')) && not(GetAsyncKeyState('2')) && not(GetAsyncKeyState('3'))) {
        int slot0 = slot();
        if (dropCase(halva) == slot0 || dropCase(key) == slot0 || dropCase(waffle) == slot0) {
            if (slot0 == 1) {
                s1 = 0;
            }
        else if (slot0 == 2) {
            s2 = 0;
        }
        else if (slot0 == 3) {
            s3 = 0;
        }
    }
    }
    return s1, s2, s3;
}

void eatCase(item& i) {
    if (i.x == hero.x && i.y == hero.y) {
        i.x = -100;
        i.y = -100;
        i.picked = 0;
        hero.hp += 2;
    }
}

void eatItem(int& s1, int& s2, int& s3) {
    while (not(GetAsyncKeyState('1')) && not(GetAsyncKeyState('2')) && not(GetAsyncKeyState('3'))) {
        dropItem(s1, s2, s3);
        eatCase(halva); eatCase(waffle);
    }
}

void useItem(int& s1, int& s2, int& s3) {
    while (not(GetAsyncKeyState('1')) && not(GetAsyncKeyState('2')) && not(GetAsyncKeyState('3'))) {
        dropItem(s1, s2, s3);
    }
}



int alpha = 0;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR  lpCmdLine, int  nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры

    setText();
    int slot1 = 0;
    int slot2 = 0;
    int slot3 = 0;

    currentTime = timeGetTime();
    lastTime = currentTime;
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        switch (hero.room) {
        case 1: {
            LocOne();
            break;
        }
        case 2: {
            LocTwo();
            break;
        }
        case 3: {
            LocThree();
            break;
        }
        case 4: {
            LocFour();
            break;
        }
        case 5: {
            LocFive();
            break;
        }
        case 6: {
            LocSix();
            break;
        }
        case 7: {
            LocSeven();
            break;
        }
        case 8: {
            LocEight();
            break;
        }
        case 9: {
            LocNine();
            break;
        }
        case 10: {
            LocTen();
            break;
        }
        case 11: {
            LocEleven();
            break;
        }
        case 12: {
            LocTwelve();
            break;
        }
        case 13: {
            LocThirteen();
            break;
        }
        }
        currentTime = timeGetTime();
        tutorial();
        std::string A = "health: " + std::to_string((int)hero.hp);
        TextOutA(window.context, 100 , 20, A.c_str(), 11);
        
        if (hero.room != 5 && hero.room != 11 && hero.room != 12) {
            //ShowCursor(NULL);
        }
        else {
            mouseInput();
        }
        initItems();
        if (hero.room != 13) {
            moveHero();
        }
        else {
            move13();
        }
        if (hero.room == 1) {
            moveEnemy(enemy);
        }
        else if (hero.room == 2) {
            moveEnemy(man);
        }
        else if (hero.room == 7) {
            moveEnemy(skeleton);
        }
        else if (hero.room == 9) {
            moveEnemy(aquaman);
        }
        if (GetAsyncKeyState('G')) {
            pickItem(slot1, slot2, slot3);
        }
        if (GetAsyncKeyState('T')) {
            dropItem(slot1, slot2, slot3);
        }
        if (GetAsyncKeyState('E')) {
            eatItem(slot1, slot2, slot3);
        }
        if (GetAsyncKeyState('U') && (hero.room == 12 || hero.room == 10)) {
            useItem(slot1, slot2, slot3);
        }
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
    }

}
