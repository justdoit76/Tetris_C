#include "tetris.h"
#include "block.h"
#include "block_factory.h"
#include <QWidget>
#include <QPainter>
#include <random>

Tetris::Tetris(QWidget* p) : parent(p), pb(nullptr)
{
    rect = inrect = p->rect();
    const int gap = 20;
    inrect.adjust(gap, gap, -gap, -gap);
    size = inrect.height() / ROW;
    cy = -1;
    cx = COL/2-Block::SIZE;

    initBlock();
    initMap();
}

Tetris::~Tetris()
{
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
    p->drawRect(inrect);

    QBrush b(QColor(255, 0, 0));
    p->setBrush(b);
    p->drawRect(rects[0][0]);

}

void Tetris::keyDown(int key)
{

}
