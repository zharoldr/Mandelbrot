#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#include "fractal.h"

Fractal* FRACTAL = nullptr;

void draw() {
    FRACTAL->generate();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    #ifdef COLOR
    glDrawPixels(FRACTAL->width(), FRACTAL->height(), GL_RGB, GL_UNSIGNED_BYTE, FRACTAL->data());
    #else
    glDrawPixels(FRACTAL->width(), FRACTAL->height(), GL_LUMINANCE, GL_UNSIGNED_BYTE, FRACTAL->data());
    #endif
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'W':
        case 'w': FRACTAL->move_up();                     break;
        case 'A':
        case 'a': FRACTAL->move_left();                   break;
        case 'S':
        case 's': FRACTAL->move_down();                   break;
        case 'D':
        case 'd': FRACTAL->move_right();                  break;

        case 'E':
        case 'e': FRACTAL->zoom_in();                     break;
        case 'Q':
        case 'q': FRACTAL->zoom_out();                    break;

        case 'R':
        case 'r': FRACTAL->inc_pix_skip();                break;
        case 'F':
        case 'f': FRACTAL->dec_pix_skip();                break;

        case 'B':
        case 'b': FRACTAL->toggle_bilinear_interpolate(); break;

        case 'G':
        case 'g': FRACTAL->toggle_adaptive_pixel_skip();  break;

        case 'O':
        case 'o': FRACTAL->write_to_file("out.ppm");      break;

        case  27: exit(0);

        default: break;
    }
    draw();
}

void reshape(int width, int height) {
    FRACTAL->resize(width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {

    FRACTAL = new Fractal(Image(512, 512), 0, 0, 1);

#if !defined(MOVIE_MODE) && !defined(PHOTO_MODE)
    glutInit(&argc, argv);

#ifdef COLOR
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
#else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RED);
#endif

    glutInitWindowSize(FRACTAL->width(), FRACTAL->height());
    
    glutCreateWindow("MANDELBROT");

    glutIdleFunc(idle);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    printf("================================================\n");
    printf("| MANDELBROT VIEWER : Created by Zachary Russ  |\n");
    printf("================================================\n");
    printf("| This program allows you to explore the       |\n");
    printf("| Mandelbrot Set! The amount of detail you are |\n");
    printf("| able to see is limited internally to keep    |\n");
    printf("| the viewer from lagging too much. If         |\n");
    printf("| everything suddenly goes black, don't panic! |\n");
    printf("| Tap 'R' until you can see again :)           |\n");
    printf("================================================\n");
    printf("|  ESC: Quit                                   |\n");
    printf("| WASD: Move Around                            |\n");
    printf("|    Q: Zoom Out                               |\n");
    printf("|    E: Zoom In                                |\n");
    printf("|    R: Decrease Resolution (Increases Detail) |\n");
    printf("|    F: Increase Resolution (Decreases Detail) |\n");
    printf("|    G: Toggle Adaptive Pixel Skip             |\n");
    printf("|    B: Toggle Bilinear Interpolation          |\n");
    printf("|    O: Write Current Image to File            |\n");
    printf("================================================\n");

    glutMainLoop();
#elif defined(MOVIE_MODE)
    FRACTAL->write_movie();
#elif defined(PHOTO_MODE)
    FRACTAL->generate();
    FRACTAL->write_to_file("pic.ppm");
    printf("\n");
#endif
    return 0;
}
