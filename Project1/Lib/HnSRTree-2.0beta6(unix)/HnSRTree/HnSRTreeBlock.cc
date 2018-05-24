/*
 * HnSRTreeBlock.cc
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2010/03/09 03:27:14
 */

#include "HnSRTree/HnSRTreeBlock.hh"
#include "HnSRTree/HnSRTreeBlockObj.hh"

/*
 * HnSRTreeBlock
 */

const HnSRTreeBlock HnSRTreeBlock::null;

HnSRTreeBlock
new_HnSRTreeBlock(long offset, int size, HnSRTreeBlock::Type type)
{
    HnSRTreeBlockObj *_obj;

    _obj = new HnSRTreeBlockObj(offset, size, type);

    if ( _obj->hasConstructorFailed() ) {
        delete _obj;
        return (HnSRTreeBlockObj *)NULL;
    }

    return _obj;
}

HnSRTreeBlock
new_HnSRTreeBlock(long offset, HnBlockStream &blockStream)
{
    HnSRTreeBlockObj *_obj;

    _obj = new HnSRTreeBlockObj(offset, blockStream);

    if ( _obj->hasConstructorFailed() ) {
        delete _obj;
        return (HnSRTreeBlockObj *)NULL;
    }

    return _obj;
}

#define HnClass HnSRTreeBlock
#include "HnSRTree/HnClassArray.cc"

