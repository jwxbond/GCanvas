/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#include "PatternImplCairo.h"

#include <stddef.h>

// #include "Canvas.h"
// #include "Image.h"

namespace NodeBinding
{
  const cairo_user_data_key_t *pattern_repeat_key;

  PatternImplCairo::PatternImplCairo(cairo_surface_t *surface, repeat_type_t repeat)
  {
    _pattern = cairo_pattern_create_for_surface(surface);
    _repeat = repeat;
    cairo_pattern_set_user_data(_pattern, pattern_repeat_key, &_repeat, NULL);
  }

  repeat_type_t PatternImplCairo::get_repeat_type_for_cairo_pattern(cairo_pattern_t *pattern)
  {
    void *ud = cairo_pattern_get_user_data(pattern, pattern_repeat_key);
    return *reinterpret_cast<repeat_type_t *>(ud);
  }
  
  PatternImplCairo::~PatternImplCairo()
  {
    cairo_pattern_destroy(_pattern);
  }

} // namespace NodeBinding
