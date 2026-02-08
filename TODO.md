# Tracker

## Done

- back-tested tsoding plasma against Xor version
  - tsoding plasma works in twigl and app
  - Xor plasma working in app
    - implement and test full-featured GLSL library
- basic implementation
- option to write to a file or write to stdout
- add timing info to streaming script
- implement multithreaded PPM generation
  - multithreading
    - create a job queue using threads
    - create line buffer array
    - queue generation of each line assigned to line buffer line
    - process one line per thread
- add PPM renderer
  - SDL3
  - macOS & Linux
  - various rendering modes, including multithreading
- improve speed
  - now generates and renders >= 60 fps
  - thread per line PPM generator
  - texture-based SDL PPM renderer
- fix shader y-reflection bug (output mirrored vertically compared to original shader)
- shader function API using shader base class
- PPM output option
  - binary
  - text
- output RGBA format for debugging (hacked version for testing)
  - https://en.wikipedia.org/wiki/Netpbm
  - https://en.wikipedia.org/wiki/Netpbm#PAM_graphics_format
- use Makefile as primary build reference
- drop RGB8 rendering support, Alpha channel required for GLSL,
  this slows down the console app as it now has to traverse the render buffer
- refactored the GLSL library to use templates for swizzles
  - this enables FC.xy syntax rather than FC.xy(), making the C++ code more GLSL-like
- remove swizzle vector 'extension' feature as this is not part of the specification
  e.g. vec3 RGB.rgba is no longer supported
- add test suite for vector & swizzle operations
- test GLSL & add more shaders
- complete 1 week of compatibility testing and ironed out many of bugs
  - type promotion is now restricted to prevent unwanted side effects such as
    vec4 = vec3 = vec2 = 1 => {1,1,1,1} (incorrect) instead of {1,0,0,0} (correct)
  - renderer clamps output - this seems to be implicit and handled natively in a shader
  - math function behavior is mapped to std:: implementations in <cmath>
    principle: functions have a top-level base-type operation function with vector operations dependent on them
- add support for Raylib
  - implement pixel copy and texture buffer
- release/publication preparation
  - add documentation for release
  - add some design notes
  - add build-scripts back in to reduce any friction/noise

## Todo

- configure image generation settings
  - config file
  - parameterize width, height
  - parameterize fps, max_frames
- consider unifying swizzle and vector types
- support matrix operations
- reconstruct v.get() v.set() into v.at() v[] pattern
  - could be implemented by using an 'is-a' rather than a 'has-a'
    container pattern, but will make swizzling more difficult
- investigate Raylib buffer rendering/presentation
  - the Alpha blending does not behave as expected
