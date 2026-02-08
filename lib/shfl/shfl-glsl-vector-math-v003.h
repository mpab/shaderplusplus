#pragma once

// https://wikis.khronos.org/opengl/Data_Type_(GLSL)#Vectors

#define _USE_MATH_DEFINES  // required on some platforms
#include <cmath>

#include "shfl-glsl-vector-v003.h"

namespace shfl {
namespace glsl {

/*
  Library functions - see https://docs.gl
*/

#ifndef PI
auto constexpr PI = t_vec_float(M_PI);
#endif

// https://docs.gl/sl4/min
inline t_vec_float min(t_vec_float a, t_vec_float b) {
  return std::min(a, b);
}

// https://docs.gl/sl4/max
inline t_vec_float max(t_vec_float a, t_vec_float b) {
  return std::max(a, b);
}

inline t_vec_float floor(const t_vec_float f) {
  return std::floor(f);
}

VEC_TMPL_OP D floor(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    *lval.data_ref.at(i) = floor(*a.data_ref.at(i));
  }
  return lval;
}

inline t_vec_float round(const t_vec_float f) {
  return std::round(f);
}

inline t_vec_float fract(const t_vec_float f) {
  return f - floor(f);
}
VEC_TMPL_OP D fract(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    *lval.data_ref.at(i) = fract(*a.data_ref.at(i));
  }
  return lval;
}

inline t_vec_float mix(const t_vec_float x,
                       const t_vec_float y,
                       const t_vec_float a) {
  // https://docs.gl/el3/mix
  // = x⋅(1−a)+y⋅a
  return x * (1. - a) + y * a;
}

inline t_vec_float clamp(const t_vec_float x,
                         const t_vec_float minVal,
                         const t_vec_float maxVal) {
  // https://docs.gl/el3/clamp
  // = min(max(x, minVal), maxVal).
  return min(max(x, minVal), maxVal);
}

VEC_TMPL_OP
D clamp(const t_vec_base<D, T, ARR_SZ>& a,
        const t_vec_float minVal,
        const t_vec_float maxVal) {
  D lval;
  for (size_t i = 0; i != a.size(); ++i) {
    lval.set(i, clamp(a.get(i), minVal, maxVal));
  }
  return lval;
}

inline t_vec_float smoothstep(const t_vec_float edge0,
                              const t_vec_float edge1,
                              const t_vec_float x) {
  // https://docs.gl/el3/smoothstep
  // genType t;  /* Or genDType t; */
  // t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
  // return t * t * (3.0 - 2.0 * t);
  t_vec_float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
  return t * t * (3.0 - 2.0 * t);
}

// https://docs.gl/sl4/dot
VEC_TMPL_OP
t_vec_float dot(const t_vec_base<D, T, ARR_SZ>& a,
                const t_vec_base<D, T, ARR_SZ>& b) {
  t_vec_float lval = 0.;
  for (size_t i = 0; i != a.size(); ++i) {
    lval += *a.data_ref.at(i) * *b.data_ref.at(i);
  }
  return lval;
}

// https://docs.gl/sl4/cross
VEC_TMPL_OP
D cross(const t_vec_base<D, T, ARR_SZ>& x, const t_vec_base<D, T, ARR_SZ>& y) {
  auto r = x.get(1) * y.get(2) - y.get(1) * x.get(2);
  auto g = x.get(2) * y.get(0) - y.get(2) * x.get(0);
  auto b = x.get(0) * y.get(1) - y.get(0) * x.get(1);
  return D(r, g, b);
}

inline t_vec_float abs(const t_vec_float a) {
  return std::abs(a);
}
VEC_TMPL_OP D abs(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    *lval.data_ref.at(i) = abs(*a.data_ref.at(i));
  }
  return lval;
}

inline t_vec_float exp(const t_vec_float a) {
  return std::exp(a);
}
VEC_TMPL_OP D exp(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    lval.set(i, exp(a.get(i)));
  }
  return lval;
}

inline t_vec_float sin(const t_vec_float a) {
  return std::sin(a);
}
VEC_TMPL_OP D sin(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    lval.set(i, sin(a.get(i)));
  }
  return lval;
}

inline t_vec_float cos(const t_vec_float a) {
  return std::cos(a);
}
VEC_TMPL_OP D cos(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    lval.set(i, cos(a.get(i)));
  }
  return lval;
}

// Hyperbolic Tangent
// https://mini.gmshaders.com/p/func-tanh
// inline t_vec_float tanh(t_vec_float x) {
//   t_vec_float exp_neg_2x = exp(-2.0 * x);
//   return -1.0 + 2.0 / (1.0 + exp_neg_2x);
// }

inline t_vec_float tanh(const t_vec_float a) {
  return std::tanh(a);
}

VEC_TMPL_OP D tanh(const t_vec_base<D, T, ARR_SZ>& a) {
  D lval;
  for (size_t i = 0; i != lval.size(); ++i) {
    lval.set(i, tanh(a.get(i)));
  }
  return lval;
}

// https://docs.gl/sl4/length
VEC_TMPL_OP
t_vec_float length(const t_vec_base<D, T, ARR_SZ>& a) {
  t_vec_float sum = 0;
  for (size_t i = 0; i != a.size(); ++i)
    sum += a.get(i) * a.get(i);  // x*x + x*y + ...
  return std::sqrt(sum);
}

// https://docs.gl/sl4/normalize
VEC_TMPL_OP
inline t_vec_base<D, T, ARR_SZ> normalize(const t_vec_base<D, T, ARR_SZ>& a) {
  auto l = length(a);
  return D(a / l);
}

inline t_vec_float atan(const t_vec_float a, const t_vec_float b) {
  return std::atan2(a, b);
}

}  // namespace glsl
}  // namespace shfl
