#ifndef PLATFORM_H
#define PLATFORM_H

void p_start(unsigned int width, unsigned int height);
_Bool p_is_running();
void p_process_events();
void p_stop();

#endif // PLATFORM_H
