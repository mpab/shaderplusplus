#include "config.h"
#include "raylib.h"
// force alpha channel to max value, otherwise raylib displays shaders too dark
// place before any sfhl-glsl* includes
#define SHFL_GLSL_FORCE_ALPHA_CHANNEL_MAX
#include "shaders-select.cpp"
#include "shfl-glsl-include.h" // IWYU pragma: keep

using namespace glsl_example;

int main(void) {
  SetTraceLogLevel(LOG_ERROR);

  auto num_workers = cfg::N_THREADS;
  auto render_pool = shfl::threading::Workers{num_workers};

  InitWindow(cfg::W, cfg::H, "glsl on raylib");

  SetTargetFPS(60);
  auto frame = 0;
  auto shader_id = 1;

  auto info_countdown = cfg::info_countdown_init;

  while (!WindowShouldClose() && !IsKeyPressed(KEY_Q)) {
    auto kp = GetKeyPressed();
    if (KEY_F1 <= kp && kp <= KEY_F12)
      shader_id = kp - KEY_F1 + 1;

    auto si = shader_select(shader_id);
    si.renderer->render_frame(&render_pool, frame++);
    std::string buffer_display_mode = " (no buffer)";
    BeginDrawing();
    ClearBackground(BLACK);
    auto ppm_buffer = &si.renderer->bitmap[0];
    switch (cfg::ray_buffer_display_mode) {
    case cfg::RAY_BUFFER_DISPLAY_NONE:
    default:
      break;
    case cfg::RAY_BUFFER_COPY_PIXEL:
      buffer_display_mode = " (buffer pixel copy)";
      for (auto line = 0; line != cfg::H; ++line) {
        for (auto col = 0; col != cfg::W; ++col) {
          Color p = {ppm_buffer[0], ppm_buffer[1], ppm_buffer[2],
                     ppm_buffer[3]};
          DrawPixel(col, line, p);
          ppm_buffer += shfl::glsl::sz_Fragment;
        }
      }
      break;
    case cfg::RAY_BUFFER_TO_TEXTURE:
      buffer_display_mode = " (buffer to texture)";
      Image ppm_image = {
          .data = ppm_buffer, // We can assign pixels directly to data
          .width = cfg::W,
          .height = cfg::H,
          .mipmaps = 1,
          .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
      Texture2D texture = LoadTextureFromImage(ppm_image);
      ppm_image.data = nullptr; // we're handling the memory management
      UnloadImage(ppm_image);   // belt-and-braces
      DrawTexture(texture, 0, 0, RAYWHITE);
      break;
    }
    if (!si.name.empty()) {
      DrawText((si.name + " - " + buffer_display_mode).c_str(), 0, 0, 20,
               LIGHTGRAY);
    }
    if (info_countdown > 1) {
      DrawText(cfg::info_text, info_countdown--, cfg::H / 2, 20, LIGHTGRAY);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}