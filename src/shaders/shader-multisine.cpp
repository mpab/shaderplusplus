#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Multisine {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.shadertoy.com/view/WsjcDz

// hack for now (I think this is required for antialiasing)
t_vec_float fwidth(t_vec_float f) {
  return f;
}

// clang-format off
float f[32] = { 2.,1000.,3.,4.,5., 6.,7.,8.,9.,11.,12.,150.,99.,
                     40.,1.,10000.,2.,1100.,12.,100.,1000.,400.,210.,112.,113.,131.,
                     1300.,110.,1139.,1108.,1313.,1.};
// clang-format on

void render_fragment(const vec4 fragCoord,
                     const vec2 iResolution,
                     const t_vec_float iTime,
                     vec4& fragColor) {
  vec2 uv = fragCoord.xy / iResolution.xy;
  uv *= vec2(4, 8);

  vec2 cell = floor(uv), xy = 2. * fract(uv) - 1.;  // TODO
  float index = cell.x + 4. * cell.y, time = xy.x + iTime,
        pct = sin(3.1415926 * f[int(index)] * time), v = xy.y - .5 * pct;

  fragColor = vec4(1. - abs(v) / fwidth(v) / 1.5);
}

}  // namespace Multisine
}  // namespace glsl_example
