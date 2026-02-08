#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
// #define VEC_LOG(...) printf(__VA_ARGS__)

#if !defined VEC_LOG
#define VEC_LOG(...) /*printf(__VA_ARGS__)*/
#endif

// template 'signature' used to ensure template type matching
#if !defined VEC_TMPL_OP
#define VEC_TMPL_OP template <typename D, typename T, size_t ARR_SZ>
#endif

namespace shfl {
namespace glsl {

struct vec2;
struct vec3;
struct vec4;
constexpr size_t SWIZZLE_MAP_SZ = 4;

VEC_TMPL_OP
struct t_vec_base {
  std::array<T *, ARR_SZ> data_ref;
  size_t size() const { return data_ref.size(); }
  T get(const size_t i) const { return *data_ref.at(i); }
  void set(const size_t i, const T &v) { *data_ref.at(i) = v; }
};

VEC_TMPL_OP
void vec_copy(t_vec_base<D, T, ARR_SZ> &dest,
              const t_vec_base<D, T, ARR_SZ> &src) {
  for (size_t i = 0; i != dest.size(); ++i) {
    dest.set(i, src.get(i));
  }
}

VEC_TMPL_OP
void vec_add(t_vec_base<D, T, ARR_SZ> &a, const t_vec_base<D, T, ARR_SZ> &b,
             const t_vec_base<D, T, ARR_SZ> &c) {
  for (size_t i = 0; i != a.size(); ++i) {
    a.set(i, b.get(i) + c.get(i));
  }
}

VEC_TMPL_OP
void vec_sub(t_vec_base<D, T, ARR_SZ> &a, const t_vec_base<D, T, ARR_SZ> &b,
             const t_vec_base<D, T, ARR_SZ> &c) {
  for (size_t i = 0; i != a.size(); ++i) {
    a.set(i, b.get(i) - c.get(i));
  }
}

VEC_TMPL_OP
void vec_mul(t_vec_base<D, T, ARR_SZ> &a, const t_vec_base<D, T, ARR_SZ> &b,
             const t_vec_base<D, T, ARR_SZ> &c) {
  for (size_t i = 0; i != a.size(); ++i) {
    a.set(i, b.get(i) * c.get(i));
  }
}

VEC_TMPL_OP
void vec_div(t_vec_base<D, T, ARR_SZ> &a, const t_vec_base<D, T, ARR_SZ> &b,
             const t_vec_base<D, T, ARR_SZ> &c) {
  for (size_t i = 0; i != a.size(); ++i) {
    a.set(i, b.get(i) / c.get(i));
  }
}

VEC_TMPL_OP
struct t_vec : t_vec_base<D, T, ARR_SZ> {
  std::array<T, ARR_SZ> data_buf = {};
  // used by vecs for storage, ensures buffer ref points to internal buffer
  t_vec() {
    for (size_t i = 0; i != this->size(); ++i) {
      this->data_ref[i] = &data_buf[i];
    }
  }
};

VEC_TMPL_OP
struct t_swizzle : t_vec_base<D, T, ARR_SZ> {
  // std::array<T, ARR_SZ> data_buf = {};  // 'gapping' used for vector
  // extension
  t_swizzle(std::array<T *, ARR_SZ> vec_data) {
    for (size_t i = 0; i != this->data_ref.size(); ++i) {
      this->data_ref[i] = vec_data[i];
    }
  }

  t_swizzle operator=(t_swizzle b) { // unswizzle and reswizzle
    if (this->size() != b.size())
      throw("t_swizzle: swizzle size mismatch");
    vec_copy(*this, b);
    return *this;
  }

  t_swizzle operator=(D b) {
    if (this->size() != b.size())
      throw("t_swizzle: type D size mismatch");
    vec_copy(*this, b);
    return *this;
  }
};

typedef double t_vec_float;

// swizzle typedefs
using swzl2 = t_swizzle<vec2, t_vec_float, 2>;
using swzl3 = t_swizzle<vec3, t_vec_float, 3>;
using swzl4 = t_swizzle<vec4, t_vec_float, 4>;

using t_vec2f = t_vec<vec2, t_vec_float, 2>;
struct vec2 : t_vec2f {
  // IMPORTANT!
  // to ensure correct data binding in the constructor
  // the order of these attributes and associated swizzles
  // must be maintained
  t_vec_float &x = data_buf[0];
  t_vec_float &y = data_buf[1];
  t_vec_float &r = x;
  t_vec_float &g = y;
  t_vec_float &s = x;
  t_vec_float &t = y;

  swzl2 xy, &rg = xy, &st = xy;
  swzl2 yx, &gr = yx, &ts = yx;
  swzl3 xyy, &rgg = xyy, &stt = xyy;
  swzl4 xyyx, &rggr = xyyx, &stts = xyyx;

  // primary constructor
  explicit vec2(t_vec_float a, t_vec_float b)
      : xy({&x, &y}), yx({&y, &x}), xyy({&x, &y, &y}), xyyx({&x, &y, &y, &x}) {
    data_buf = {a, b};
  }
  // conversion and copy operations
  vec2(std::array<t_vec_float, 2> a) : vec2(a[0], a[1]) {}
  vec2(t_vec_float a) : vec2(a, a) {}
  vec2() : vec2(0., 0.) {}
  // - base type constructor: required for vector operation template macros
  vec2(const t_vec_base &b) : vec2(*b.data_ref[0], *b.data_ref[1]) {}
  vec2 operator=(vec2 b) {
    vec_copy(*this, b);
    return *this;
  }
  // frustrate any conversion shenanigans
  template <class T> vec2(T) = delete;
};

using t_vec3f = t_vec<vec3, t_vec_float, 3>;
struct vec3 : t_vec3f {
  // IMPORTANT!
  // to ensure correct data binding in the constructor
  // the order of these attributes and associated swizzles
  // must be maintained
  t_vec_float &x = data_buf[0];
  t_vec_float &y = data_buf[1];
  t_vec_float &z = data_buf[2];
  t_vec_float &r = x;
  t_vec_float &g = y;
  t_vec_float &b = z;
  t_vec_float &s = x;
  t_vec_float &t = y;
  t_vec_float &p = z;

  swzl2 xy, &rg = xy, &st = xy;
  swzl2 yx, &gr = yx, &ts = yx;
  swzl2 yz, &gb = yz, &tp = yx;
  swzl2 xz, &rb = xz, &sp = xz;
  swzl3 xyz, &rgb = xyz, &stp = xyz;
  swzl3 yzx, &gbr = yzx, &tps = yzx;

  // primary constructor
  explicit vec3(t_vec_float a, t_vec_float b, t_vec_float c)
      : xy({&x, &y}), yx({&y, &x}), yz({&y, &z}), xz({&x, &z}),
        xyz({&x, &y, &z}), yzx({&y, &z, &x}) {
    data_buf = {a, b, c};
  }
  // conversion and copy operations
  vec3(std::array<t_vec_float, 3> a) : vec3(a[0], a[1], a[2]) {}
  vec3(t_vec_float a) : vec3(a, a, a) {}
  vec3() : vec3(0., 0., 0.) {}
  vec3(vec2 v, t_vec_float z) : vec3(v.x, v.y, z) {}
  // - base type constructor: required for vector operation template macros
  vec3(const t_vec_base &b)
      : vec3(*b.data_ref[0], *b.data_ref[1], *b.data_ref[2]) {}
  vec3 operator=(vec3 b) {
    vec_copy(*this, b);
    return *this;
  }
  // frustrate any conversion shenanigans
  template <class T> vec3(T) = delete;
};

using t_vec4f = t_vec<vec4, t_vec_float, 4>;
struct vec4 : t_vec4f {
  // IMPORTANT!
  // to ensure correct data binding in the constructor
  // the order of these attributes and associated swizzles
  // must be maintained
  t_vec_float &x = data_buf[0];
  t_vec_float &y = data_buf[1];
  t_vec_float &z = data_buf[2];
  t_vec_float &w = data_buf[3];
  t_vec_float &r = x;
  t_vec_float &g = y;
  t_vec_float &b = z;
  t_vec_float &a = w;
  t_vec_float &s = x;
  t_vec_float &t = y;
  t_vec_float &p = z;
  t_vec_float &q = w;

  swzl2 xy, &rg = xy, &st = xy;
  swzl2 yx, &gr = yx, &ts = yx;
  swzl3 xyz, &rgb = xyz, &stp = xyz;
  swzl4 xyyx, &rggr = xyyx, &stts = xyyx;

  // primary constructor
  explicit vec4(t_vec_float a, t_vec_float b, t_vec_float c, t_vec_float d)
      : xy({&x, &y}), yx({&y, &x}), xyz({&x, &y, &z}), xyyx({&x, &y, &y, &x}) {
    data_buf = {a, b, c, d};
  }
  // conversion and copy operations
  vec4(std::array<t_vec_float, 4> a) : vec4(a[0], a[1], a[2], a[3]) {}
  vec4(t_vec_float a) : vec4(a, a, a, a) {}
  vec4() : vec4(0., 0., 0., 0.) {}
  vec4(vec3 v, t_vec_float w) : vec4(v.x, v.y, v.z, w) {}
  // conversion and copy operations
  // - base type constructor: required for vector operation template macros
  vec4(const t_vec_base &b)
      : vec4(*b.data_ref[0], *b.data_ref[1], *b.data_ref[2], *b.data_ref[3]) {}
  vec4 operator=(vec4 b) {
    vec_copy(*this, b);
    return *this;
  }
  // frustrate any conversion shenanigans
  template <class T> vec4(T) = delete;
};

// vector operation template macros
// operations follow a standard pattern as defined below
// defined as macros because of the arithmetic operators
// NOTE: specified to return a t_vec_base derived (caller) D

#define VEC_TMPL_OP_VEC_FN_VEC_VEC                                             \
  VEC_TMPL_OP                                                                  \
  D operator VEC_OP(const t_vec_base<D, T, ARR_SZ> &a,                         \
                    const t_vec_base<D, T, ARR_SZ> &b) {                       \
    D lval;                                                                    \
    VEC_FN(lval, a, b);                                                        \
    return lval;                                                               \
  }

#define VEC_TMPL_OP_VEC_OP_VEC_T                                               \
  VEC_TMPL_OP                                                                  \
  D operator VEC_OP(const t_vec_base<D, T, ARR_SZ> &a, const T b) {            \
    D vb;                                                                      \
    vb.data_buf.fill(b);                                                       \
    return a VEC_OP vb;                                                        \
  }

#define VEC_TMPL_OP_VEC_OP_T_VEC                                               \
  VEC_TMPL_OP                                                                  \
  D operator VEC_OP(const T a, const t_vec_base<D, T, ARR_SZ> &b) {            \
    D va;                                                                      \
    va.data_buf.fill(a);                                                       \
    return va VEC_OP b;                                                        \
  }

#define VEC_TMPL_OP_VEC_OPX_D_VEC                                              \
  VEC_TMPL_OP                                                                  \
  D operator VEC_OPX(D &a, const t_vec_base<D, T, ARR_SZ> &b) {                \
    VEC_FN(a, a, b);                                                           \
    return a;                                                                  \
  }

#define VEC_TMPL_OP_VEC_OPX_VEC_T                                              \
  VEC_TMPL_OP                                                                  \
  D operator VEC_OPX(t_vec_base<D, T, ARR_SZ> &a, const T b) {                 \
    D vb;                                                                      \
    vb.data_buf.fill(b);                                                       \
    VEC_FN(a, a, vb);                                                          \
    return a;                                                                  \
  }

// basic arithmetic: addition
#define VEC_FN vec_add
#define VEC_OP +
#define VEC_OPX +=

VEC_TMPL_OP_VEC_FN_VEC_VEC
VEC_TMPL_OP_VEC_OP_VEC_T
VEC_TMPL_OP_VEC_OP_T_VEC
VEC_TMPL_OP_VEC_OPX_D_VEC
VEC_TMPL_OP_VEC_OPX_VEC_T

#undef VEC_FN
#undef VEC_OP
#undef VEC_OPX

// basic arithmetic: subtraction
#define VEC_FN vec_sub
#define VEC_OP -
#define VEC_OPX -=

VEC_TMPL_OP_VEC_FN_VEC_VEC
VEC_TMPL_OP_VEC_OP_VEC_T
VEC_TMPL_OP_VEC_OP_T_VEC
VEC_TMPL_OP_VEC_OPX_D_VEC
VEC_TMPL_OP_VEC_OPX_VEC_T

#undef VEC_FN
#undef VEC_OP
#undef VEC_OPX

// basic arithmetic: multiplication
#define VEC_FN vec_mul
#define VEC_OP *
#define VEC_OPX *=

VEC_TMPL_OP_VEC_FN_VEC_VEC
VEC_TMPL_OP_VEC_OP_VEC_T
VEC_TMPL_OP_VEC_OP_T_VEC
VEC_TMPL_OP_VEC_OPX_D_VEC
VEC_TMPL_OP_VEC_OPX_VEC_T

#undef VEC_FN
#undef VEC_OP
#undef VEC_OPX

// basic arithmetic: division
#define VEC_FN vec_div
#define VEC_OP /
#define VEC_OPX /=

VEC_TMPL_OP_VEC_FN_VEC_VEC
VEC_TMPL_OP_VEC_OP_VEC_T
VEC_TMPL_OP_VEC_OP_T_VEC
VEC_TMPL_OP_VEC_OPX_D_VEC
VEC_TMPL_OP_VEC_OPX_VEC_T

#undef VEC_FN
#undef VEC_OP
#undef VEC_OPX

} // namespace glsl
} // namespace shfl
