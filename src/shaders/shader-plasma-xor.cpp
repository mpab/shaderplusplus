#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace PlasmaXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.xordev.com/arsenal?alphabetical=plasma
// https://twigl.app?ol=true&ss=-OkoMDrM1nUPHooX-vGm

// clang-format off
void render_fragment(const vec4 FC, const vec2 r, const t_vec_float t, vec4 &o) {
vec2 p=(FC.xy*2.-r)/r.y,l,v=p*(1.-(l+=abs(.7-dot(p,p))))/.2;
  for(t_vec_float i=0.;i++<8.;o+=(sin(v.xyyx)+1.)*abs(v.x-v.y)*.2)
    v+=cos(v.yx*i+vec2(0,i)+t)/i+.7;
  o=tanh(exp(p.y*vec4(1,-1,-2,0))*exp(-4.*l.x)/o);
}
// clang-format on

}  // namespace PlasmaXor
}  // namespace glsl_example