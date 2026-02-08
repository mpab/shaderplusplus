#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Fadient {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://twigl.app?ol=true&ss=-OksIyycTb7mHHhA48Ud

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p = (FC.xy * 2. - r) / r.y;
  o.rg = p.rg;
}

}  // namespace Fadient
}  // namespace glsl_example
