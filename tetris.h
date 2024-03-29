#ifndef TETRIS_H
#define TETRIS_H

#include <QRect>

class Block;
class QWidget;
class QPainter;
class QColor;

class Tetris
{
public:
    Tetris(QWidget*);
    ~Tetris();

private:
    static const int ROW=20, COL=10;
    QWidget *parent;
    QRect rect, inrect;
    int size, cx, cy;
    Block *pb;

    std::vector<std::vector<QColor>> cmaps;
    std::vector<std::tuple<int, int>> before;
    std::vector<std::vector<int>> maps;
    std::vector<std::vector<QRect>> rects;

public:
    void draw(QPainter*);
    void keyDown(int);

private:
    void initBlock();
    void initMap();
    int randInt(int, int);

};

#endif // TETRIS_H
