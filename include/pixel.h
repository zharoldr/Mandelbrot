#ifndef __ZRUSS_MANDELBROT_PIXEL_H__
#define __ZRUSS_MANDELBROT_PIXEL_H__

#include <cmath>

class Pixel {
private:
    unsigned char mr;
    unsigned char mg;
    unsigned char mb;
public:
    Pixel();
    Pixel(const unsigned char c);
    Pixel(const unsigned char u, const unsigned char v);
    Pixel(const unsigned char r, const unsigned char g, const unsigned char b);
    Pixel(const Pixel& p);
    Pixel& operator=(const Pixel& p);

    Pixel  operator+(const Pixel& p);
    Pixel  operator*(const float  f);
    Pixel  operator/(const float  f);

    Pixel  toRGB() const;
    Pixel  toLAB() const;

    static Pixel  float_to_RGB(const float f);

    inline const unsigned char r() const {return this->mr;}
    inline const unsigned char g() const {return this->mg;}
    inline const unsigned char b() const {return this->mb;}

    inline const unsigned char l() const {return this->mr;}
    inline const unsigned char a() const {return this->mg;}
};

#endif