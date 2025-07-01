#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

static bool go = false;
static int rotate = 0;
static float angle = 0.0;
static float angle_a = 0.0;
static float acceleration = 0.0;

static SDL_FPoint line_points[] = {
    {300, 300}, {320, 310}, {300, 320}, {300, 300}
};

static float center_x = 310;
static float center_y = 310;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define MAX_SPEED 200

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
            rotate = -1;
        } else if (event->key.scancode == SDL_SCANCODE_RIGHT){
            rotate = 1;
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
    const Uint64 now = SDL_GetTicks();
    const float elapsed = ((float) (now - last_time)) / 1000.0f;
    static SDL_FPoint line_points_final[] = {
        {300, 300}, {320, 310}, {300, 320}, {300, 300}
    };

    center_x += MAX_SPEED * acceleration * SDL_cosf(angle_a) * elapsed;
    center_y -= MAX_SPEED * acceleration * SDL_sinf(angle_a) * elapsed;
    for (int i = 0; i < SDL_arraysize(line_points); i++) {
        line_points[i].x += MAX_SPEED * acceleration * SDL_cosf(angle_a) * elapsed;
        line_points[i].y -= MAX_SPEED * acceleration * SDL_sinf(angle_a) * elapsed;
        line_points_final[i].x += MAX_SPEED * acceleration * SDL_cosf(angle_a) * elapsed;
        line_points_final[i].y -= MAX_SPEED * acceleration * SDL_sinf(angle_a) * elapsed;
    }
    if (go == true) {
        angle_a = angle;
        if (acceleration < 1) {
            acceleration += 3 * elapsed;
        }
    } else {
        if (acceleration > 0) {
            acceleration -= 1 * elapsed;
        }
    }

    if (rotate != 0) {
        if (rotate > 0) {
            //if (angle <= 0.0) {
            //    angle = 360.0;
            //}
            angle -= 5 * elapsed;
        } else {
            //if (angle >= 360.0) {
            //    angle = 0.0;
            //}
            angle += 5 * elapsed;
        }
        printf("%f\n", angle);

        for (int i = 0; i < SDL_arraysize(line_points); i++) {
            line_points_final[i].x = center_x + ((line_points[i].x-center_x)*SDL_cosf(angle)) + ((line_points[i].y-center_y)*SDL_sinf(angle));
            line_points_final[i].y = center_y + ((line_points[i].y-center_y)*SDL_cosf(angle)) - ((line_points[i].x-center_x)*SDL_sinf(angle));
        }
    }
    last_time = now;

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLines(renderer, line_points_final, SDL_arraysize(line_points_final));
    SDL_RenderPresent(renderer);


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)  {

}
