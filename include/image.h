#ifndef __ZRUSS_MANDELBROT_IMAGE_H__
#define __ZRUSS_MANDELBROT_IMAGE_H__

#include <cmath>
#include <iostream>
#include <vector>

#include "pixel.h"

class Image {
private:
    unsigned int        mwidth;
    unsigned int       mheight;

    #ifdef COLOR
    std::vector<Pixel>         mpixels;
    #else
    std::vector<unsigned char> mpixels;
    #endif
public:
    Image();
   ~Image();
    Image(const unsigned int s);
    Image(const unsigned int width, const unsigned int height);
    Image(const Image& img);
    Image& operator=(const Image& img);

    inline const unsigned int       width() const {return  this->mwidth;}
    inline const unsigned int      height() const {return this->mheight;}

    #ifdef COLOR
    inline const Pixel*              data() const {return this->mpixels.data();}

    inline const void           set_pixel(const unsigned int r, const unsigned int c, const Pixel& p) {if (c < this->mwidth && r < this->mheight) {this->mpixels[c + this->mwidth * r] = p;} else if (c >= this->mwidth) {if (r >= this->mheight) {this->mpixels[(this->mwidth-1) + this->mwidth * (this->mheight-1)] = p;} else {this->mpixels[(this->mwidth-1) + this->mwidth * r] = p;}} else {this->mpixels[c + this->mwidth * (this->mheight-1)] = p;}}
    inline const Pixel          get_pixel(const unsigned int r, const unsigned int c)           const {return (c < this->mwidth && r < this->mheight) ? this->mpixels[c + this->mwidth * r] : (c >= this->mwidth ? (r >= this->mheight ? this->mpixels[(this->mwidth-1) + this->mwidth * (this->mheight-1)] : this->mpixels[(this->mwidth-1) + this->mwidth * r]) : this->mpixels[c + this->mwidth * (this->mheight-1)]);}
    
    inline const void                wipe() {this->mpixels.assign(this->mwidth*this->mheight, {0});}
    #else
    inline const unsigned char*      data() const {return this->mpixels.data();}

    inline const void           set_pixel(const unsigned int r, const unsigned int c, const unsigned char p) {if (c < this->mwidth && r < this->mheight) {this->mpixels[c + this->mwidth * r] = p;} else if (c >= this->mwidth) {if (r >= this->mheight) {this->mpixels[(this->mwidth-1) + this->mwidth * (this->mheight-1)] = p;} else {this->mpixels[(this->mwidth-1) + this->mwidth * r] = p;}} else {this->mpixels[c + this->mwidth * (this->mheight-1)] = p;}}
    inline const unsigned char  get_pixel(const unsigned int r, const unsigned int c)                  const {return (c < this->mwidth && r < this->mheight) ? this->mpixels[c + this->mwidth * r] : (c >= this->mwidth ? (r >= this->mheight ? this->mpixels[(this->mwidth-1) + this->mwidth * (this->mheight-1)] : this->mpixels[(this->mwidth-1) + this->mwidth * r]) : this->mpixels[c + this->mwidth * (this->mheight-1)]);}
    
    inline const void                wipe() {this->mpixels.assign(this->mwidth*this->mheight, 0);}
    #endif
    const void resize(unsigned int width, unsigned int height);

    const void write_to_file(const char* filename);
};

#endif