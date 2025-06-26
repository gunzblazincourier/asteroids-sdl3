#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static bool go = false;
static int rotate = 0;
static float angle = 0.0;

static SDL_FPoint line_points[] = {
    {300, 300}, {310, 280}, {320, 300}, {300, 300}
};

static float center_x = 310;
static float center_y = 290;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Asteroids", "1.0", "asteroids-sdl3");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("asteroids", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_UP) {
            go = true;
        }

        if (event->key.scancode == SDL_SCANCODE_LEFT) {
            rotate = 1;
        } else if (event->key.scancode == SDL_SCANCODE_RIGHT){
            rotate = -1;
        }
    } else {
        go = false;
        rotate = 0;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    static SDL_FPoint line_points_final[] = {
        {300, 300}, {310, 280}, {320, 300}, {300, 300}
    };
    if (go == true) {
        center_x += 0.01;
        center_y += 0.01;
        for (int i = 0; i < SDL_arraysize(line_points); i++) {
            line_points[i].x += 0.01;
            line_points[i].y += 0.01;
            line_points_final[i].x += 0.01;
            line_points_final[i].y += 0.01;
        }
    }

    if (rotate != 0) {
        if (rotate > 0) {
            //if (angle <= 0.0) {
            //    angle = 360.0;
            //}
            angle -= 0.0001;
        } else {
            //if (angle >= 360.0) {
            //    angle = 0.0;
            //}
            angle += 0.0001;
        }
        printf("%f\n", angle);

        for (int i = 0; i < SDL_arraysize(line_points); i++) {
            line_points_final[i].x = center_x + ((line_points[i].x-center_x)*SDL_cosf(angle)) + ((line_points[i].y-center_y)*SDL_sinf(angle));
            line_points_final[i].y = center_y + ((line_points[i].y-center_y)*SDL_cosf(angle)) - ((line_points[i].x-center_x)*SDL_sinf(angle));
        }
    }
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLines(renderer, line_points_final, SDL_arraysize(line_points_final));
    SDL_RenderPresent(renderer);


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)  {

}
