#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace GigabitXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec3 p, v;
  t_vec_float i = 0, z = 0, d = 0, l = 0;
  for (; i++ < 8e1; o += (cos(i * .1 + t + vec4(6, 1, 2, 0)) + 1.) / d / z)
    p = z * normalize(FC.rgb * 2. - r.xyy),
    p = dot(v = normalize(cos(t / 4. - z * .1 + vec3(0, 2, 4))), p) * v +
        cross(v, p),
    z += d = .5 * length(vec2(cos(p.z + t / .1) * .1, length(cos(p.xy)) - .6));
  o = tanh(o / 3e2);
}

}  // namespace GigabitXor
}  // namespace glsl_example
