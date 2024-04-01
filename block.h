#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <tuple>

enum Btype {O, I, S, Z, L, J, T};

class Block
{
public:
    Block(Btype bt) : type(bt), idx(0), R(0), G(0), B(0), A(255) {}
public:
    static const int SIZE=4;
    void rotate_r();
    void rotate_l();    
    std::tuple<int, int, int, int> findTail() const;
    inline const std::vector<std::vector<bool>> getArr() const {return arr[idx];}
    inline const std::tuple<int, int, int, int> getColor() const {return std::make_tuple(R, G, B, A);}
protected:
    Btype type;
    std::vector<std::vector<std::vector<bool>>> arr;
    size_t idx;
    int R, G, B, A;

    int findUpperTail(const std::vector<std::vector<bool>>) const;
    int findLowerTail(const std::vector<std::vector<bool>>) const;
    int findLeftTail(const std::vector<std::vector<bool>>) const;
    int findRightTail(const std::vector<std::vector<bool>>) const;
};

class BO : public Block
{
public:
    BO(Btype);
};

class BI : public Block
{
public:
    BI(Btype);
};

class BS : public Block
{
public:
    BS(Btype);
};

class BZ : public Block
{
public:
    BZ(Btype);
};

class BL : public Block
{
public:
    BL(Btype);
};

class BJ : public Block
{
public:
    BJ(Btype);
};

class BT : public Block
{
public:
    BT(Btype);
};

#endif // BLOCK_H
