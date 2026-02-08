# Design Notes

## Features

- Reproduces the look and feel and behavior of GLSL.
- Uses a combination of templates and macros to generate the glsl types at compile time.
- Minimal use of dynamic memory.
- GLSL Library is header-only.
- Applications use a single-file approach
  - a main source file which chains source dependencies.
- Multithreaded rendering using a thread pool approach
  (currently one thread task is assigned per line).
- Targets C++ 20 (but will probably work with C++17, and perhaps earlier).
- Designed for comprehension (anyone can read it...)
- Cross-platform (leverages SDL or Raylib for rendering).
- Internally the shaders write data to a flat memory array,
  which is then copied to a texture for the GUI applications,
  or written to stdout (sans alpha channel) for the console application.
- The PPM write can be sped up by using an RGB8 buffer,
  meaning that a PPM binary mode file is ostensibly the render buffer flushed to stdout.
  (This is currently not released, as GLSL compatibility is a priority.)

## Type Restrictions

To reduce the number of issues with template type matching,and subsequent incorrect selection of implementation, the data type for operations is restricted to double.  
This can be changed in the glsl library by redefining t_vec_float (not recommended, unless you like compiler warnings).  
e.g. GLSL float is mapped to double in the library.  

## Type Suport

It should be trivial to extend the library to support the other GLSL vector types.

## Shader Code

[shader-null.cpp](../src/shaders/shader-null.cpp)

```cpp
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

```

## Modifying GLSL shaders used in the GUI app

See [shaders-select.cpp](../src/shaders-select.cpp)

Shaders are configured in the following pattern
- include the shader header
- modify the case statement accordingly

e.g.

```cpp

#include "shaders/shader-gradient.cpp"

...
...

  switch (select) {
    case 1:
      current_shader = Gradient::get_renderer();
      current_shader_name = "Gradient";
      break;
```
