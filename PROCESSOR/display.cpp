#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>

// #include "processor.h"
#include "proc-type.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"
#include "display.h"


int create_display(display_t* disp_set)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return P_ERROR;
    }

    SDL_Window* window = SDL_CreateWindow("Display",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          disp_set->len * PIXEL_SIZE, disp_set->high * PIXEL_SIZE,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

      if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return P_ERROR;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             disp_set->len, disp_set->high);

    disp_set->window = window;
    disp_set->renderer = renderer;
    disp_set->texture = texture;

    return P_OK;
}


int update_display(cpu_t* proc)
{
    assert(proc);

    SDL_Event cur_event = {};
    SDL_PollEvent(&cur_event);

    if (cur_event.type == SDL_QUIT)
    {
        return P_END;
    }

    SDL_UpdateTexture(proc->disp_set.texture,
                      NULL,
                      proc->VRAM,
                      (long unsigned int) proc->disp_set.len * sizeof(int));

    SDL_RenderClear(proc->disp_set.renderer);
    SDL_RenderCopy(proc->disp_set.renderer,
                   proc->disp_set.texture,
                   NULL,
                   NULL);
    SDL_RenderPresent(proc->disp_set.renderer);

    // SDL_Delay(10);

    return P_OK;
}


int destroy_display(cpu_t* proc)
{
    assert(proc);

    SDL_DestroyTexture(proc->disp_set.texture);
    SDL_DestroyRenderer(proc->disp_set.renderer);
    SDL_DestroyWindow(proc->disp_set.window);
    SDL_Quit();

    return P_OK;
}




