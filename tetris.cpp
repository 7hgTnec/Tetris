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
        ShapeMove(DOWN);
    //fresh screen
    if(event->timerId()==paint_timer)
        update();
}

void Tetris::keyPressEvent(QKeyEvent *event)
{
    //int t;
    int new_id;
    switch(event->key())
    {
    case Qt::Key_Up:
        ShapeMove(UP);
        break;
    case Qt::Key_Down:
        ShapeMove(DOWN);
        break;
    case Qt::Key_Left:
        ShapeMove(LEFT);
        break;
    case Qt::Key_Right:
        ShapeMove(RIGHT);
        break;
    case Qt::Key_Space:

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

void Tetris::ShapeCopy(int to[4][4], int from[4][4]){
    for(int i = 0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            to[i][j] = from[i][j];
        }
    }
}

void Tetris::CleanShape(){
    for(int i =0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            if(cur_block[i][j] == 1){
                game_area[shape_pos.pos_i+i][shape_pos.pos_j+j] = 0;// cur_block[i][j];
                colorTable[shape_pos.pos_i+i][shape_pos.pos_j+j] = iWHITE;//cur_color;
            }
        }
    }
}

void Tetris::UpdateScreen(){
    for(int i =0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            if(cur_block[i][j] == 1){
                game_area[shape_pos.pos_i+i][shape_pos.pos_j+j] = cur_block[i][j];
                colorTable[shape_pos.pos_i+i][shape_pos.pos_j+j] = cur_color;
            }
        }
    }
}

bool Tetris::isCollison(int fi, int fj){
    for(int i = 0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            if(cur_block[i][j] == 1){
                if(cur_block[i][j]==1 &&(fi+i<0 || fi+i>19)) return false;
                if(cur_block[i][j]==1 &&(fj+j<0 || fj+j>11)) return false;
                if(game_area[fi+i][fj+j] == 2 && cur_block[i][j]==1) return false;
            }
        }
    }
    return true;
}


void Tetris::ShapeMove(Direction dir){
    bool myflag = true;
    switch (dir) {
    case DOWN:
        update();
        myflag = false;
        if(isCollison(shape_pos.pos_i+1,shape_pos.pos_j)){
            CleanShape();
            shape_pos.pos_i++;
            UpdateScreen();
        }

        //************judge is touch the bottun or the bottun block
        for(int i = 0; i<4; ++i){
            for(int j = 0; j<4; ++j){
                if(cur_block[i][j] == 1){
                    if(shape_pos.pos_i+i+1>19 || game_area[shape_pos.pos_i+i+1][shape_pos.pos_j+j] == 2){
                        myflag = true;
                        break;
                    }
                }
            }
            if(myflag) break;
        }
        if(myflag){//if touched change the moveing block to the satble bolck on the map
            for(int i = 0; i<4; ++i){
                for(int j = 0; j<4; ++j){
                    if(cur_block[i][j] == 1){
                        game_area[shape_pos.pos_i+i][shape_pos.pos_j+j] = 2;
                        colorTable[shape_pos.pos_i+i][shape_pos.pos_j+j] = cur_color;
                    }
                }
            }
            //************get the new shape and create new nextblock and reset the pos
            shape_pos.pos_i = 0;
            shape_pos.pos_j = 3;
            ShapeCopy(cur_block,next_block);
            cur_color = next_color;

            int new_id = rand()%7;
            ShapeCopy(next_block,new_id);
            next_color = (block_color)new_id;


            int i;
            bool break_flag = false;
            for(i = 19; i>-1; --i){
                for(int j = 0; j<12; ++j){
                    if(game_area[i][j] == 0){
                        break_flag = true;
                        break;
                    }
                }
                if(break_flag) break;
            }
            if(i<19){
                for(int k = 19; k>-1; --k){
                    for(int j = 0; j<12; ++j){
                        if(game_area[k-1][j] != 1){
                            game_area[k][j] = game_area[k-1][j];
                            colorTable[k][j] = colorTable[k-1][j];
                            //score=i;
                        }
                    }
                }
                UpdateScreen();
                score += (19-i)*10;
            }
        }
        for(int j = 0; j<12; ++j){
            if(game_area[0][j] == 2)
                GameOver();
        }
        //*****************
        break;
    case LEFT:
        if(isCollison(shape_pos.pos_i,shape_pos.pos_j-1)){
            CleanShape();
            shape_pos.pos_j--;
            UpdateScreen();
        }
        break;
    case RIGHT:
        if(isCollison(shape_pos.pos_i,shape_pos.pos_j+1)){
            CleanShape();
            shape_pos.pos_j++;
            UpdateScreen();
        }
        break;
    case UP:
        myflag = true;
        int temp_shape[4][4];
        for(int i = 0; i<4; ++i){
            for(int j = 0; j<4; ++j){
                temp_shape[i][j] = cur_block[3-j][i];
            }
        }

        for(int i = 0; i<4; ++i){
            for(int j = 0; j<4; ++j){
                if(temp_shape[i][j] == 1)
                if(game_area[shape_pos.pos_i+i][shape_pos.pos_j] == 2 || shape_pos.pos_i+i<0 || shape_pos.pos_i+i>19 ||
                        shape_pos.pos_j+j<0 || shape_pos.pos_j+j>11){
                    myflag = false;
                    break;
                }

            }
            if(myflag == false) break;
        }

        if(myflag){
            CleanShape();
            ShapeCopy(cur_block,temp_shape);
            UpdateScreen();
        }
        break;
    default:
        break;
    }
}
//*************************logic end******************************
void Tetris::StartGame()
{
    game_timer=startTimer(speed_ms); //start game timer
    paint_timer=startTimer(refresh_ms); //start screen timer
    //creat the first shape
    int block_id= rand()%7;
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


