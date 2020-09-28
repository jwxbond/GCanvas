/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#pragma once
#include <cairo.h>

namespace NodeBinding
{
  typedef enum
  {
    NO_REPEAT, // match CAIRO_EXTEND_NONE
    REPEAT,    // match CAIRO_EXTEND_REPEAT
    REPEAT_X,  // needs custom processing
    REPEAT_Y   // needs custom processing
  } repeat_type_t;

  extern const cairo_user_data_key_t *pattern_repeat_key;

  class PatternImplCairo
  {
  public:
    static repeat_type_t get_repeat_type_for_cairo_pattern(cairo_pattern_t *pattern);
    PatternImplCairo(cairo_surface_t *surface, repeat_type_t repeat);
    inline cairo_pattern_t *pattern() { return _pattern; }

  private:
    ~PatternImplCairo();
    cairo_pattern_t *_pattern;
    repeat_type_t _repeat;
  };

} // namespace NodeBinding