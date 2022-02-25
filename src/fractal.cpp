#include "fractal.h"

Fractal::Fractal(const Image& image, const double center_x, const double center_y, const double scale) {
    this->mimage                     =                        image;
    
    this->mcenter_x                  =                     center_x;
    this->mcenter_y                  =                     center_y;

    this->mscale                     =                        scale;
#ifdef MOVIE_MODE
    this->mmin_iter                  =                          150;
    this->mmax_iter                  =         this->mmin_iter+1800;

    this->mnum_iter                  =              this->mmin_iter;
#else
    this->mnum_iter                  =                        100.0;
#endif
    this->mmin_skip                  =                            1;

    this->mbilinear_interpolate_flag =                        false;
    this->madaptive_pixel_skip_flag  =                        false;

    this->mwidth                     =                image.width();
    this->mheight                    =               image.height();

    this->minv_width                 =           1.0 / this->mwidth;
    this->minv_height                =          1.0 / this->mheight;

    this->mratio                     = this->mwidth / this->mheight;
}

const void Fractal::generate() {
#ifdef PHOTO_MODE
    this->msolve_start = std::chrono::high_resolution_clock::now();
#endif
    // printf("%.16f, %.16f, %.16f\n", this->mcenter_x, this->mcenter_y, this->mscale);

    this->mdraw_start = std::chrono::high_resolution_clock::now();
    
    if (this->madaptive_pixel_skip_flag) {
        this->generate(0, 0, this->mheight, this->mwidth, this->mmin_skip*8);
    } else {
        for (unsigned int r = 0; r < this->mheight; r += this->mmin_skip) {
            for (unsigned int c = 0; c < this->mwidth; c += this->mmin_skip) {
                float f = this->solve_pixel(r+this->mmin_skip/2, c+this->mmin_skip/2);
                if (this->mmin_skip > 1) {
                    if (!this->mbilinear_interpolate_flag) {
                        this->nearest_neighbor(r, c, this->mmin_skip, f);
                    } else if (this->mmin_skip > 1) {
                        this->bilinear_interpolate(r, c, this->mmin_skip);
                    } else {
#ifdef COLOR
                        this->mimage.set_pixel(r, c, Pixel::float_to_RGB(f));
#else
                        this->mimage.set_pixel(r, c, 255*sqrtf(f));
#endif
                    }
                } else {
#ifdef COLOR
                        this->mimage.set_pixel(r, c, Pixel::float_to_RGB(f));
#else
                        this->mimage.set_pixel(r, c, 255*sqrtf(f));
#endif
                }
            }
        }
    }
    this->mdraw_stop = std::chrono::high_resolution_clock::now();
    this->mdraw_time = std::chrono::duration_cast<std::chrono::duration<double>>(this->mdraw_stop - this->mdraw_start).count();

#if !defined(MOVIE_MODE) && !defined(PHOTO_MODE)

    //Draw time is too slow. Cut the number of iterations by one-third.
    if (this->mdraw_time > 0.1) {
        this->two_thr_iter();
    
    //Draw time is a bit slow. Decrease the number of iterations.
    } else if (this->mdraw_time > 0.08) {
        this->dec_iter();
    
    //Draw time is a bit fast. Increase the number of iterations.
    } else if (this->mdraw_time < 0.05) {
        this->inc_iter();
    }

#endif
}

const void Fractal::nearest_neighbor(unsigned int r, unsigned int c, unsigned int pix_skip, float f) {
    for (unsigned int rr = r; rr < r+pix_skip; rr++) {
        for (unsigned int cc = c; cc < c+pix_skip; cc++) {
#ifdef COLOR
        this->mimage.set_pixel(rr, cc, Pixel::float_to_RGB(f));
#else
        this->mimage.set_pixel(rr, cc, 255*sqrtf(f));
#endif
        }
    }
}

const void Fractal::bilinear_interpolate(unsigned int r, unsigned int c, unsigned int pix_skip) {
    float tl_f = this->solve_pixel(r,                   c);
    float tr_f = this->solve_pixel(r,          c+pix_skip);
    float bl_f = this->solve_pixel(r+pix_skip,          c);
    float br_f = this->solve_pixel(r+pix_skip, c+pix_skip);

#ifdef COLOR
    Pixel tl_c = Pixel::float_to_RGB(tl_f);
    Pixel tr_c = Pixel::float_to_RGB(tr_f);
    Pixel bl_c = Pixel::float_to_RGB(bl_f);
    Pixel br_c = Pixel::float_to_RGB(br_f);
#endif                
    for (unsigned int rr = r; rr < r+pix_skip; rr++) {
        for (unsigned int cc = c; cc < c+pix_skip; cc++) {
            float u_mod_dist_to_0 =       (cc % pix_skip) / (float)pix_skip;
            float u_mod_dist_to_1 = 1.0f-((cc % pix_skip) / (float)pix_skip);

            float v_mod_dist_to_0 =       (rr % pix_skip) / (float)pix_skip;
            float v_mod_dist_to_1 = 1.0f-((rr % pix_skip) / (float)pix_skip);

#ifdef COLOR
            Pixel H1 = bl_c * u_mod_dist_to_1 + br_c * u_mod_dist_to_0;
            Pixel H2 = tl_c * u_mod_dist_to_1 + tr_c * u_mod_dist_to_0;

            this->mimage.set_pixel(rr, cc, H1 * v_mod_dist_to_0 + H2 * v_mod_dist_to_1);
#else
            float H1 = sqrtf(bl_f) * u_mod_dist_to_1 + sqrtf(br_f) * u_mod_dist_to_0;
            float H2 = sqrtf(tl_f) * u_mod_dist_to_1 + sqrtf(tr_f) * u_mod_dist_to_0;

            this->mimage.set_pixel(rr, cc, 255 * H1 * v_mod_dist_to_0 + 255 * H2 * v_mod_dist_to_1);
#endif
        }
    }
}

const float Fractal::solve_pixel(const unsigned int r, const unsigned int c) {
#ifdef HIGH_PRECISION
    long double R = this->mcenter_x+(this->mratio*this->mscale*(-1.0+2.0*c*this->minv_width ));
    long double I = this->mcenter_y+(             this->mscale*(-1.0+2.0*r*this->minv_height));

    long double oR = R;
    long double oI = I;

    long double tR;
    long double tI;
#else
    double R = this->mcenter_x+(this->mratio*this->mscale*(-1.0+2.0*c*this->minv_width ));
    double I = this->mcenter_y+(             this->mscale*(-1.0+2.0*r*this->minv_height));

    double oR = R;
    double oI = I;

    double tR;
    double tI;
#endif
#ifdef PHOTO_MODE
    
#endif
    for (unsigned int n = 0; n < this->mnum_iter; n++) {
        
        tR = R;
        tI = I;

        R = oR+(tR*tR - tI*tI);
        I = oI+(tR*tI + tR*tI);
        
        if (R*R + I*I > 4.0) {
            return n / (float)this->mnum_iter;
        }
    }
#ifdef PHOTO_MODE
    // this->msolve_stop = std::chrono::high_resolution_clock::now();
    this->msolve_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - this->msolve_start).count();
    

    printf("[");
    int pos = 100 * r / this->mheight;
    for (int p = 0; p < 100; p++) {
        if (p < pos) {printf("=");}
        else if (p == pos) {printf(">");}
        else {printf(" ");}
    }

    int pixels_complete = c + this->mheight * r;
    int pixels_left = this->mheight*this->mwidth - pixels_complete;
    int seconds = this->msolve_time;

    int remaining_hours   = seconds / 3600;
    int remaining_minutes = (seconds / 60) - (remaining_hours * 60);
    int remaining_seconds = seconds - (remaining_minutes * 60) - (remaining_hours * 3600);

    printf("] %.2f%% %02dh%02dm%02ds\r", 100 * r / this->mheight, remaining_hours, remaining_minutes, remaining_seconds);
#endif
    return 1.0f;
}

const void Fractal::generate(unsigned int tlr, unsigned int tlc, unsigned int brr, unsigned int brc, unsigned int pix_skip) {
    for (unsigned int r = tlr; r < brr; r += pix_skip) {
        for (unsigned int c = tlc; c < brc; c += pix_skip) {
            float tl_f = this->solve_pixel(r         , c         );
            float tr_f = this->solve_pixel(r         , c+pix_skip);
            float bl_f = this->solve_pixel(r+pix_skip, c         );
            float br_f = this->solve_pixel(r+pix_skip, c+pix_skip);
            
            float f_max = this->max_float(tl_f, this->max_float(tr_f, this->max_float(bl_f, br_f)));

            if (f_max > 0.9f && pix_skip > this->mmin_skip) {
                // Top Left
                this->generate(r,            c           , r+pix_skip/2, c+pix_skip/2, pix_skip/2);
                
                //Top Right
                this->generate(r,            c+pix_skip/2, r+pix_skip/2, c+pix_skip  , pix_skip/2);

                //Bottom Left
                this->generate(r+pix_skip/2, c           , r+pix_skip  , c+pix_skip/2, pix_skip/2);

                //Bottom Right
                this->generate(r+pix_skip/2, c+pix_skip/2, r+pix_skip  , c+pix_skip  , pix_skip/2);
            } else {
                if (!this->mbilinear_interpolate_flag) {
                    this->nearest_neighbor(r, c, pix_skip, f_max);
                } else  {
                    this->bilinear_interpolate(r, c, pix_skip);
                }
            }
        }
    }
}

#ifdef MOVIE_MODE
const void Fractal::write_movie() {
    for (int i = 0; i < this->mmax_iter - this->mmin_iter; i++) {
        printf("%%%.2f Complete...\n", 100 * i / (float)(this->mmax_iter - this->mmin_iter));
        this->generate();
        printf("Completed frame in %lf seconds.\n\n", this->mtime);

        const char* filename = ("imgs/" + std::to_string(i) + ".ppm").c_str();
        this->write_to_file(filename);

        this->inc_iter();
    }
}
#endif