#pragma once

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace PlasmaTsoding {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf#file-plasma-cpp
// https://twigl.app?ol=true&ss=-OkoOGvL3J-NMwdaC4Bf

// NOTE: tsoding used a vec2 rather than the 'standard' vec4 for FC
// hence the FC.xy swizzle in this version
// the original version inverted the y coordinate which is corrected in the
// rendering backend

// clang-format off
void render_fragment(const vec4 FC,
                    const vec2 r,
                    const t_vec_float t,  // double
                    vec4& o) {
  vec2 p=(FC.xy*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));
  for(;i.y++<8.;o+=(sin(v.xyyx)+1.)*abs(v.x-v.y))v+=cos(v.yx*i.y+i+t)/i.y+.7;
  o=tanh(5.*exp(l.x-4.-p.y*vec4(-1,1,2,0))/o);
}
// clang-format on

}  // namespace PlasmaTsoding
}  // namespace glsl_example