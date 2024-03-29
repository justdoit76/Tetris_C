#ifndef BLOCK_FACTORY_H
#define BLOCK_FACTORY_H

#include "block.h"

class Block_Factory
{
public:
    static Block* createBlock(Btype bt)
    {
        switch(bt)
        {
        case O:
            return new BO(bt);
        case I:
            return new BI(bt);
        case S:
            return new BS(bt);
        case Z:
            return new BZ(bt);
        case L:
            return new BL(bt);
        case J:
            return new BJ(bt);
        case T:
            return new BZ(bt);
        default:
            return nullptr;
        }
    }
};

#endif // BLOCK_FACTORY_H
