#include "platform.h"

#include <stdio.h>

int main() {
  printf("rev up those fryers!");

  p_start();

  while (p_is_running()) {
    p_process_events();
  }

  p_stop();
}
