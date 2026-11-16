#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gl3.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 500
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

int main(int argc, char *argv[])
{
    SDL_Window *win;
    SDL_GLContext glContext;
    int win_width, win_height;
    int running = 1;

    struct nk_context *ctx;
    struct nk_colorf bg;

    /* Pemboleh ubah kalkulator */
    static char display[256] = "";
    static double num1 = 0;
    static char op = 0;

    (void)argc;
    (void)argv;

    /* Inisialisasi SDL */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    win = SDL_CreateWindow("Kalkulator ZWDevs",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);

    /* Inisialisasi OpenGL & GLEW */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Gagal memulakan GLEW\n");
        exit(1);
    }

    ctx = nk_sdl_init(win);
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
    }

    bg.r = 0.10f; bg.g = 0.18f; bg.b = 0.24f; bg.a = 1.0f;

    /* Gelung Utama (Main Loop) */
    while (running)
    {
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_sdl_handle_grab();
        nk_input_end(ctx);

        /* UI Kalkulator */
        /* UI Kalkulator */
        if (nk_begin(ctx, "Kalkulator", nk_rect(20, 20, 360, 460),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
        {
            /* Paparan Skrin */
            nk_layout_row_dynamic(ctx, 60, 1);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_READ_ONLY, display, sizeof(display) - 1, nk_filter_default);

            /* Grid Butang (4 lajur) */
            nk_layout_row_dynamic(ctx, 55, 4);

            if (nk_button_label(ctx, "7")) strncat(display, "7", 255 - strlen(display));
            if (nk_button_label(ctx, "8")) strncat(display, "8", 255 - strlen(display));
            if (nk_button_label(ctx, "9")) strncat(display, "9", 255 - strlen(display));
            if (nk_button_label(ctx, "/")) strncat(display, " / ", 255 - strlen(display));

            if (nk_button_label(ctx, "4")) strncat(display, "4", 255 - strlen(display));
            if (nk_button_label(ctx, "5")) strncat(display, "5", 255 - strlen(display));
            if (nk_button_label(ctx, "6")) strncat(display, "6", 255 - strlen(display));
            if (nk_button_label(ctx, "*")) strncat(display, " * ", 255 - strlen(display));

            if (nk_button_label(ctx, "1")) strncat(display, "1", 255 - strlen(display));
            if (nk_button_label(ctx, "2")) strncat(display, "2", 255 - strlen(display));
            if (nk_button_label(ctx, "3")) strncat(display, "3", 255 - strlen(display));
            if (nk_button_label(ctx, "-")) strncat(display, " - ", 255 - strlen(display));

            if (nk_button_label(ctx, "C")) display[0] = '\0';
            if (nk_button_label(ctx, "0")) strncat(display, "0", 255 - strlen(display));

            /* Logik Kiraan (=) */
            if (nk_button_label(ctx, "=")) {
                double n1 = 0, n2 = 0, result = 0;
                char oper = 0;

                /* Ekstrak nombor 1, operasi, dan nombor 2 dari skrin */
                if (sscanf(display, "%lf %c %lf", &n1, &oper, &n2) == 3) {
                    if (oper == '+') result = n1 + n2;
                    else if (oper == '-') result = n1 - n2;
                    else if (oper == '*') result = n1 * n2;
                    else if (oper == '/') result = (n2 != 0) ? (n1 / n2) : 0;

                    sprintf(display, "%.2f", result);
                }
            }

            if (nk_button_label(ctx, "+")) strncat(display, " + ", 255 - strlen(display));
        }
        nk_end(ctx);

        /* Render ke Skrin */
        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
        SDL_GL_SwapWindow(win);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
