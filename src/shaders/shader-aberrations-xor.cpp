#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace AberrationsXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=aberrations
// https://twigl.app?ol=true&ss=-OkpnBXGSaXjqWVgwkuu

// buggy but runs
void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y * 4.;
  o = tanh(.3 * cos(p.x / .6 - p.y + vec4(6, 1, 2, 0)) *
           cos(p.x - t * cos(round(p.y / PI) * PI)) / cos(p.y));
}

}  // namespace AberrationsXor
}  // namespace glsl_example
