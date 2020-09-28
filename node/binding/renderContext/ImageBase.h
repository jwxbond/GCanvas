/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef IMAGEBASE_H
#define IMAGEBASE_H
#include <iostream>

namespace NodeBinding 
{
class ImageBase {
public:
    ImageBase() = default;
    ImageBase(short w, short h);
    ~ImageBase();

    virtual int textureId(){ 
        //TODO 
        return 0;
    }
    inline short width(){ return _width; }
    inline short height(){ return _height;}
    inline uint8_t* data(){ return _data; }

protected:
    short _width;
    short _height;
    uint8_t *_data;
};

}   

#endif
