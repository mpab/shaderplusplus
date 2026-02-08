#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace ZigZag {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://twigl.app?ol=true&ss=-Ojr6e4wupCtLakfk26q
// https://thebookofshaders.com/edit.php#09/zigzag.frag

// uniform vec2 resolution;
// uniform vec2 mouse;
// uniform float time;

vec2 mirrorTile(vec2 _st, t_vec_float _zoom) {
  _st *= _zoom;
  if (fract(_st.y * 0.5) > 0.5) {
    _st.x = _st.x + 0.5;
    _st.y = 1.0 - _st.y;
  }
  return fract(_st);
}

t_vec_float fillY(vec2 _st, float _pct, float _antia) {
  return smoothstep(_pct - _antia, _pct, _st.y);
}

void render_fragment(const vec4 gl_FragCoord,
                     const vec2 resolution,
                     const t_vec_float t,
                     vec4& gl_FragColor) {
  vec2 st = gl_FragCoord.xy / resolution.xy;
  vec3 color = vec3(0.0);

  st = mirrorTile(st * vec2(1., 2.), 5.);
  t_vec_float x = st.x * 2.;
  t_vec_float a = floor(1. + sin(x * 3.14));
  t_vec_float b = floor(1. + sin((x + 1.) * 3.14));
  t_vec_float f = fract(x);

  color = vec3(fillY(st, mix(a, b, f), 0.01));

  gl_FragColor = vec4(color, 1.0);
}

}  // namespace ZigZag
}  // namespace glsl_example
