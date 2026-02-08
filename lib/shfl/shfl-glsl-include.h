#pragma once

#if !defined SHFL_GLSL_VERSION
#define SHFL_GLSL_VERSION v003
#endif

// clang-format off
#include "shfl-threading.h"
#include "shfl-glsl-vector.h"
#include "shfl-glsl-vector-math.h"
#include "shfl-glsl-renderer.h"
// clang-format on

#undef VEC_TMPL_OP

#define GLSL_FRAGMENT_FN(SHADER_FN, H, W)                           \
  using namespace shfl::glsl;                                       \
  using shfl::glsl::abs;                                            \
  using shfl::glsl::cos;                                            \
  using shfl::glsl::exp;                                            \
  using shfl::glsl::floor;                                          \
  using shfl::glsl::round;                                          \
  using shfl::glsl::sin;                                            \
  void SHADER_FN(const vec4, const vec2, const t_vec_float, vec4&); \
  static Renderer* _renderer = nullptr;                             \
  Renderer* get_renderer() {                                        \
    if (!_renderer) {                                               \
      _renderer = new Renderer(SHADER_FN, H, W, cfg::MAX_FRAMES);   \
    }                                                               \
    return _renderer;                                               \
  }
