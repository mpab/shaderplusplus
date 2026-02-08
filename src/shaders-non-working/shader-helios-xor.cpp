#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace HeliosXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?newest=helios
// https://twigl.app?ol=true&ss=-Okeon-HXWo235z6ihD-

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  t_vec_float i = 0., z = 0., d = 0.;
  for (; i++ < 6e1; o += vec4(z, 3, 1, 1) / d) {
    vec3 p = z * normalize(FC.rgb * 2. - r.xyy), a = p;
    p.z += 8.;
    for (d = 1.; d++ < 9.; a += sin(a * d + t + i).yzx / d)
      ;
    d = length(p) - 5.;  // fix sequence warning
    z += d = max(d, -d / 3.) * .5 + length(sin(a / .3 + z) + 1.) / 4e1;
  }
  o = tanh(o / 2e3);
}

}  // namespace HeliosXor
}  // namespace glsl_example
