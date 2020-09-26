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

    virtual int GetTextureId();
    virtual short GetWidth();
    virtual short GetHeight();

};

}   

#endif
