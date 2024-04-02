#include "tetris.h"
#include "block.h"
#include "block_factory.h"
#include "widget.h"
#include <QWidget>
#include <QPainter>
#include <random>
#include <chrono>

Tetris::Tetris(Widget* p) : QObject(), parent(p), pb(nullptr), pthread(nullptr), run(true)
{
    //signals
    connect(this, &Tetris::updateSignal, p, &Widget::update);
    connect(this, &Tetris::endSignal, p, &Widget::gameOver);

    rect = inrect = p->rect();
    const int gap = 20;
    inrect.adjust(gap, gap, -gap, -gap);
    size = inrect.height() / ROW;    

    initBlock();
    initMap();

    //thread
    pthread = new std::thread(&Tetris::threadFunc, this);
}

Tetris::~Tetris()
{
    run = false;
    if(pthread)
    {
        if(pthread->joinable())
            pthread->join();

        delete pthread;
        pthread = nullptr;
    }

    if(pb)
    {
        delete pb;
        pb = nullptr;
    }
}

int Tetris::randInt(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

void Tetris::initBlock()
{
    if(pb)
    {
        delete pb;
        pb = nullptr;
    }

    int n = randInt(O, T);
    pb = Block_Factory::createBlock(static_cast<Btype>(n));

    cy = -1;
    cx = COL/2-Block::SIZE/2;
}

void Tetris::initMap()
{
    int x = inrect.left();
    int y = inrect.top();

    for(int r=0; r<ROW; r++)
    {
        //color map
        std::vector<QColor> colors(COL, QColor(0,0,0,0));
        cmaps.push_back(colors);

        //logical map (0:None, 1:Move block, 2:Stacked block)
        std::vector<int> v(COL, 0);
        maps.push_back(v);

        // displayed map
        std::vector<QRect> temp;
        for (int c=0; c<COL; c++)
        {
            int dx = x+c*size;
            int dy = y+r*size;
            QRect rect(dx, dy, size, size);
            temp.push_back(rect);
        }
        rects.push_back(temp);
    }
}

void Tetris::draw(QPainter* p)
{
    drawBackground(p);
    drawBlock(p);
}

void Tetris::drawBackground(QPainter *p)
{
    int x = inrect.left();
    int y = inrect.top();
    int x2 = inrect.right();
    int y2 = inrect.top();
    int x3 = inrect.left();
    int y3 = inrect.bottom();

    for (int i=0; i<ROW+1; i++)
    {
        p->drawLine(x, y+i*size, x2, y2+i*size);
        if(i<COL+1)
            p->drawLine(x+i*size, y, x3+i*size, y3);
    }
}

void Tetris::drawBlock(QPainter* p)
{
    for(int r=0; r<ROW; r++)
    {
        for(int c=0; c<COL; c++)
        {
            if (maps[r][c]!=0)
            {
                if(maps[r][c]==1)
                {
                    auto [R,G,B,A] = pb->getColor();
                    p->setBrush( QColor(R,G,B,A));
                }
                else
                {
                    int R = cmaps[r][c].red();
                    int G = cmaps[r][c].green();
                    int B = cmaps[r][c].blue();
                    int A = cmaps[r][c].alpha();
                    p->setBrush(QColor(R,G,B,A));
                }
            }
            else
            {
                p->setBrush(Qt::NoBrush);
            }
            p->drawRect(rects[r][c]);
            p->drawText(rects[r][c], Qt::AlignCenter, QString::number(maps[r][c]));
        }
    }
}

void Tetris::keyDown(int key)
{
    mtx.lock();
    auto [U, D, L, R] = pb->findTail();

    switch(key)
    {
    case Qt::Key_Left:
        if (cx>0-L && isOverlapped(cx-1, cy)==false)
        {
            cx-=1;
        }
        break;
    case Qt::Key_Right:
        if(cx<COL-1-R && isOverlapped(cx+1, cy)==false)
        {
            cx+=1;
        }
        break;
    case Qt::Key_Up:
        {
        pb->rotate_r();
        auto [U, D, L, R] = pb->findTail();
        if( (cx<0-L || cx>COL-1-R) || isOverlapped(cx, cy))
            pb->rotate_l();
        break;
        }
    case Qt::Key_Down:        
        if(cy-D < ROW-2)
            cy+=1;
        break;
    }
    blockUpdate();
    mtx.unlock();
    emit updateSignal();
}

bool Tetris::isOverlapped(int x, int y)
{
    if(y<Block::SIZE)
        return false;

    auto [U, D, L, R] = pb->findTail();
    auto bl = pb->getArr();

    for(int r=U; r<D+1; r++)
        for(int c=L; c<R+1; c++)            
                if(bl[r][c] && maps[y-r-1][c+x]==2)
                    return true;
    return false;
}

bool Tetris::blockUpdate()
{
    auto bl = pb->getArr();

    // delete before blocks
    for (auto itr=before.begin(); itr!=before.end(); ++itr)
    {
        auto [r, c] = *itr;
        maps[r][c] = 0;
    }
    before.clear();

    // set current blocks
    for(int r=0; r<Block::SIZE; r++)
    {
        for(int c=0; c<Block::SIZE; c++)
        {
            if(bl[Block::SIZE-1-r][c])
            {
                if(cy-r>=0)
                {
                    maps[cy-r][c+cx] = 1;
                    // remember current blocks
                    before.push_back(std::make_tuple(cy-r, c+cx));
                }
            }
        }
    }

    // stack or not
    if(!isMoveDown())
    {
        if(cy<=1)
        {
            emit updateSignal();
            return false;
        }

        stackBlock();
        removeBlock();
        initBlock();
    }
    emit updateSignal();
    return true;
}

bool Tetris::isMoveDown()
{
    auto bl = pb->getArr();
    for(int r=0; r<Block::SIZE; r++)
    {
        for(int c=0; c<Block::SIZE; c++)
        {
            if(bl[Block::SIZE-1-r][c])
            {
                if(cy-r+1 > ROW-1)
                {
                    before.clear();
                    return false;
                }
                else if(cy-r>=0)
                {
                    if(maps[cy-r+1][c+cx]==2)
                    {
                        before.clear();
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Tetris::stackBlock()
{
    auto bl = pb->getArr();
    auto [R,G,B,A] = pb->getColor();
    QColor color(R,G,B,A);

    for (int r=0; r<Block::SIZE; r++)
    {
        for (int c=0; c<Block::SIZE; c++)
        {
            if(bl[Block::SIZE-1-r][c])
            {
                maps[cy-r][c+cx] = 2;
                cmaps[cy-r][c+cx] = color;
            }
        }
    }
}

void Tetris::removeBlock()
{
    std::vector<int> lines;
    for (int r=0; r<ROW; r++)
    {
        int cnt = 0;
        for (int c=0; c<COL; c++)
        {
            if(maps[r][c]==2)
                cnt+=1;
            else
                break;
        }
        if(cnt==COL)
            lines.push_back(r);
    }

    if(!lines.empty())
    {
        for(auto itr = lines.begin(); itr!=lines.end(); ++itr)
        {
            for(int c=0; c<COL; c++)
            {
                maps[*itr][c] = 0;
            }
            std::this_thread::sleep_for( std::chrono::milliseconds(20) );

            // fall blocks
            for(int rr=*itr-1; rr>-1; rr--)
            {
                for(int cc=0; cc<COL; cc++)
                {
                    if(maps[rr][cc]==2)
                    {
                        maps[rr+1][cc] = 2;
                        maps[rr][cc] = 0;
                        std::this_thread::sleep_for( std::chrono::milliseconds(10) );
                    }
                }
            }
            std::this_thread::sleep_for( std::chrono::milliseconds(20) );
        }
    }
}

void Tetris::threadFunc()
{
    while(run)
    {
        mtx.lock();
        cy+=1;
        if(!blockUpdate())
        {
            emit endSignal();
            mtx.unlock();
            break;
        }
        mtx.unlock();
        std::this_thread::sleep_for( std::chrono::milliseconds(500) );
    }
}
