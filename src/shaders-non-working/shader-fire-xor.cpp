#pragma once

// clang-format on

#include "config.h"
#include "shfl-glsl-include.h"  // IWYU pragma: keep
#include "shfl-glsl-vector-math-v3.h"
#include "shfl-glsl-vector-v3.h"

namespace glsl_example {
namespace FireXor {
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

// https://www.shadertoy.com/view/msjXRK

// clang-format off
void render_fragment(const vec4 I, const vec2 iResolution, const t_vec_float iTime, vec4 &O) {
    //Time for animation
    t_vec_float t = iTime,
    //Raymarch loop iterator
    i,
    //Raymarched depth
    z,
    //Raymarch step size and "Turbulence" frequency
    //https://www.shadertoy.com/view/WclSWn
    d;

    //Raymarching loop with 50 iterations
    for (O *= i; i++ < 50.;
        //Add color and glow attenuation
        O += (sin(z / 3. + vec4(7, 2, 3, 0)) + 1.1) / d )
    {
        //Compute raymarch sample point
        vec3 p = z * normalize(vec3(I+I,0)-iResolution.xyy);
        //Shift back and animate
        p.z += 5. + cos(t);
        //Twist and rotate
        p.xz *= mat2(cos(p.y * .5 + vec4(0, 33, 11, 0))) 
        //Expand upward
        / max(p.y * .1 + 1., .1);
        //Turbulence loop (increase frequency)
        for (d = 2.; d < 15.; d /= .6)
            //Add a turbulence wave
            p += cos((p.yzx - vec3(t/.1, t, d) ) * d ) / d;
        //Sample approximate distance to hollow cone
        z += d = .01 + abs(length(p.xz) + p.y * .3 - .5) / 7.;
    }
    //Tanh tonemapping
    //https://www.shadertoy.com/view/ms3BD7
    O = tanh(O / 1e3);
}
// clang-format on

}  // namespace FireXor
}  // namespace glsl_example