#ifndef __ZRUSS_MANDELBROT_FRACTAL_H__
#define __ZRUSS_MANDELBROT_FRACTAL_H__

#include <chrono>
#include <iostream>

#include "image.h"

class Fractal {
private:
    Image                    mimage;

    double                mcenter_x;
    double                mcenter_y;

    double                   mscale;
    unsigned int          mnum_iter;
    unsigned int          mmin_skip;

    bool mbilinear_interpolate_flag;
    bool  madaptive_pixel_skip_flag;

    double                   mwidth;
    double                  mheight;

    double               minv_width;
    double              minv_height;

    double                   mratio;

    double               mdraw_time;
    double              msolve_time;

#ifdef MOVIE_MODE
    unsigned int          mmin_iter;
    unsigned int          mmax_iter;
#endif

    std::chrono::high_resolution_clock::time_point mdraw_start;
    std::chrono::high_resolution_clock::time_point  mdraw_stop;

#ifdef PHOTO_MODE
    std::chrono::high_resolution_clock::time_point msolve_start;
    std::chrono::high_resolution_clock::time_point  msolve_stop;
#endif
    inline const float max_float(const float f1, const float f2) const {return (f1 > f2) ? f1 : f2;}

    const void                           generate(unsigned int tlr, unsigned int tlc, unsigned int brr, unsigned int brc, unsigned int pix_skip);
    
    const void                    nearest_neighbor(unsigned int r, unsigned int c, unsigned int pix_skip, float f);
    const void                    bilinear_interpolate(unsigned int r, unsigned int c, unsigned int pix_skip);

public:
void draw();
    Fractal(const Image& image, const double center_x, const double center_y, const double scale);
    
    const float                       solve_pixel(const unsigned int r, const unsigned int c);

    const void                           generate();
    

    inline const void                     move_up() {this->mcenter_y += 0.1 * this->mscale;}
    inline const void                   move_left() {this->mcenter_x -= 0.1 * this->mscale;}
    inline const void                   move_down() {this->mcenter_y -= 0.1 * this->mscale;}
    inline const void                  move_right() {this->mcenter_x += 0.1 * this->mscale;}

    inline const void                    inc_iter() {this->mnum_iter++;}
    inline const void                    dec_iter() {if (this->mnum_iter > 10) this->mnum_iter--;}
    inline const void                   half_iter() {this->mnum_iter /= 2;}
    inline const void                two_thr_iter() {this->mnum_iter *= (0.66f);}

    inline const void                     zoom_in() {this->mscale *= 0.9;}
    inline const void                    zoom_out() {this->mscale *= 1.111111111;}

    inline const void                inc_pix_skip() {this->mmin_skip *= 2;}
    inline const void                dec_pix_skip() {if (this->mmin_skip > 1) this->mmin_skip /= 2;}

    inline const void toggle_bilinear_interpolate() {this->mbilinear_interpolate_flag = !this->mbilinear_interpolate_flag;}
    inline const void  toggle_adaptive_pixel_skip() {this->madaptive_pixel_skip_flag = !this->madaptive_pixel_skip_flag;}

    inline const void                      resize(unsigned int width, unsigned int height) {
                                                    this->mimage.resize(width, height);
                                                    this->mwidth = width; this->mheight = height;
                                                    this->minv_width = 1.0 / width;
                                                    this->minv_height = 1.0 / height;
                                                    this->mratio = width / (double)height;
                                            }

    inline const unsigned int               width() const {return  this->mimage.width();}
    inline const unsigned int              height() const {return this->mimage.height();}

    inline const unsigned int        get_num_iter() const {return this->mnum_iter;}

#ifdef MOVIE_MODE
    const void                       write_movie();
#endif

#ifdef COLOR
    inline const Pixel*                      data() const {return this->mimage.data();}
#else
    inline const unsigned char*              data() const {return this->mimage.data();}
#endif
    inline const void               write_to_file(const char* filename) {this->mimage.write_to_file(filename);}
};

#endif