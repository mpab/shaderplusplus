#pragma once

#include "shfl-glsl-include.h" // IWYU pragma: keep
#include <sstream>

#include "shaders/shader-aberrations-xor.cpp"
#include "shaders/shader-fadient.cpp"
#include "shaders/shader-gradient2-xor.cpp"
#include "shaders/shader-interference-xor.cpp"
#include "shaders/shader-multisine.cpp"
#include "shaders/shader-null.cpp"
#include "shaders/shader-pivotal-xor.cpp"
#include "shaders/shader-plasma-tsoding.cpp"
#include "shaders/shader-plasma-xor.cpp"
#include "shaders/shader-simplex-grid.cpp"
#include "shaders/shader-zigzag.cpp"

namespace glsl_example {

typedef struct {
  shfl::glsl::Renderer *renderer;
  std::string name;
} ShaderInfo;

static int _info_countdown = -1;
static int _current_selection = -1;

inline ShaderInfo shader_select(int select) {
  shfl::glsl::Renderer *current_shader = Null::get_renderer();
  std::string current_shader_name = "No Configured Shader";

  _info_countdown = (_current_selection != select) ? 60 : _info_countdown - 1;
  _current_selection =
      (_current_selection != select) ? select : _current_selection;

  switch (select) {
  case 1:
    current_shader = Fadient::get_renderer();
    current_shader_name = "Fadient";
    break;

  case 2:
    current_shader = glsl_example::ZigZag::get_renderer();
    current_shader_name = "ZigZag";
    break;

  case 3:
    current_shader = SimplexGrid::get_renderer();
    current_shader_name = "Simple Grid";
    break;

  case 4:
    current_shader = PlasmaXor::get_renderer();
    current_shader_name = "Xor Plasma";
    break;

  case 5:
    current_shader = PlasmaTsoding::get_renderer();
    current_shader_name = "Plasma (tsoding algorithm)";
    break;

  case 6:
    current_shader = Multisine::get_renderer();
    current_shader_name = "Multisine";
    break;

  case 7:
    current_shader = AberrationsXor::get_renderer();
    current_shader_name = "Xor Aberrations";
    break;

  case 8:
    current_shader = Gradient2Xor::get_renderer();
    current_shader_name = "Xor Gradient2";
    break;

  case 9:
    current_shader = InterferenceXor::get_renderer();
    current_shader_name = "Xor Interference";
    break;

  case 10:
    current_shader = PivotalXor::get_renderer();
    current_shader_name = "Xor Pivotal";
    break;

  default:
    break;
  }

  std::stringstream desc;
  desc << select << " - " << current_shader_name;
  return {.renderer = current_shader,
          .name = (_info_countdown > 0) ? desc.str() : ""};
}

} // namespace glsl_example
