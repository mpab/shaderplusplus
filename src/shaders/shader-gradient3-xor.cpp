#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Gradient3Xor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=gradient+3
// https://twigl.app?ol=true&ss=-OkoJOQ7tlst9ZxTl9Jx

// some color interference in the center

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y;
  o = (sin(tanh(p + p).y * 7. - vec4(0, 1, 2, 0)) + 1.) /
      (1.5 + .5 * fract(cos(dot(p, r)) * 4e4) + 3. * dot(p, p));
}

}  // namespace Gradient3Xor
}  // namespace glsl_example
