#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Heavenly2Xor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=heavenly+2

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  t_vec_float i = 0., z = 0., d = 0.;
  for (; i++ < 1e2; o += (cos(z + t + vec4(0, 1, 2, 3)) + 1.1) / d) {
    vec3 p = z * normalize(FC.rgb * 2. - r.xyy);
    p.z -= t;
    for (d = 1.; d < 9.; d /= .7)
      p += cos(p.yzx * d + z * .2 - t * .1) / d;
    z += d = .02 + .1 * abs(p.y + 1.);
  }
  o = tanh(o / 2e3);
}

}  // namespace Heavenly2Xor
}  // namespace glsl_example
