#include "Backend.h"
#include <string>

Napi::FunctionReference Backend::constructor;

void Backend::init(Napi::Env env)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "Backend", {});
  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Napi::Object Backend::NewInstance(Napi::Env env, int width, int height)
{
  Napi::Object obj = constructor.New({ 
      Napi::String::New(env,  "backend"),
      Napi::Number::New(env, width),
      Napi::Number::New(env, height) });
  obj.Set("name",  Napi::String::New(env, "backend"));
  return obj;
}

Backend::Backend(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<Backend>(info)
{
  this->name = info[0].As<Napi::String>().Utf8Value();
  this->width = info[1].As<Napi::Number>().Int32Value(); 
  this->height = info[2].As<Napi::Number>().Int32Value(); 
}

Backend::~Backend()
{
  this->destroySurface();
}

// void Backend::init(const Nan::FunctionCallbackInfo<v8::Value> &info) {
//   int width  = 0;
//   int height = 0;
//   if (info[0]->IsNumber()) width  = Nan::To<uint32_t>(info[0]).FromMaybe(0);
//   if (info[1]->IsNumber()) height = Nan::To<uint32_t>(info[1]).FromMaybe(0);

//   Backend *backend = construct(width, height);

//   backend->Wrap(info.This());
//   info.GetReturnValue().Set(info.This());
// }

void Backend::setCanvas(Canvas* _canvas)
{
  this->canvas = _canvas;
}


cairo_surface_t* Backend::recreateSurface()
{
  this->destroySurface();

  return this->createSurface();
}

cairo_surface_t* Backend::createSurface() {
    return nullptr;
}

cairo_surface_t* Backend::getSurface() {
  if (!surface) createSurface();
  return surface;
}

void Backend::destroySurface()
{
  if(this->surface)
  {
    cairo_surface_destroy(this->surface);
    this->surface = NULL;
  }
}


std::string Backend::getName()
{
  return name;
}

int Backend::getWidth()
{
  return this->width;
}
void Backend::setWidth(int width_)
{
  this->width = width_;
  this->recreateSurface();
}

int Backend::getHeight()
{
  return this->height;
}
void Backend::setHeight(int height_)
{
  this->height = height_;
  this->recreateSurface();
}

bool Backend::isSurfaceValid(){
  bool hadSurface = surface != NULL;
  bool isValid = true;

  cairo_status_t status = cairo_surface_status(getSurface());

  if (status != CAIRO_STATUS_SUCCESS) {
    error = cairo_status_to_string(status);
    isValid = false;
  }

  if (!hadSurface)
    destroySurface();

  return isValid;
}


BackendOperationNotAvailable::BackendOperationNotAvailable(Backend* backend,
  std::string operation_name)
  : backend(backend)
  , operation_name(operation_name)
{};

BackendOperationNotAvailable::~BackendOperationNotAvailable() throw() {};

const char* BackendOperationNotAvailable::what() const throw()
{
  std::string msg = "operation " + this->operation_name +
    " not supported by backend " + backend->getName();

  return msg.c_str();
};
