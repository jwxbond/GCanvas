#ifndef WEBGLFRAMEBUFFER_H
#define WEBGLFRAMEBUFFER_H
#include <napi.h>
#include <GL/gl.h>
namespace NodeBinding
{
    class WebGLFrameBuffer : public Napi::ObjectWrap<WebGLFrameBuffer>
    {
    public:
        WebGLFrameBuffer(const Napi::CallbackInfo &info);
        static void Init(Napi::Env env);
        static Napi::Object NewInstance(Napi::Env env, const Napi::Value arg);
        inline GLuint getId() const { return mId; }

    private:
        GLuint mId = 0;
        static Napi::FunctionReference constructor;
    };
} // namespace NodeBinding
#endif