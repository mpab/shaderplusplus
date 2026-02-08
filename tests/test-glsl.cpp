// #define VEC_LOG(...) printf(__VA_ARGS__)
#include <cassert>
#include <cstdio>

#include "shfl-glsl-include.h"  // IWYU pragma: keep

using namespace shfl::glsl;

static int current_passed, current_failed, total_passed, total_failed = 0;

// reset by summary
static bool approximate_comparison = false;
static bool quiet = false;

template <typename T>
void fmt_print_vf(const T& v, bool LF = true) {
  for (size_t i = 0; i != v.size(); ++i) {
    auto f = v.get(i);
    if (!i) {
      printf("(%f, ", f);
    } else if (i < v.size() - 1) {
      printf("%f, ", f);
    } else {
      if (LF)
        printf("%f)\n", f);
      else
        printf("%f)", f);
    }
  }
}

template <typename T1, typename T2>
bool check_if_equal_vf(const T1& v1, const T2& v2, bool log = true) {
  if (log) {
    printf("V");
    fmt_print_vf(v1);

    printf("C");
    fmt_print_vf(v2);
  }

  if (v1.size() != v2.size()) {
    printf("v1 sz %zu != v2 sz %zu\n", v1.size(), v2.size());
    if (log) {
      printf("FAIL\n");
    }
    return false;
  }

  bool equal = true;

  for (size_t i = 0; i != v1.size(); ++i) {
    auto v1v = v1.get(i);
    auto v2v = v2.get(i);
    if (!approximate_comparison) {
      equal = (v1v == v2v);
    } else {  // if (approximate_comparison)
      int f1 = v1.get(i) * 1000;
      int f2 = v2.get(i) * 1000;
      equal = !(abs(f1 - f2) > 1);
    }
    if (!equal)
      break;
  }

  if (log) {
    printf(equal ? "PASS\n" : "FAIL\n");
  }
  return equal;
}

#define USE(STMT) STMT = STMT

#define CHECK_INIT(STMNT, CHECKVEC, VEC)                          \
  if (!quiet) {                                                   \
    printf("%s\n", #STMNT);                                       \
  }                                                               \
  STMNT;                                                          \
  {                                                               \
    bool status = check_if_equal_vf(VEC, CHECKVEC, !quiet);       \
    if (status)                                                   \
      current_passed++;                                           \
    else {                                                        \
      printf("%s:%d - PROBLEM %s\n", __FILE__, __LINE__, #STMNT); \
      check_if_equal_vf(VEC, CHECKVEC);                           \
      current_failed++;                                           \
    }                                                             \
  }

#define CHECK(STMNT, CHECKVEC, VEC)                               \
  if (!quiet) {                                                   \
    printf("%s\n", #STMNT);                                       \
    printf("O");                                                  \
    fmt_print_vf(VEC);                                            \
  }                                                               \
  STMNT;                                                          \
  {                                                               \
    bool status = check_if_equal_vf(VEC, CHECKVEC, !quiet);       \
    if (status)                                                   \
      current_passed++;                                           \
    else {                                                        \
      printf("%s:%d - PROBLEM %s\n", __FILE__, __LINE__, #STMNT); \
      check_if_equal_vf(VEC, CHECKVEC);                           \
      current_failed++;                                           \
    }                                                             \
  }

#define CHECKN(STMNT, CHECKVEC, VEC)                              \
  if (!quiet) {                                                   \
    printf("%s\n", #STMNT);                                       \
    printf("O");                                                  \
    fmt_print_vf(VEC);                                            \
  }                                                               \
  {                                                               \
    bool status = check_if_equal_vf(VEC, CHECKVEC, !quiet);       \
    if (status)                                                   \
      current_passed++;                                           \
    else {                                                        \
      printf("%s:%d - PROBLEM %s\n", __FILE__, __LINE__, #STMNT); \
      check_if_equal_vf(VEC, CHECKVEC);                           \
      current_failed++;                                           \
    }                                                             \
  }

#define TEST_ADD 1
#define TEST_SUB 1
#define TEST_MUL 1
#define TEST_DIV 1
#define TEST_MATH 1
#define TEST_SWIZZLE 1

void banner(const char* msg, bool _noisy_logging = false) {
  printf("========================================\n");
  printf("%s\n", msg);
  current_passed = current_failed = 0;
  quiet = !_noisy_logging;
}

void summary() {
  printf("PASS: %d\nFAIL: %d\n", current_passed, current_failed);
  printf("----------------------------------------\n");
  quiet = true;
  approximate_comparison = false;
  total_passed += current_passed;
  total_failed += current_failed;
}

void overall_summary() {
  printf("TOTAL PASS: %d\nTOTAL FAIL: %d\n", total_passed, total_failed);
  printf("----------------------------------------\n");
}

int main() {
  banner("BASELINE");
  CHECK_INIT(vec2 v2f_1, vec2(), v2f_1);
  CHECK_INIT(vec2 v2f_2 = vec2(-1, -1), vec2(-1, -1), v2f_2);
  CHECK(v2f_1 = vec2(1, 1), vec2(1, 1), v2f_1);

  CHECK_INIT(vec3 v3f_1, vec3(), v3f_1);
  CHECK_INIT(vec3 v3f_2 = vec3(-1, -1, -1), vec3(-1, -1, -1), v3f_2);

  CHECK_INIT(vec4 v4f_1, vec4(), v4f_1);
  CHECK_INIT(vec4 v4f_2 = vec4(-1, -1, -1, -1), vec4(-1, -1, -1, -1), v4f_2);

  // not yet supported
  // print_v2f_inf("O", v1);
  // CHECK(v1 *= -v1, vec2(-1, -1), v1);
  // CHECK(v1 = -v1, vec2(1, 1), v1);
  // print_v2f_inf("O", v2);
  // CHECK(v2 = -v2, vec2(1, 1), v2);
  // quiet = true;
  summary();

  if (current_failed)
    return -1;

#if TEST_ADD
  banner("ADDITION");

  // auto v2a = v2f_1 + v2f_2;
  // fmt_print_vf(v2a);
  // auto v3a = v3f_1 + v3f_2;
  // fmt_print_vf(v3a);
  // auto v4a = v4f_1 + v4f_2;
  // fmt_print_vf(v4a);

  CHECK(v2f_1 = vec2(1, 2), vec2(1., 2.), v2f_1);
  CHECK(v2f_2 = vec2(3, 4), vec2(3., 4.), v2f_2);
  CHECK(v2f_2 = v2f_2 + v2f_1, vec2(4., 6.), v2f_2);
  CHECK(v2f_2 += 2., vec2(6., 8.), v2f_2);
  CHECK(v2f_2 += v2f_2, vec2(12., 16.), v2f_2);
  CHECK(v2f_1 = v2f_1, vec2(1., 2.), v2f_1);
  CHECK(v2f_2 = 1. + v2f_1, vec2(2., 3.), v2f_2);
  CHECK(v2f_2 = v2f_1 + 2., vec2(3., 4.), v2f_2);
  CHECK(v2f_1 = 1., vec2(1., 1.), v2f_1);
  CHECK(v2f_1 += v2f_1, vec2(2., 2.), v2f_1);
  CHECK(v2f_1 = vec2(1, 2), vec2(1., 2.), v2f_1);
  CHECK(v2f_1 = 1., vec2(1., 1.), v2f_1);
  CHECK(v2f_1 += 1., vec2(2., 2.), v2f_1);
  CHECK(v2f_1 = vec2(-1, -2), vec2(-1., -2.), v2f_1);
  CHECK(v2f_1 += -1., vec2(-2., -3.), v2f_1);
  v2f_1 = 1.;  // should fail with a dimension mismatch
  summary();
#endif  // TEST_ADD

#if TEST_SUB
  banner("SUBTRACTION");

  // auto v2s = v2f_1 - v2f_2;
  // fmt_print_vf(v2s);
  // auto v3s = v3f_1 - v3f_2;
  // fmt_print_vf(v3s);
  // auto v4s = v4f_1 - v4f_2;
  // fmt_print_vf(v4s);

  CHECK(v2f_1 = vec2({1., 2.}), vec2(1, 2), v2f_1);
  CHECK(v2f_2 = vec2(3., 4.), vec2(3, 4), v2f_2);
  CHECK(v2f_2 = v2f_2 - v2f_1, vec2(2, 2), v2f_2);
  CHECK(v2f_2 -= 2., vec2(0., 0.), v2f_2);
  CHECK(v2f_2 -= v2f_2, vec2(0., 0.), v2f_2);
  CHECK(v2f_2 -= v2f_1, vec2(-1, -2), v2f_2);
  CHECK(v2f_1 = v2f_1, vec2(1, 2), v2f_1);
  CHECK(v2f_2 = 1. - v2f_1, vec2(0, -1), v2f_2);
  CHECK(v2f_2 = v2f_1 - 2., vec2(-1, 0), v2f_2);
  CHECK(v2f_2 -= -2., vec2(1, 2), v2f_2);
  CHECK(v2f_2 = -2., vec2(-2, -2), v2f_2);
  CHECK(v2f_2 -= -2., vec2(0., 0.), v2f_2);
  // not yet supported
  // CHECK(v2 = -v1, vec2(-1, -2), v2);
  // not yet supported
  // CHECK(v2 = -v2, vec2(1, 2), v2);
  summary();
#endif  // TEST_SUB

#if TEST_MUL
  banner("MULTIPLICATION");

  // auto v2m = v2f_1 * v2f_2;
  // fmt_print_vf(v2m);
  // auto v3m = v3f_1 * v3f_2;
  // fmt_print_vf(v3m);
  // auto v4m = v4f_1 * v4f_2;
  // fmt_print_vf(v4m);

  CHECK(v2f_1 = vec2(1, 2), vec2(1, 2), v2f_1);
  CHECK(v2f_2 = vec2(3, 4), vec2(3, 4), v2f_2);
  CHECK(v2f_2 = v2f_2 * v2f_1, vec2(3, 8), v2f_2);
  CHECK(v2f_2 *= 2., vec2(6, 16), v2f_2);
  CHECK(v2f_2 *= v2f_2, vec2(36, 256), v2f_2);
  CHECK(v2f_2 *= v2f_1, vec2(36, 512), v2f_2);
  CHECK(v2f_1 = v2f_1, vec2(1, 2), v2f_1);
  CHECK(v2f_2 = 3. * v2f_1, vec2(3, 6), v2f_2);
  CHECK(v2f_2 = v2f_1 * 2., vec2(2, 4), v2f_2);
  CHECK(v2f_1 *= -1., vec2(-1, -2), v2f_1);
  CHECK(v2f_1 += -1. * v2f_1, vec2(0., 0.), v2f_1);

  {
    vec2 scale{2, 2};
    vec2 gl_FragCoord{320, 240};
    vec2 u_resolution = vec2(640, 480);
    CHECK(gl_FragCoord *= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.set(0, gl_FragCoord.get(0) * scale.get(0));
    gl_FragCoord.set(1, gl_FragCoord.get(1) * scale.get(1));
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.x = gl_FragCoord.x * scale.x;
    gl_FragCoord.y = gl_FragCoord.y * scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.x *= scale.x;
    gl_FragCoord.y *= scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);
  }

  {
    vec2 scale{2, 2};
    vec4 gl_FragCoord{
        320.,
        240.,
        0,
        0,
    };
    vec4 u_resolution = vec4(640., 480., .0, 0.);
    CHECKN(u_resolution, vec4(640, 480, 0, 0), u_resolution);
    // CHECK(gl_FragCoord *= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.set(0, gl_FragCoord.get(0) * scale.get(0));
    gl_FragCoord.set(1, gl_FragCoord.get(1) * scale.get(1));
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.x = gl_FragCoord.x * scale.x;
    gl_FragCoord.y = gl_FragCoord.y * scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.x *= scale.x;
    gl_FragCoord.y *= scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);
  }

  summary();
#endif  // TEST_MUL

#if TEST_DIV
  banner("DIVISION");

  // auto v2d = v2f_1 / v2f_2;
  // fmt_print_vf(v2d);
  // auto v3d = v3f_1 / v3f_2;
  // fmt_print_vf(v3d);
  // auto v4d = v4f_1 / v4f_2;
  // fmt_print_vf(v4d);

  CHECK(v2f_1 = vec2(1, 2), vec2(1, 2), v2f_1);
  CHECK(v2f_2 = vec2(3, 4), vec2(3, 4), v2f_2);
  CHECK(v2f_2 = v2f_2 / v2f_1, vec2(3, 2), v2f_2);
  CHECK(v2f_2 /= 2., vec2(1.5, 1), v2f_2);
  CHECK(v2f_2 /= v2f_2, vec2(1, 1), v2f_2);
  CHECK(v2f_2 /= v2f_1, vec2(1, 0.5), v2f_2);
  CHECK(v2f_1 = v2f_1, vec2(1, 2), v2f_1);
  CHECK(v2f_2 = 3. / v2f_1, vec2(3, 1.5), v2f_2);
  CHECK(v2f_2 = v2f_1 * 2., vec2(2, 4), v2f_2);
  CHECK(v2f_1 = -1., vec2(-1, -1), v2f_1);
  CHECK(v2f_1 *= -1., vec2(1, 1), v2f_1);
  CHECK(v2f_1 += 1., vec2(2, 2), v2f_1);
  CHECK(v2f_1 /= -1. * v2f_1, vec2(-1, -1), v2f_1);
  CHECK(v2f_1 = v2f_2, vec2(2, 4), v2f_1);
  CHECK(v2f_1 /= v2f_1, vec2(1, 1), v2f_1);

  {
    vec2 scale{.5, .5};
    vec2 gl_FragCoord{320, 240};
    vec2 u_resolution = vec2(640, 480);
    CHECK(gl_FragCoord /= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.set(0, gl_FragCoord.get(0) / scale.get(0));
    gl_FragCoord.set(1, gl_FragCoord.get(1) / scale.get(1));
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.x = gl_FragCoord.x / scale.x;
    gl_FragCoord.y = gl_FragCoord.y / scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2(320, 240);
    gl_FragCoord.x /= scale.x;
    gl_FragCoord.y /= scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);
  }

  {
    vec2 scale{.5, .5};
    vec4 gl_FragCoord{
        320.,
        240.,
        0,
        0,
    };
    vec4 u_resolution = vec4(640., 480., .0, 0.);
    CHECKN(u_resolution, vec4(640, 480, 0, 0), u_resolution);
    // CHECK(gl_FragCoord *= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.set(0, gl_FragCoord.get(0) / scale.get(0));
    gl_FragCoord.set(1, gl_FragCoord.get(1) / scale.get(1));
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.x = gl_FragCoord.x / scale.x;
    gl_FragCoord.y = gl_FragCoord.y / scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);

    gl_FragCoord = vec4({320., 240., 0., 0.});
    gl_FragCoord.x /= scale.x;
    gl_FragCoord.y /= scale.y;
    CHECKN(gl_FragCoord, u_resolution, gl_FragCoord);
  }

  summary();
#endif  // TEST_DIV

#if TEST_MATH
  banner("VECTOR MATH");
  float fr = 1.5;
  assert(!(fract(fr) - 0.5));
  CHECK(v2f_1 = vec2(1.5, 1.5), vec2(1.5, 1.5), v2f_1);
  CHECK(v2f_1 = fract(v2f_1), vec2(.5, .5), v2f_1);
  CHECK(v2f_1 = fract(fr), vec2(.5, .5), v2f_1);
  summary();
#endif  // TEST_MATH

#if TEST_SWIZZLE
  banner("SWIZZLING");
  approximate_comparison = true;
  CHECK(v3f_1 = vec3(1, 2, 3), vec3(1, 2, 3), v3f_1);
  CHECK(v3f_2 = vec3(4, 5, 6), vec3(4, 5, 6), v3f_2);
  // vec2 sw2 = v3f_1.rg;
  CHECKN(v3f_1, vec2(1, 2), v3f_1.rg);
  CHECK(v3f_1.rg = vec2(-1, -2), vec2(-1, -2), v3f_1.rg);
  CHECKN(v3f_1, vec3(-1, -2, 3), v3f_1);
  CHECK(v3f_2.rg = v3f_1.rg, vec3(-1, -2, 6), v3f_2);

  CHECK(v3f_2 = vec3(1.9, 2.8, 3.7), vec3(1.9, 2.8, 3.7), v3f_2);
  CHECK(v3f_1 = fract(v3f_2), vec3(.9, .8, .7), v3f_1);

  CHECK(v3f_2 = vec3(1.9, 2.8, 3.7), vec3(1.9, 2.8, 3.7), v3f_2);
  CHECK(v3f_2 = fract(v3f_2), vec3(.9, .8, .7), v3f_2);

  CHECK(v3f_2 = vec3(1.9, 2.8, 3.7), vec3(1.9, 2.8, 3.7), v3f_2);
  CHECK(v2f_1 = vec2(2.5, 3.6), vec2(2.5, 3.6), v2f_1);
  CHECK(v3f_2.rg = fract(v2f_1), vec3(.5, .6, 3.7), v3f_2);

  CHECK(v3f_2 = vec3(1.9, 2.8, 3.7), vec3(1.9, 2.8, 3.7), v3f_2);
  CHECK(v2f_1 = vec2(2.5, 3.6), vec2(2.5, 3.6), v2f_1);
  CHECK(v3f_2.rg = fract(v2f_1).rg, vec3(.5, .6, 3.7), v3f_2);

  CHECK(v3f_1 = vec3(1.1, 2.3, 6.6), vec3(1.1, 2.3, 6.6), v3f_1);
  CHECK(v3f_2 = vec3(2.3, 1.1, 7.7), vec3(2.3, 1.1, 7.7), v3f_2);
  CHECKN(v3f_2, v3f_2.yx, v3f_1.xy);
  CHECKN(v3f_2, v3f_2.xy, v3f_1.yx);
  CHECKN(v3f_1, v3f_1.xy, v3f_2.yx);
  CHECKN(v3f_1, v3f_1.yx, v3f_2.xy);

  CHECK(v2f_1 = vec2(2.5, 3.6), vec2(2.5, 3.6), v2f_1);

  // illegal - removed due to a misreading of example code
  // CHECKN(v2f_1.rgb, vec3(2.5, 3.6, 0), v2f_1.rgb);
  // CHECKN(v2f_1.xyz, vec3(2.5, 3.6, 0), v2f_1.xyz);

  {
    vec2 scale{2, 2};
    vec2 gl_FragCoord{320, 240};
    vec2 u_resolution = vec2(640, 480);
    CHECK(gl_FragCoord *= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2({320, 240});
    gl_FragCoord.xy = gl_FragCoord.xy * scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    gl_FragCoord = vec2({320, 240});
    gl_FragCoord *= scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // To Be Implemented?
    // gl_FragCoord = vec2({320, 240});
    // gl_FragCoord.xy *= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);
  }

  {
    vec2 scale{2, 2};
    vec4 gl_FragCoord{
        320.,
        240.,
        0,
        0,
    };
    vec4 u_resolution = vec4(640., 480., .0, 0.);
    CHECKN(u_resolution, vec4(640, 480, 0, 0), u_resolution);
    gl_FragCoord.xy = gl_FragCoord.xy * scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // gl_FragCoord = vec4(320., 240.);
    // gl_FragCoord *= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // To Be Implemented?
    // gl_FragCoord = vec4(320., 240.);
    // gl_FragCoord.xy *= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);
  }

  {
    vec2 scale{.5, .5};
    vec2 gl_FragCoord{320, 240};
    vec2 u_resolution = vec2(640, 480);
    CHECK(gl_FragCoord /= scale, u_resolution, gl_FragCoord);

    gl_FragCoord = vec2({320, 240});
    gl_FragCoord.xy = gl_FragCoord.xy / scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    gl_FragCoord = vec2({320, 240});
    gl_FragCoord /= scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // To Be Implemented?
    // gl_FragCoord = vec2({320, 240});
    // gl_FragCoord.xy /= scale;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // To Be Implemented?
    // gl_FragCoord = vec2({320, 240});
    // gl_FragCoord.xy /= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);
  }

  {
    vec2 scale{.5, .5};
    vec4 gl_FragCoord{
        320.,
        240.,
        0,
        0,
    };
    vec4 u_resolution = vec4(640., 480., .0, 0.);
    CHECKN(u_resolution, vec4(640, 480, 0, 0), u_resolution);
    gl_FragCoord.xy = gl_FragCoord.xy / scale.xy;
    CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // gl_FragCoord = vec4({320, 240, 0, 0});
    // gl_FragCoord /= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);

    // To Be Implemented?
    // gl_FragCoord = vec4({320, 240, 0, 0});
    // gl_FragCoord.xy /= scale.xy;
    // CHECKN(gl_FragCoord.xy, u_resolution.xy, gl_FragCoord.xy);
  }

  summary();
#endif  // TEST_SWIZZLE

  banner("TESTS SUMMARY");
  overall_summary();

  return 0;
}
//} // namespace glsl
//} // namespace shfl