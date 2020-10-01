#pragma once

#include "backend/Backend.h"
#include <napi.h>

namespace cairocanvas
{

class Backends : public Napi::ObjectWrap<Backends> {
  public:
    static void init(Napi::Env env);
};

}
