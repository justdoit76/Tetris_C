#include "widget.h"
#include "tetris.h"
#include <QPainter>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent), pGame(nullptr)
{
    setWindowTitle("Ocean Coding School");
    setFixedSize(340,640);
    pGame = new Tetris(this);
}

Widget::~Widget()
{
    if(pGame)
    {
        delete pGame;
        pGame = nullptr;
    }
}

void Widget::paintEvent(QPaintEvent* e)
{
    QPainter qp;
    qp.begin(this);

    if(pGame)
        pGame->draw(&qp);

    qp.end();
}

void Widget::keyPressEvent(QKeyEvent* e)
{
    if(pGame)
        pGame->keyDown( e->key() );
}
