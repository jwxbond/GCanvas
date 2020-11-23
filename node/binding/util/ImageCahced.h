#ifndef IMAGE_POOL_H
#define IMAGE_POOL_H
#include <vector>
class ImageCached
{
public:
    std::vector<unsigned char> &getPixels(){
        return this->pixels;
    }
    unsigned int width = 0;
    unsigned int height = 0;

    size_t size(){
        return  this->pixels.size();
    }

    char *buffer;
    size_t bufferSize;

private:
    std::vector<unsigned char> pixels;

};
#endif