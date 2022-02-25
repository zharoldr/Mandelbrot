#include "image.h"

Image::Image() {
    this->mwidth  = 0;
    this->mheight = 0;
}

Image::~Image() {
    this->mpixels.clear();
}

Image::Image(const unsigned int s) {
    this->mwidth  = s;
    this->mheight = s;

    #ifdef COLOR
    this->mpixels.assign(s*s, Pixel(0));
    #else
    this->mpixels.assign(s*s, 0);
    #endif
}

Image::Image(const unsigned int width, const unsigned int height) {
    this->mwidth  =  width;
    this->mheight = height;

    #ifdef COLOR
    this->mpixels.assign(width*height, Pixel(0));
    #else
    this->mpixels.assign(width*height, 0);
    #endif
}

Image::Image(const Image& img) {
    #ifdef COLOR
    this->mpixels.assign(img.mwidth*img.mheight, Pixel(0));
    #else
    this->mpixels.assign(img.mwidth*img.mheight, 0);
    #endif

    for (unsigned int i = 0; i < this->mpixels.size(); i++) {
        this->mpixels[i] = img.mpixels[i];
    }
}

Image& Image::operator=(const Image& img) {
    if (this->mwidth   != img.mwidth || this->mheight != img.mheight) {
        this->mwidth   =  img.mwidth;
        this->mheight  = img.mheight;
        this->mpixels.resize(img.mwidth*img.mheight);
    }

    for (unsigned int i = 0; i < this->mpixels.size(); i++) {
        this->mpixels[i] = img.mpixels[i];
    }

    return *this;
}

const void Image::resize(unsigned int width, unsigned int height) {
    this->mwidth  =  width;
    this->mheight = height;
    
    this->mpixels.resize(width*height);
}

const void Image::write_to_file(const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "P6\n%d %d 255\n", this->mwidth, this->mheight);
    for (int r = this->mheight-1; r >= 0; r--) {
        for (unsigned int c = 0; c < this->mwidth; c++) {
#ifdef COLOR
            fprintf(f, "%c%c%c", this->get_pixel(r, c).r(), this->get_pixel(r, c).g(), this->get_pixel(r, c).b());
#else
            fprintf(f, "%c%c%c", this->get_pixel(r, c), this->get_pixel(r, c), this->get_pixel(r, c));
#endif
        }
    }
    fclose(f);
}