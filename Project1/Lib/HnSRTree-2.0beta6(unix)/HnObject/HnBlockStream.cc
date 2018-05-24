/*
 * HnBlockStream.cc
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2010/03/09 03:26:43
 */

#include "HnSRTree/HnBlockStream.hh"
#include "HnSRTree/HnBlockStreamObj.hh"

/*
 * HnBlockStream
 */

const HnBlockStream HnBlockStream::null;

HnBlockStream
new_HnBlockStream(int size)
{
    HnBlockStreamObj *_obj;

    _obj = new HnBlockStreamObj(size);

    if ( _obj->hasConstructorFailed() ) {
        delete _obj;
        return (HnBlockStreamObj *)NULL;
    }

    return _obj;
}

HnBlockStream
new_HnBlockStream(const HnBlockStream &parent, int offset, int size)
{
    HnBlockStreamObj *_obj;

    _obj = new HnBlockStreamObj(parent, offset, size);

    if ( _obj->hasConstructorFailed() ) {
        delete _obj;
        return (HnBlockStreamObj *)NULL;
    }

    return _obj;
}

#define HnClass HnBlockStream
#include "HnSRTree/HnClassArray.cc"

