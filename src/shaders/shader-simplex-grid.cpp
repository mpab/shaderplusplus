#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace SimplexGrid {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://twigl.app?ol=true&ss=-Ojr6e4wupCtLakfk26q
// https://thebookofshaders.com/edit.php#11/simplex-grid.frag

vec2 skew(vec2 st) {
  vec2 r = vec2(0.0);
  r.x = 1.1547 * st.x;
  r.y = st.y + 0.5 * r.x;
  return r;
}

vec3 simplexGrid(vec2 st) {
  vec3 xyz = vec3(0.0);

  vec2 p = fract(skew(st));
  if (p.x > p.y) {
    xyz.xy = 1. - vec2(p.x, p.y - p.x);
    xyz.z = p.y;
  } else {
    xyz.yz = 1. - vec2(p.x - p.y, p.y);
    xyz.x = p.x;
  }

  return fract(xyz);
}

void render_fragment(const vec4 gl_FragCoord,
                     const vec2 u_resolution,
                     const t_vec_float u_time,
                     vec4& gl_FragColor) {
  vec2 st = gl_FragCoord.xy / u_resolution.xy;
  vec3 color = vec3(0.0);

  // Scale the space to see the grid
  st *= 10.;

  // Show the 2D grid
  color.rg = fract(st);

  // Skew the 2D grid
  // color.rg = fract(skew(st));

  // Subdivide the grid into to equilateral triangles
  // color = simplexGrid(st);

  gl_FragColor = vec4(color, 1.0);
}

}  // namespace SimplexGrid
}  // namespace glsl_example
