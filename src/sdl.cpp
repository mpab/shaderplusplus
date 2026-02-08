#include "SDL3/SDL_scancode.h"
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "config.h"
#include "sdl-text.h"
#include "shaders-select.cpp"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

using namespace glsl_example;

auto constexpr ABGR8_BUFF_LEN = cfg::W * cfg::H;
static Uint32 abgr8_buffer[ABGR8_BUFF_LEN];
auto constexpr ABGR8_BUFF_SIZE_BYTES = cfg::W * cfg::H * sizeof(Uint32);

struct AppContext {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  SDL_FRect texture_rect;
  SDL_AppResult app_quit = SDL_APP_CONTINUE;
  shfl::threading::Workers render_pool;
  int frame;
  Uint64 ticks_ms;
  int shader_selection;
  int info_countdown;
};

SDL_AppResult SDL_Fail() {
  SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
  return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  // init the library, here we make a window so we only need the Video
  // capabilities.
  if (not SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_Fail();
  }

  if (SDL_APP_FAILURE == sdl3::text::initialize("Inter-VariableFont.ttf")) {
    return SDL_Fail();
  }

  // create a window
  SDL_Window* window = SDL_CreateWindow("Plasma", cfg::W, cfg::H, 0);
  if (not window) {
    return SDL_Fail();
  }

  // create a renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
  if (not renderer) {
    return SDL_Fail();
  }

  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                        SDL_TEXTUREACCESS_STREAMING, cfg::W, cfg::H);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
  SDL_SetRenderLogicalPresentation(renderer, cfg::W, cfg::H,
                                   SDL_LOGICAL_PRESENTATION_DISABLED);

  auto texProps = SDL_GetTextureProperties(texture);
  SDL_FRect texture_rect{.x = 0,
                         .y = 0,
                         .w = float(SDL_GetNumberProperty(
                             texProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0)),
                         .h = float(SDL_GetNumberProperty(
                             texProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0))};

  // print some information about the window
  SDL_ShowWindow(window);
  {
    int width, height, bbwidth, bbheight;
    SDL_GetWindowSize(window, &width, &height);
    SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
    SDL_Log("Window size: %ix%i", width, height);
    SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
    if (width != bbwidth) {
      SDL_Log("This is a highdpi environment.");
    }
  }

  // set up the application data, deleted when app dies
  auto num_workers = cfg::N_THREADS;
  *appstate =
      new AppContext{.window = window,
                     .renderer = renderer,
                     .texture = texture,
                     .texture_rect = texture_rect,
                     .render_pool = shfl::threading::Workers{num_workers},
                     .frame = 0,
                     .ticks_ms = SDL_GetTicks(),
                     .shader_selection = 1,
                     .info_countdown = cfg::info_countdown_init};

  if (cfg::SDL_VSYNC)
    SDL_SetRenderVSync(renderer, -1);  // enable vysnc

  SDL_Log("Application started successfully!");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  auto* app = (AppContext*)appstate;

  if (event->type == SDL_EVENT_QUIT) {
    app->app_quit = SDL_APP_SUCCESS;
  }

  auto kp = event->key.scancode;

  if (kp == SDL_SCANCODE_Q || kp == SDL_SCANCODE_ESCAPE)
    app->app_quit = SDL_APP_SUCCESS;
  else if (SDL_SCANCODE_F1 <= kp && kp <= SDL_SCANCODE_F12)
    app->shader_selection = kp - SDL_SCANCODE_F1 + 1;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  auto* app = (AppContext*)appstate;

  auto ticks_ms = SDL_GetTicks();

  if (ticks_ms - app->ticks_ms < cfg::SDL_FRAME_DELAY_MS) {
    return app->app_quit;
  }

  app->ticks_ms = ticks_ms;

  auto si = shader_select(app->shader_selection);

  si.renderer->render_frame(&app->render_pool, app->frame++);
  auto ppm_buffer = &si.renderer->bitmap[0];
  std::string buffer_display_mode = " (no buffer)";

  switch (cfg::sdl_buffer_display_mode) {
    case cfg::SDL_BUFFER_DISPLAY_NONE:
    default:
      break;

    case cfg::SDL_BUFFER_COPY_PIXEL:
      buffer_display_mode = " (buffer pixel copy)";
      for (auto line = 0; line != cfg::H; ++line) {
        for (auto col = 0; col != cfg::W; ++col) {
          auto r = ppm_buffer[0];
          auto g = ppm_buffer[1];
          auto b = ppm_buffer[2];
          auto a = ppm_buffer[3];
          ppm_buffer += shfl::glsl::sz_Fragment;
          SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
          SDL_RenderPoint(app->renderer, (float)col, float(line));
        }
      }
      break;
    case cfg::SDL_BUFFER_TO_TEXTURE_BYTE_COPY: {
      buffer_display_mode = " (buffer byte copy to texture)";
      for (auto idx = 0; idx != ABGR8_BUFF_LEN; ++idx) {
        // re-order this logic as (or if) required
        abgr8_buffer[idx] = ppm_buffer[0] + (ppm_buffer[1] << 8) +
                            (ppm_buffer[2] << 16) + (ppm_buffer[3] << 24);
        ppm_buffer += shfl::glsl::sz_Fragment;
      }

      int pitch;
      void* pixels;
      SDL_LockTexture(app->texture, NULL, &pixels, &pitch);
      SDL_memcpy(pixels, abgr8_buffer, ABGR8_BUFF_SIZE_BYTES);
      SDL_UnlockTexture(app->texture);
      SDL_RenderTexture(app->renderer, app->texture, NULL, NULL);
    } break;

    case cfg::SDL_BUFFER_TO_TEXTURE_BLOCK_COPY: {
      buffer_display_mode = " (buffer block copy to texture)";
      int pitch;
      void* pixels;
      SDL_LockTexture(app->texture, NULL, &pixels, &pitch);
      SDL_memcpy(pixels, ppm_buffer, ABGR8_BUFF_SIZE_BYTES);
      SDL_UnlockTexture(app->texture);
      SDL_RenderTexture(app->renderer, app->texture, NULL, NULL);
    } break;
  }

  if (!si.name.empty()) {
    sdl3::text::render(app->renderer,
                       std::format("{} - {}", si.name, buffer_display_mode));
  }
  if (app->info_countdown > 1) {
    if (app->info_countdown > 1) {
      sdl3::text::render(app->renderer, cfg::info_text, app->info_countdown--,
                         float(cfg::H) / 2);
    }
  }

  SDL_RenderPresent(app->renderer);

  if (app->frame >= cfg::MAX_FRAMES) {
    if (!cfg::SDL_REPEAT_ANIMATION)
      app->app_quit = SDL_APP_SUCCESS;
    else
      app->frame = 0;
  }

  return app->app_quit;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  auto* app = (AppContext*)appstate;
  if (app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    delete app;
  }

  SDL_Log("Application quit successfully!");
  SDL_Quit();
}
