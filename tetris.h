#ifndef TETRIS_H
#define TETRIS_H

#include <QObject>
#include <QRect>
#include <thread>
#include <mutex>

class Block;
class Widget;
class QPainter;
class QColor;

class Tetris : public QObject
{
    Q_OBJECT
public:
    Tetris(Widget*);
    ~Tetris();

private:
    static const int ROW=20, COL=10;
    Widget *parent;
    QRect rect, inrect;
    int size, cx, cy;
    Block *pb;
    std::thread *pthread;
    bool run;
    std::mutex mtx;

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
    void drawBackground(QPainter*);
    void drawBlock(QPainter*);
    bool isOverlapped(int, int);
    bool blockUpdate();
    bool isMoveDown();
    void stackBlock();
    void removeBlock();
    void threadFunc();

signals:
    void updateSignal();
    void endSignal();
};

#endif // TETRIS_H
