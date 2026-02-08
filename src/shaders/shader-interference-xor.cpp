#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace InterferenceXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=interference
// https://twigl.app?ol=true&ss=-OkoKgRL0jy4Ye8ktl51

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y, v = p - clamp(p, -.3, .3),
       l = vec2(atan(p.y, p.x), length(v));
  o = tanh((cos(l.x + sin(l.x / .2) * sin(l.y / .02 - t) * .4 + t +
                vec4(0, 2, 4, 0)) +
            1.2) *
           exp(cos(l.x / .2) * cos(l.y / .02 - t)) / l.y / 3e1) *
      clamp(l * r * .1, 0., 1.).y;
}

}  // namespace InterferenceXor
}  // namespace glsl_example
