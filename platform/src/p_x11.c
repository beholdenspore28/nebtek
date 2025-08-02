#ifdef __linux__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <glad/gl.h>
#include <glad/glx.h>

const int window_width = 800, window_height = 480;

struct p_state {
  _Bool is_running;
  Display *display;
  Window window;
  GLXContext context;
  Colormap colormap;
  XVisualInfo *visual_info;
};

static struct p_state internal_state = {0};

_Bool p_is_running(void) { return internal_state.is_running; }

void p_start(void) {
  internal_state.display = XOpenDisplay(NULL);
  if (internal_state.display == NULL) {
    printf("cannot connect to X server\n");
    exit(1);
  }

  int screen = DefaultScreen(internal_state.display);

  int glx_version = gladLoaderLoadGLX(internal_state.display, screen);
  if (!glx_version) {
    printf("Unable to load GLX.\n");
    exit(1);
  }
  printf("Loaded GLX %d.%d\n", GLAD_VERSION_MAJOR(glx_version),
         GLAD_VERSION_MINOR(glx_version));

  Window root = RootWindow(internal_state.display, screen);

  GLint visual_attributes[] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
  internal_state.visual_info =
      glXChooseVisual(internal_state.display, screen, visual_attributes);

  internal_state.colormap =
      XCreateColormap(internal_state.display, root,
                      internal_state.visual_info->visual, AllocNone);

  XSetWindowAttributes attributes;
  attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
  attributes.colormap = internal_state.colormap;

  internal_state.window =
      XCreateWindow(internal_state.display, root, 0, 0, window_width,
                    window_height, 0, internal_state.visual_info->depth,
                    InputOutput, internal_state.visual_info->visual,
                    CWColormap | CWEventMask, &attributes);

  XMapWindow(internal_state.display, internal_state.window);
  XStoreName(internal_state.display, internal_state.window,
             "[glad] GLX with X11");

  if (!internal_state.window) {
    printf("Unable to create window.\n");
    exit(1);
  }

  internal_state.context = glXCreateContext(
      internal_state.display, internal_state.visual_info, NULL, GL_TRUE);
  glXMakeCurrent(internal_state.display, internal_state.window,
                 internal_state.context);

  int gl_version = gladLoaderLoadGL();
  if (!gl_version) {
    printf("Unable to load GL.\n");
    exit(1);
  }
  printf("Loaded GL %d.%d\n", GLAD_VERSION_MAJOR(gl_version),
         GLAD_VERSION_MINOR(gl_version));

  XWindowAttributes gwa;
  XGetWindowAttributes(internal_state.display, internal_state.window, &gwa);
  glViewport(0, 0, gwa.width, gwa.height);
  glClearColor(0.4, 0.5, 0.6, 1.0);

  internal_state.is_running = true;
}

void p_process_events(void) {
  while (XPending(internal_state.display)) {
    XEvent xev;
    XNextEvent(internal_state.display, &xev);

    if (xev.type == KeyPress) {
      internal_state.is_running = true;
    }
  }

  glClear(GL_COLOR_BUFFER_BIT);

  glXSwapBuffers(internal_state.display, internal_state.window);
}

void p_stop(void) {
  glXMakeCurrent(internal_state.display, 0, 0);
  glXDestroyContext(internal_state.display, internal_state.context);

  XDestroyWindow(internal_state.display, internal_state.window);
  XFreeColormap(internal_state.display, internal_state.colormap);
  XCloseDisplay(internal_state.display);

  gladLoaderUnloadGLX();
}

#endif // __linux__
