#pragma once

// TODO
// - map to config file
// - map to command line parameters

#include <cstddef>
namespace glsl_example {
namespace cfg {
auto constexpr W = 16 * 60;  // 960
auto constexpr H = 9 * 60;   // 540
auto constexpr MAX_FRAMES = 240;
// change as required, anything below 1 means
// N_THREADS is determined by the OS/runtime
constexpr std::size_t N_THREADS = 0;

enum PPM_FMODE { BIN, TXT };

// Raylib settings

enum RAY_BUFFER_DISPLAY_MODE {
  RAY_BUFFER_DISPLAY_NONE,
  RAY_BUFFER_COPY_PIXEL,
  RAY_BUFFER_TO_TEXTURE
};

const RAY_BUFFER_DISPLAY_MODE ray_buffer_display_mode = RAY_BUFFER_TO_TEXTURE;

// SDL settings

enum SDL_BUFFER_DISPLAY_MODE {
  SDL_BUFFER_DISPLAY_NONE,
  SDL_BUFFER_COPY_PIXEL,  // draws on screen (slowest, most compatible)
  SDL_BUFFER_TO_TEXTURE_BYTE_COPY,   // shader writes to buffer, buffer byte
                                     // copied to texture, SDL_memcpy texture to
                                     // screen (allows byte re-ordering, can be
                                     // used for compatibility)
  SDL_BUFFER_TO_TEXTURE_BLOCK_COPY,  // shader writes to buffer, buffer block
                                     // copied to texture, SDL_memcpy texture to
                                     // screen (fastest, if the correct pixel
                                     // format is available and the machine
                                     // memory architecture is compatible)
};
auto constexpr sdl_buffer_display_mode = SDL_BUFFER_TO_TEXTURE_BLOCK_COPY;
auto constexpr SDL_VSYNC =
    true;  // each frame is synchronized with display refresh
auto constexpr SDL_FRAME_DELAY_MS =
    17;  // minimum delay between frames, 17ms ~= 60 fps
auto constexpr SDL_REPEAT_ANIMATION =
    true;  // if false, shaders run for MAX_FRAMES

auto constexpr info_text =
    "use the function keys to select a shader, ESC or q quits";
auto constexpr info_countdown_init = 300;

}  // namespace cfg
}  // namespace glsl_example
