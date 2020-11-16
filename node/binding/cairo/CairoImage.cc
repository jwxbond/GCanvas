#include "CairoImage.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>


namespace cairocanvas
{

typedef struct {
    unsigned len;
    uint8_t *buf;
} read_closure_t;

Napi::FunctionReference Image::constructor;

void Image::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Image", {
        InstanceAccessor("src", &Image::getSrc, &Image::setSrc),
        InstanceAccessor("width", &Image::getWidth, nullptr),
        InstanceAccessor("height", &Image::getHeight, nullptr),
        InstanceAccessor("onload", &Image::getOnLoadCallback, &Image::setOnLoadCallback),
        InstanceAccessor("onerror", &Image::getOnErrorCallback, &Image::setOnErrorCallback),
    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Image", func);
}


Napi::Object Image::NewInstance(Napi::Env env)
{
    Napi::Object obj = constructor.New({});
    obj.Set("name", Napi::String::New(env, "image"));
    return obj;
}

Image::Image(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Image>(info)
{
    mCallbackSet = new ImageCallbackSet();
    _data = nullptr;
    _data_len = 0;
    _surface = NULL;
    width = height = 0;
}

Image::~Image()
{
    clearData();
    
    if(mDownloadImageWorker) {
        // mDownloadImageWorker
        // delete mDownloadImageWorker;
        // mDownloadImageWorker = nullptr;
    }

    if( mCallbackSet ) {
        delete mCallbackSet;
        mCallbackSet = nullptr;
    }
    mImageMemCached = nullptr;
}

Napi::Value Image::getSrc(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), src);
}



void Image::DownloadCallback(Napi::Env env, uint8_t *data, size_t size, std::string errMsg )
{
    if( data != nullptr && size > 0 )
    {
        //decode 
        cairo_status_t status = loadFromBuffer(data, size);
        if( CAIRO_STATUS_SUCCESS == status )
        {
            loaded();
        }

        //callback
        if( mCallbackSet->mOnLoadCallback )
        {
            mCallbackSet->mOnLoadCallback.Call({env.Undefined()});
        }
    }
    else
    {
        if( mCallbackSet->mOnErrorCallback )
        {
            mCallbackSet->mOnErrorCallback.Call({Napi::String::New(env, errMsg)});
        }
    }
}

void Image::setSrc(const Napi::CallbackInfo &info, const Napi::Value &value)
  {
    NodeBinding::checkArgs(info, 1);
    src = value.As<Napi::String>().Utf8Value();
    mImageMemCached=std::make_shared<ImageCached>();
    if (!mDownloadImageWorker)
    {
        mDownloadImageWorker = new ImageAsyncWorker(info.Env(), src, mImageMemCached, std::bind(&Image::DownloadCallback, this,  std::placeholders::_1, std::placeholders::_2,  std::placeholders::_3,  std::placeholders::_4) );
    }

    mDownloadImageWorker->Queue();
}

Napi::Value Image::getOnLoadCallback(const Napi::CallbackInfo &info)
{
    return mCallbackSet->mOnLoadCallback.Value();
}
Napi::Value Image::getOnErrorCallback(const Napi::CallbackInfo &info)
{
    return mCallbackSet->mOnErrorCallback.Value();
}

void Image::setOnLoadCallback(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    NodeBinding::checkArgs(info, 1);
    mCallbackSet->mOnLoadCallback = Napi::Persistent(value.As<Napi::Function>());
}

void Image::setOnErrorCallback(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    NodeBinding::checkArgs(info, 1);
    mCallbackSet->mOnErrorCallback = Napi::Persistent(value.As<Napi::Function>());
}

Napi::Value Image::getWidth(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), width);

}
Napi::Value Image::getHeight(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), height);
}

int Image::getWidth()
{
    return width;
}
int Image::getHeight()
{
    return height;
}

std::vector<unsigned char> &Image::getPixels()
{
    if (mImageMemCached)
    {
        //Cairo ARGB TO A
        // NodeBinding::pixelsConvertRGBAToARBG(&mImageMemCached->getPixels()[0], getWidth(), getHeight());
        return mImageMemCached->getPixels();
    }
    else
    {
        return emptyPixels;
    }
}

 cairo_surface_t* Image::getSurface()
 {
    if( !_surface ) 
    {
        ImageCached *imageCached = mImageMemCached.get();
        if (imageCached) 
        {
            //TODO decode to ragba
            unsigned int len = imageCached->getPixels().size();
            unsigned char *buf = imageCached->getPixels().data();

            // NodeBinding::PIC_FORMAT format = NodeBinding::parseFormat((char *)buf, len);
            // // if( format == NodeBinding::PNG_FORAMT)
            // {
            //     decodePNGToSurface(buf, len);
            // }
            // else if( format ==  NodeBinding::JPEG_FORMAT )
            // {
            //     decodeJPEGToSurface(buf, len);
            // }
            // else
            // {
            //     std::cout << "FORMAT ERROR " << std::endl;
            // }

            int width = imageCached->width;
            int height = imageCached->height;
            int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);

            std::cout  << "stride is " << stride << std::endl;

            //RGBA buffer
            unsigned char * buf2 = imageCached->getPixels().data();
            NodeBinding::pixelsConvertRGBAToARGB(buf2, width, height);

            _surface = cairo_image_surface_create_for_data( 
                buf2,
                CAIRO_FORMAT_ARGB32,
                width, 
                height, 
                stride);
        }
    }
    return _surface;
}

void Image::clearData()
{
    if (_surface) {
        cairo_surface_destroy(_surface);
        // Nan::AdjustExternalMemory(-_data_len);
        _data_len = 0;
        _surface = NULL;
    }

    delete[] _data;
    _data = nullptr;
    width = height = 0;
}

// Load image data form buffer
cairo_status_t Image::loadFromBuffer(uint8_t *buf, unsigned len) 
{
    uint8_t data[4] = {0};
    memcpy(data, buf, (len < 4 ? len : 4) * sizeof(uint8_t));

    if (isPNG(data)) return loadPNGFromBuffer(buf);
    // if (isJPEG(data)) 
    // {
    //     if (DATA_IMAGE == data_mode) 
    //         return loadJPEGFromBuffer(buf, len);
        // if (DATA_MIME == data_mode) 
            // return decodeJPEGBufferIntoMimeSurface(buf, len);
        // if ((DATA_IMAGE | DATA_MIME) == data_mode) {
        //     cairo_status_t status;
        //     status = loadJPEGFromBuffer(buf, len);
        //     if (status) 
        //         return status;
        //     return assignDataAsMime(buf, len, CAIRO_MIME_TYPE_JPEG);
        // }
    // }

    // errorInfo = "Unsupported image type";   
    return CAIRO_STATUS_READ_ERROR;
}


/*
 * Return UNKNOWN, SVG, GIF, JPEG, or PNG based on the filename.
 */

Image::type Image::extension(const char *filename) {
  size_t len = strlen(filename);
  filename += len;
  if (len >= 5 && 0 == strcmp(".jpeg", filename - 5)) return Image::JPEG;
//   if (len >= 4 && 0 == strcmp(".gif", filename - 4)) return Image::GIF;
  if (len >= 4 && 0 == strcmp(".jpg", filename - 4)) return Image::JPEG;
  if (len >= 4 && 0 == strcmp(".png", filename - 4)) return Image::PNG;
//   if (len >= 4 && 0 == strcmp(".svg", filename - 4)) return Image::SVG;
  return Image::UNKNOWN;
}
/*
 * Sniff bytes 0..1 for JPEG's magic number ff d8.
 */

int Image::isJPEG(uint8_t *data) 
{
    return 0xff == data[0] && 0xd8 == data[1];
}

/*
 * Sniff bytes 1..3 for "PNG".
 */

int Image::isPNG(uint8_t *data) 
{
    return 'P' == data[1] && 'N' == data[2] && 'G' == data[3];
}

// Load PNG data from buffer
cairo_status_t Image::loadPNGFromBuffer(uint8_t *buf) {
    read_closure_t closure;
    closure.len = 0;
    closure.buf = buf;
    _surface = cairo_image_surface_create_from_png_stream(readPNG, &closure);
    cairo_status_t status = cairo_surface_status(_surface);
    if (status) return status;
    return CAIRO_STATUS_SUCCESS;
}

// Read PNG data
cairo_status_t Image::readPNG(void *c, uint8_t *data, unsigned int len) {
    read_closure_t *closure = (read_closure_t *) c;
    memcpy(data, closure->buf + closure->len, len);
    closure->len += len;
    return CAIRO_STATUS_SUCCESS;
}

void Image::loaded() 
{
    state = COMPLETE;
    width = naturalWidth = cairo_image_surface_get_width(_surface);
    height = naturalHeight = cairo_image_surface_get_height(_surface);
    _data_len = naturalHeight * cairo_image_surface_get_stride(_surface);
    // Nan::AdjustExternalMemory(_data_len);
}



}