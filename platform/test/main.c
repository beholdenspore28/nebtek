#include "platform.h"
#include "glad/gl.h"

#include <stdio.h>

int main() {
  printf("rev up those fryers!");

  p_start(640, 480);

  glClearColor(0.35, 0.325, 0.732,1);

  while (p_is_running()) {
    glClear(GL_COLOR_BUFFER_BIT);
    p_process_events();
  }

  p_stop();
}
