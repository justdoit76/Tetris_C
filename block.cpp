#include "block.h"
#include <algorithm>

void Block::rotate_r()
{
    size_t n = arr.size();
    if (idx>=n-1)
        idx = 0;
    else
        idx+=1;
}

void Block::rotate_l()
{
    size_t n = arr.size();
    if (idx<=0)
        idx = n-1;
    else
        idx-=1;
}

std::tuple<int, int, int, int> Block::findTail() const
{
    int U = findUpperTail(arr[idx]);
    int D = findLowerTail(arr[idx]);
    int L = findLeftTail(arr[idx]);
    int R = findRightTail(arr[idx]);
    return std::make_tuple(U, D, L, R);
}

int Block::findUpperTail(const std::vector<std::vector<bool>> bl) const
{
    for(int r=0; r<SIZE; r++)
        for (int c=0; c<SIZE; c++)
            if(bl[r][c])
                return r;
}

int Block::findLowerTail(const std::vector<std::vector<bool>> bl) const
{
    for(int r=SIZE-1; r>=0; r--)
        for (int c=0; c<SIZE; c++)
            if(bl[r][c])
                return r;
}

int Block::findLeftTail(const std::vector<std::vector<bool>> bl) const
{
    for(int c=0; c<SIZE; c++)
        for (int r=0; r<SIZE; r++)
            if(bl[r][c])
                return c;
}

int Block::findRightTail(const std::vector<std::vector<bool>> bl) const
{
    for(int c=SIZE-1; c>=0; c--)
        for (int r=0; r<SIZE; r++)
            if(bl[r][c])
                return c;
}


BO::BO(Btype bt) : Block(bt)
{
    R=255, G=255, B=0;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    for(int r=1; r<3; r++)
    {
        for (int c=1;c<3;c++)
        {
            temp[r][c] = true;
        }
    }
    arr.push_back(temp);
}

BI::BI(Btype bt) : Block(bt)
{
    R=115, G=251, B=253;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    for(int c=0; c<SIZE; c++)
        temp[1][c] = true;
    arr.push_back(temp);

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    for (int r=0; r<SIZE; r++)
        temp[r][2] = true;
    arr.push_back(temp);
}

BS::BS(Btype bt) : Block(bt)
{
    R=0, G=255, B=0;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][1] = true;
    temp[2][2] = true;
    arr.push_back(temp);

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][3] = true;
    arr.push_back(temp);
}

BZ::BZ(Btype bt) : Block(bt)
{
    R=255, G=0, B=0;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    temp[2][2] = true;
    temp[2][3] = true;
    temp[1][1] = true;
    temp[1][2] = true;
    arr.push_back(temp);

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][3] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][2] = true;
    arr.push_back(temp);
}

BL::BL(Btype bt) : Block(bt)
{
    R=255, G=168, B=76;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][1] = true;
    arr.push_back(temp);

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][2] = true;
    temp[2][2] = true;
    temp[2][3] = true;
    arr.push_back(temp);

    //2
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][3] = true;
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    arr.push_back(temp);

    //3
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][1] = true;
    temp[0][2] = true;
    temp[1][2] = true;
    temp[2][2] = true;
    arr.push_back(temp);
}

BJ::BJ(Btype bt) : Block(bt)
{
    R=0, G=0, B=255;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][3] = true;
    arr.push_back(temp);

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[0][3] = true;
    temp[1][2] = true;
    temp[2][2] = true;
    arr.push_back(temp);

    //2
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][1] = true;
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    arr.push_back(temp);

    //3
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][2] = true;
    temp[2][1] = true;
    temp[2][2] = true;
    arr.push_back(temp);
}

BT::BT(Btype bt) : Block(bt)
{
    R=255, G=0, B=255;
    std::vector<std::vector<bool>> temp(SIZE, std::vector<bool>(SIZE, false));
    //0
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][2] = true;

    //1
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    temp[2][2] = true;
    arr.push_back(temp);

    //2
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][1] = true;
    temp[1][2] = true;
    temp[1][3] = true;
    arr.push_back(temp);

    //3
    fill(temp.begin(), temp.end(),  std::vector<bool>(SIZE, false));
    temp[0][2] = true;
    temp[1][1] = true;
    temp[1][2] = true;
    temp[2][2] = true;
    arr.push_back(temp);
}
