#include "pixel.h"

Pixel::Pixel() {
    this->mr = 0;
    this->mg = 0;
    this->mb = 0;
}

Pixel::Pixel(const unsigned char c) {
    this->mr = c;
    this->mg = c;
    this->mb = c;
}

Pixel::Pixel(const unsigned char u, const unsigned char v) {
    this->mr = u;
    this->mg = v;
    this->mb = 0;
}

Pixel::Pixel(const unsigned char r, const unsigned char g, const unsigned char b) {
    this->mr = r;
    this->mg = g;
    this->mb = b;
}

Pixel::Pixel(const Pixel& p) {
    this->mr = p.mr;
    this->mg = p.mg;
    this->mb = p.mb;
}

Pixel& Pixel::operator=(const Pixel& p) {
    this->mr = p.mr;
    this->mg = p.mg;
    this->mb = p.mb;

    return *this;
}

Pixel Pixel::operator+(const Pixel& p) {
    return Pixel(
        this->mr + p.mr,
        this->mg + p.mg,
        this->mb + p.mb
    );
}

Pixel Pixel::operator*(const float f) {
    return Pixel(
        this->mr * f,
        this->mg * f,
        this->mb * f
    );
}

Pixel Pixel::operator/(const float f) {
    return Pixel(
        this->mr / f,
        this->mg / f,
        this->mb / f
    );
}

Pixel Pixel::toRGB() const {
    float mL = this->mr / 255.0f;
    float mA = this->mg / 255.0f;
    float mB = this->mb / 255.0f;

    float L = (2.0f*1.7321f*mL + 2.4495f*mA + 3.0f*1.4142f*mB)*0.1667f;
    float M = (2.0f*1.7321f*mL + 2.4495f*mA - 3.0f*1.4142f*mB)*0.1667f;
    float S = (             mL - 1.4142f*mA                  )*0.5774f;

    return {(unsigned char)( 4.4679f*L*255 - 3.5873f*M*255 + 0.1193f*S*255),
            (unsigned char)(-1.2186f*L*255 + 2.3809f*M*255 - 0.1624f*S*255),
            (unsigned char)( 0.0497f*L*255 - 0.2439f*M*255 + 1.2045f*S*255)};
}

Pixel Pixel::toLAB() const {
    float mR = this->mr / 255.0f;
    float mG = this->mg / 255.0f;
    float mB = this->mb / 255.0f;
    
    float L = 0.3811f*mR + 0.5783f*mG + 0.0402f*mB;
    float M = 0.1967f*mR + 0.7244f*mG + 0.0782f*mB;
    float S = 0.0241f*mR + 0.1288f*mG + 0.8444f*mB;

    return {(unsigned char)(255*(L + M +      S)*0.5774f),
            (unsigned char)(255*(L + M - 2.0f*S)*0.4082f),
            (unsigned char)(255*(L - M         )*0.7071f)};
}

Pixel Pixel::float_to_RGB(const float f) {
    float h = 360 * f;
    float v = sqrtf(f);

    float x = 1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f);

    int hi = h / 60;

    switch (hi) {
        case 0:  return Pixel(  255*v, 255*x*v,       0).toRGB(); break;
        case 1:  return Pixel(255*x*v,   255*v,       0).toRGB(); break;
        case 2:  return Pixel(      0,   255*v, 255*x*v).toRGB(); break;
        case 3:  return Pixel(      0, 255*x*v,   255*v).toRGB(); break;
        case 4:  return Pixel(255*x*v,       0,   255*v).toRGB(); break;
        case 5:  return Pixel(  255*v,       0,     x*v).toRGB(); break;
        default: return Pixel(               0         ).toRGB(); break;
    }
}