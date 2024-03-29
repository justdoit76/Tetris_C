#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Tetris;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    Tetris* pGame;
};
#endif // WIDGET_H