/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef GRADIENTBASE_H
#define GRADIENTBASE_H
#include <iostream>

namespace NodeBinding 
{

typedef enum {
    GRADIENT_TYPE_LINEAR,
    GRADIENT_TYPE_RADIAL
} GradientType;

class GradientBase 
{
public:
    GradientBase() = default;
    GradientBase(double x0, double y0, double x1, double y1);
    GradientBase(double x0, double y0, double r0, double x1, double y1, double r1);
    virtual ~GradientBase() = default;
    virtual void addColorStop(double pos, std::string color) = 0;

public:
    GradientType mGradientType;
    double startX, startY, startR;
    double endX, endY, endR;
};

}   

#endif
