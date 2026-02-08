#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace PivotalXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=pivotal
// https://twigl.app?ol=true&ss=-OksODyVm8x8-h3Qmkg0

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y / .2, c = fract(p) - .5 - p * .2;
  o = cos(atan(c.y, c.x) + vec4(0, 1, 2, 0) + t);
}

}  // namespace PivotalXor
}  // namespace glsl_example
