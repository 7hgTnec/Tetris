#include <time.h>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include "tetris.h"
#include "ui_tetris.h"

//difine the block and range 0 mean no block, 1 mean unstable block, 2 mean stable bolck
//o shape

int item[7][4][4] =
{
    //o shape
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },

    //L shape
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },

    //J shape
    {
        {0,0,1,0},
        {0,0,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },

    //s shape
    {
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    //z shape
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    //T shape
    {
        {0,1,1,1},
        {0,0,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    //I shape
    {
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0}
    }
};
//block define end

Tetris::Tetris(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tetris)
{
    ui->setupUi(this);

    //set the windoes size
    //wideth euqal the (col+4)*blockzie+margin*4 because there have one main area and the nextblock area
    //length queal row*blocksize + margin*2 only main area
    resize((AREA_COL+4)*BLOCK_SIZE+MARGIN*4,AREA_ROW*BLOCK_SIZE+MARGIN*2);

    //initial game
    InitGame();
}

Tetris::~Tetris()
{
    delete ui;
}


void Tetris::setColor(QPainter *p, block_color c){
    switch (c) {
    case iRED:
        p->setBrush(QBrush(Qt::red,Qt::SolidPattern));
        break;
    case iORANG:
        p->setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
        break;
    case iYELLOW:
        p->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
        break;
    case iGREEN:
        p->setBrush(QBrush(Qt::green,Qt::SolidPattern));
        break;
    case iBLUE:
        p->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
        break;
    case iCYAN:
        p->setBrush(QBrush(Qt::cyan,Qt::SolidPattern));
        break;
    case iPURPLE:
        p->setBrush(QBrush(Qt::magenta,Qt::SolidPattern));
        break;
    default:
        p->setBrush(QBrush(Qt::white,Qt::SolidPattern));
        break;
    }
}

void Tetris::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    //draw the main game area

    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));//(backgrouand, pattern)
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);//(x,y,wendth,length)

    //draw the next shape area

    //selcet the painter color
    //beacuse color change a lot, so make those code because a function

   // painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));//()

    setColor(&painter,next_color);

    //setColor(painter,next_color);
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_block[i][j]==1)
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE,MARGIN+i*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //score
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(QRect(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN*2+4*BLOCK_SIZE,BLOCK_SIZE*4,BLOCK_SIZE*4),Qt::AlignCenter,"score: "+QString::number(score));


    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            //draw falling shape
            if(game_area[i][j]==1)
            {
                //painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
                setColor(&painter,cur_color);
                painter.drawRect(MARGIN+j*BLOCK_SIZE,MARGIN+i*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);//(i*BLOCK_SIZE+MARGIN,(19-j)*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
            //draw stable shape
            else if(game_area[i][j]==2)
            {
                setColor(&painter,colorTable[i][j]);
                //painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
        }
}

void Tetris::timerEvent(QTimerEvent *event)
{
    //block falling
    if(event->timerId()==game_timer)
        //BlockMove(DOWN);
    //fresh screen
    if(event->timerId()==paint_timer)
        update();
}

void Tetris::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        //BlockMove(UP);
        break;
    case Qt::Key_Down:
        //BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        //BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        //BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        //BlockMove(SPACE);
        break;
    default:
        break;
    }
}
//***************************logic*****************************
void Tetris::ShapeCopy(int shape[4][4], int block_id){
    for(int i =  0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            shape[i][j] = item[block_id][i][j];
        }
    }

}

void Tetris::UpdateScreen(){
    for(int i =0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            game_area[shape_pos.pos_i+i][shape_pos.pos_j+j] = cur_block[i][j];
            colorTable[shape_pos.pos_i+i][shape_pos.pos_j+j] = cur_color;
        }
    }
}
//*************************logic end******************************

void Tetris::StartGame()
{
    game_timer=startTimer(speed_ms); //start game timer
    paint_timer=startTimer(refresh_ms); //start screen timer
    //creat the first shape
    int block_id=rand()%7;
    ShapeCopy(cur_block, block_id);
    //set the shape color
    cur_color = (block_color)block_id;
    UpdateScreen();

    //creat the first next shape
    block_id=rand()%7;
    ShapeCopy(next_block,block_id);
    //set the next shape color
    next_color = (block_color)block_id;


    //CreateBlock(next_block,block_id);
    //ResetBlock(); //产生方块
}

void Tetris::InitGame()
{
    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            game_area[i][j]=0;
            colorTable[i][j] = iWHITE;
        }
    //initial first shape position at center of top
    shape_pos.pos_i = 0;
    shape_pos.pos_j = 3;
    //game_area[3][19] = 1;
    //cur_color = iRED;
    //initial the timer intervial
    speed_ms=800;
    refresh_ms=30;

    //initial time seed
    srand(time(0));

    //reset score to 0
    score=0;

    //start game
    StartGame();
}

void Tetris::GameOver()
{
    //gameover stop the timer
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this,"failed","game over");

}


