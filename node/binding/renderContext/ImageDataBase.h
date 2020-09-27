/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H
#include <iostream>

namespace NodeBinding 
{
class ImageDataBase {
public:
    ImageDataBase(uint8_t *data, int width, int height) : _width(width), _height(height), _data(data) {}
    virtual ~ImageDataBase();

    inline short width() { return _width; }
    inline short height() { return _height; }
    inline uint8_t *data() { return _data; }

protected:
    short _width;
    short _height;
    uint8_t *_data;
};

}   

#endif
