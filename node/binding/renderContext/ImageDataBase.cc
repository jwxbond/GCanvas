/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */


#include "ImageDataBase.h"

namespace NodeBinding
{

    ImageDataBase::ImageDataBase(int w, int h)
        : mWidth(w), mHeight(h)
    {
        
    }

    ImageDataBase::~ImageDataBase()
    {
        if( mData )
        {
            delete [] mData;
            mData = nullptr;
        }
    }
    uint8_t * ImageDataBase::GetData()
    {
        std::cout << " GetData implement in " << std::endl;
    }

} // namespace NodeBinding
