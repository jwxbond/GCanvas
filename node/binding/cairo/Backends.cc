#include "Backends.h"

#include "backend/ImageBackend.h"



void Backends:: init(Napi::Env env){
  //TODO
  ImageBackend::init(env);
  // Nan::Set(target, Nan::New<String>("Backends").ToLocalChecked(), obj).Check();
}
