/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef PATTERNBASE_H
#define PATTERNBASE_H
#include <iostream>

namespace NodeBinding 
{

typedef enum {
  PATTERN_NO_REPEAT,
  PATTERN_REPEAT,
  PATTERN_REPEAT_X,
  PATTERN_REPEAT_Y
} PatternRepeatType;


class PatternBase 
{
public:
  PatternBase() = default;
  PatternBase(void * repeatInst, PatternRepeatType repeatType);
  ~PatternBase();
public:
  void * mRepeatInstance;
  PatternRepeatType mRepeatType;
};

}   

#endif
