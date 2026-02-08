#pragma once

#include "config.h"             // supplies render buffer parameters
#include "shfl-glsl-include.h"  // IWYU pragma: keep

namespace glsl_example {
namespace Null {
// Helper macro for configuring the shader fragment renderer
// If you change the name of the fragment function, change it here too
GLSL_FRAGMENT_FN(render_fragment, cfg::H, cfg::W);

/*
 Entry point for the shader, maps to twigl/Shadertoy/Book Of Shaders code
 Change the type names to suit your implementation
 e.g.
twigl
void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o)

Shadertoy
void render_fragment(const vec4 fragCoord,
                     const vec2 iResolution,
                     const t_vec_float iTime,
                     vec4& fragColor)

Book of Shaders
void render_fragment(const vec4 gl_FragCoord,
                     const vec2 u_resolution,
                     const t_vec_float u_time,
                     vec4& gl_FragColor)

 Reminder: If you change the name of the function,
 remember to change it in the macro

 The rendering code is called for each pixel in the ARGB8 fragment buffer
 Important! t_vec_float should be used instead of float in the C++ shader code
 */
void render_fragment(const vec4 FC,
                     const vec2 r,
                     const t_vec_float t,
                     vec4& o) {}

}  // namespace Null
}  // namespace glsl_example
