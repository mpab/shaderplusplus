// $ cc -O3 -o plasma plasma.cpp -lm
// $ ./plasma | mpv --mf-fps=60 --cache-pause=no -
// README.md

#include <stdio.h>
#include <cstddef>

#include <getopt.h>
#include "config.h"
#include "shaders-select.cpp"
#include "shfl-glsl-renderer.h"

using namespace glsl_example;

// https://netpbm.sourceforge.net/doc/ppm.html

// TODO: correctly parse command line parameters

int main(int argc, char** argv) {
  char path_buf[256];
  const char* output_path = path_buf;
  FILE* f = stdout;
  glsl_example::cfg::PPM_FMODE ppm_fmode = cfg::PPM_FMODE::TXT;
  auto num_workers = cfg::N_THREADS;
  auto render_pool = shfl::threading::Workers{num_workers};

  auto rinf = shader_select(4);

  for (int frame = 0; frame < cfg::MAX_FRAMES; ++frame) {
    rinf.renderer->render_frame(&render_pool, frame);
    auto ppm_buffer = &rinf.renderer->bitmap[0];

    // arse the command line parameters
    if (argc > 1) {
      snprintf(path_buf, sizeof(path_buf), "%s-%03d.ppm", argv[1], frame);
      f = fopen(output_path, "wb");
    }

    if (argc > 2) {
      ppm_fmode = cfg::PPM_FMODE::BIN;
    }

    fprintf(f, ppm_fmode == cfg::BIN ? "P6\n" : "P3\n");
    fprintf(f, "%d %d\n", cfg::W, cfg::H);
    fprintf(f, "255\n");

    if (ppm_fmode == cfg::BIN) {
      std::size_t idx = 0;
      while (idx < rinf.renderer->bitmap.size()) {
        fputc(ppm_buffer[idx + 0], f);  // red
        fputc(ppm_buffer[idx + 1], f);  // green
        fputc(ppm_buffer[idx + 2], f);  // blue
        idx += shfl::glsl::sz_Fragment;
      }
    } else {
      std::size_t idx = 0;
      while (idx < rinf.renderer->bitmap.size()) {
        auto r = ppm_buffer[idx + 0];
        auto g = ppm_buffer[idx + 1];
        auto b = ppm_buffer[idx + 2];
        fprintf(f, "%d %d %d\n", r, g, b);
        idx += shfl::glsl::sz_Fragment;
      }
    }

    if (argc > 1) {
      fclose(f);
      printf("Generated %s (%3d/%3d)\n", output_path, frame + 1,
             cfg::MAX_FRAMES);
    }
  }
  delete rinf.renderer;
  return 0;
}