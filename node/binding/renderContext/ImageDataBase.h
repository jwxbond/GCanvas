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
    ImageDataBase(int w, int h);
    virtual ~ImageDataBase();
    virtual uint8_t *GetData() = 0;
    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }
protected:
    int mWidth;
    int mHeight;
    uint8_t *mData;
};

}   

#endif
