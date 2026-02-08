#pragma once
#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace CosmicXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=cosmic

// vec2 p,q;
// for(float
// i,a;i++<3e1;o+=5e-3/(abs(length(q=p*mat2(1,-1,2,2)/(r.y-p-p.yx))-i/4e1)+1./r.y)*clamp(cos(a=atan(q.y,q.x)*ceil(i*.1)+t*sin(i*i)+i*i),.0,.6)*(cos(a-i+vec4(0,1,2,0))+1.))p=FC.xy-r*.6;

// https://twigl.app?ol=true&ss=-OhqRZQJMAISpdBOZq79

void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {
  vec2 p, q;
  for (float i, a; i++ < 3e1;
       o +=
       5e-3 /
       (abs(length(q = p * mat2(1, -1, 2, 2) / (r.y - p - p.yx)) - i / 4e1) +
        1. / r.y) *
       clamp(cos(a = atan(q.y, q.x) * ceil(i * .1) + t * sin(i * i) + i * i),
             .0, .6) *
       (cos(a - i + vec4(0, 1, 2, 0)) + 1.))
    p = FC.xy - r * .6;
};
}  // namespace CosmicXor
}  // namespace glsl_example
