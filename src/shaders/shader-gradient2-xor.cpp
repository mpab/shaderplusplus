#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Gradient2Xor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=gradient+2
// https://twigl.app?ol=true&ss=-OkoJ4yx6i_4krM5Fdkj

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y;
  o = (sin(p.y / .3 + vec4(9, 2, 0, 0)) + 1.) / (2. + dot(p, p));
}

}  // namespace Gradient2Xor
}  // namespace glsl_example
