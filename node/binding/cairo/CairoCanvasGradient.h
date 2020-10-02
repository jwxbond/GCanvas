// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include <napi.h>
#include <cairo.h>


namespace cairocanvas
{

class Gradient: public Napi::ObjectWrap<Gradient> 
{
  public:
    static void init(Napi::Env env);
    static Napi::Object NewInstance(const Napi::CallbackInfo &info);

    Gradient(const Napi::CallbackInfo &info);
    ~Gradient();

    void addColorStop(const Napi::CallbackInfo &info);
    
    void setupGradient(double x0, double y0, double x1, double y1);
    void setupGradient(double x0, double y0, double r0, double x1, double y1, double r1);
    inline cairo_pattern_t *pattern(){ return _pattern; }

  private:
    static Napi::FunctionReference constructor;
    cairo_pattern_t *_pattern;
};

}
