/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */


#include "ImageBase.h"

namespace NodeBinding
{

    ImageBase::ImageBase(short w, short h)
        : _width(w), _height(h)
    {
        
    }

    ImageBase::~ImageBase()
    {
        if( _data )
        {
            delete [] _data;
            _data = nullptr;
        }
    }
} // namespace NodeBinding
