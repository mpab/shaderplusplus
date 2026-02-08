#pragma once

#include <filesystem>
#include <string_view>
#include "SDL3/SDL_assert.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"

namespace glsl_example {
namespace sdl3 {
namespace text {

static TTF_Font* _font = nullptr;
static bool _initialised = false;

inline SDL_AppResult initialize(std::string_view font_name, float ptsize = 24) {
  SDL_assert(!_initialised);
  _initialised = true;

  if (not TTF_Init()) {
    return SDL_APP_FAILURE;
  }

  SDL_assert(!_font);

#if __ANDROID__
  std::filesystem::path basePath =
      "";  // on Android we do not want to use basepath. Instead, assets are
           // available at the root directory.
#else
  auto basePathPtr = SDL_GetBasePath();
  if (not basePathPtr) {
    return SDL_APP_FAILURE;
  }
  const std::filesystem::path basePath = basePathPtr;
#endif
  const auto fontPath = basePath / font_name;
  _font = TTF_OpenFont(fontPath.string().c_str(), ptsize);
  if (not _font) {
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

inline void cleanup() {
  // we no longer need the font or the surface, so we can destroy those now.
  TTF_CloseFont(_font);
  _font = nullptr;
}

inline void render(SDL_Renderer* renderer,
                   std::string_view text,
                   float x = 0,
                   float y = 0) {
  // render the font to a surface
  auto* surface =
      TTF_RenderText_Solid(_font, text.data(), text.length(), {255, 255, 255});

  // make a texture from the surface
  auto texture = SDL_CreateTextureFromSurface(renderer, surface);

  // we no longer need the font or the surface, so we can destroy those now.
  SDL_DestroySurface(surface);

  auto messageTexProps = SDL_GetTextureProperties(texture);
  SDL_FRect text_frect = {
      .x = x,
      .y = y,
      .w = float(SDL_GetNumberProperty(messageTexProps,
                                       SDL_PROP_TEXTURE_WIDTH_NUMBER, 0)),
      .h = float(SDL_GetNumberProperty(messageTexProps,
                                       SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0))};
  SDL_RenderTexture(renderer, texture, NULL, &text_frect);

  SDL_DestroyTexture(texture);
}
}  // namespace text
}  // namespace sdl3
}  // namespace glsl_example
