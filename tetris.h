#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>

//gloable const date for sense
const int BLOCK_SIZE = 25;// the lenht of one pice
const int MARGIN = 5;
const int AREA_ROW = 20;
const int AREA_COL = 12;
//gloable const date end

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

enum block_color{
    iRED,
    iORANG,
    iYELLOW,
    iGREEN,
    iBLUE,
    iCYAN,
    iPURPLE,
    iWHITE
};

struct Border
{
    int ubound;
    int dbound;
    int lbound;
    int rbound;
};


//most confusing part, the 0 point is i = 0, j = 0;
//and i means going dong, j means going ringht
struct shape_point
{
    int pos_i;
    int pos_j;
    //    block_point(int x,int y):pos_x(x),pos_y(y){}
};

namespace Ui {
class Tetris;
}

class Tetris : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tetris(QWidget *parent = 0);
    ~Tetris();

    //logic part
    void GetBorder(int block[4][4], Border &border);
    void ShapeMove(Direction dir);//to define the block move
    void GetNewShape();
    void ShapeCopy(int shape[4][4], int block_id);
    void UpdateScreen();
    //logic part


    //QT5, out of class range
    void paintEvent(QPaintEvent *event);// paint all blocks
    void timerEvent(QTimerEvent *event); //two timer, first to control the shape falling,second control the sence fresh
    void keyPressEvent(QKeyEvent *event);//handle the keyboard event to control the shape move
    void setColor(QPainter *p, block_color c);
    //out of class range end


    //game start inital over
    void InitGame();
    void StartGame();
    void GameOver();
    //game states end

private:
    Ui::Tetris *ui;

    int game_area[AREA_ROW][AREA_COL];
    shape_point shape_pos;
    int cur_block[4][4];
    Border cur_border;
    block_color cur_color;
    int next_block[4][4];
    block_color next_color;
    block_color colorTable[AREA_ROW][AREA_COL];
    bool isStable;
    int score;
    int game_timer;
    int paint_timer;
    int speed_ms;
    int refresh_ms;
};

#endif // TETRIS_H
