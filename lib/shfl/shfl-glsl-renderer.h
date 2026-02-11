#pragma once

// clang-format off
#include "shfl-glsl-include.h"
// clang-format on

namespace shfl {
namespace glsl {

typedef unsigned char shader_buffer_t;
const std::size_t sz_Fragment = sizeof(char) * 4;

typedef void (*t_fn_render_fragment)(const vec4 FC, const vec2 r,
                                     const t_vec_float t, vec4 &o);

struct Renderer {
  const int W;
  const int H;
  const int MAX_FRAMES;
  t_fn_render_fragment render_fragment;
  std::vector<shader_buffer_t> bitmap;

  virtual ~Renderer() {};
  Renderer(t_fn_render_fragment fn, size_t height, size_t width,
           size_t num_frames)
      : W(width), H(height), MAX_FRAMES(num_frames), render_fragment(fn),
        bitmap(height * width * sz_Fragment) {}
  void render_frame(threading::Workers *p_workers, const int FRAME) {
    threading::t_latch num_tasks(
        H); // Initialize latch with the number of lines to process

    for (int line = 0; line < H; line++) {
      p_workers->assign({
          threading::Task::Cmd::run,
          [this, FRAME, line,
           &num_tasks](std::vector<threading::t_task_param> const &) {
            // Lambda
            render_line(FRAME, line);
            num_tasks.count_down();
          },
          {} // Arguments
      });
    }
    num_tasks.wait(); // wait for frame to be rendered to buffer
  }

  void render_line(const int FRAME, const int nLINE) {
    float t = ((float)FRAME / MAX_FRAMES) * 2 * M_PI;
    int index = nLINE * W * sz_Fragment;
    const vec2 r = vec2(t_vec_float(W), t_vec_float(H));
    for (int x = 0; x < W; ++x) {
      vec4 o{0, 0, 0, 1};
      vec4 FC = vec4(t_vec_float(x), t_vec_float(H - 1 - nLINE), 0, 0);
      render_fragment(FC, r, t, o);
      o = clamp(o, 0.0, 1.0);
      bitmap[index++] = o.r * 255;
      bitmap[index++] = o.g * 255;
      bitmap[index++] = o.b * 255;
#ifdef SHFL_GLSL_FORCE_ALPHA_CHANNEL_MAX
      bitmap[index++] = 255; // hack needed for raylib
#else
      bitmap[index++] = o.a * 255;
#endif
    }
  }
};

} // namespace glsl
} // namespace shfl
