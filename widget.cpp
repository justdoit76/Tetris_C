#include "widget.h"
#include "tetris.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

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
    {
        if(e->key()==Qt::Key_Left || e->key()==Qt::Key_Right
            || e->key()==Qt::Key_Up || e->key()==Qt::Key_Down)
            pGame->keyDown( e->key() );
    }
}

void Widget::closeEvent(QCloseEvent* e)
{
    if(pGame)
    {
        delete pGame;
        pGame = nullptr;
    }
}

void Widget::gameOver()
{
    auto result = QMessageBox::information(this, tr("Game Over!"), tr("Retry(Y), Exit(N)"), QMessageBox::Yes | QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
        if(pGame)
        {
            delete pGame;
            pGame = nullptr;
        }
        pGame = new Tetris(this);
    }
    else
    {
        close();
    }
}
