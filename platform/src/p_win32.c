#ifdef __GNUC__
#ifdef __MINGW32__

#include <windows.h>
#include <stdio.h>

#include "glad/gl.h"
#include "glad/wgl.h"

struct p_state {
  _Bool is_running;
  unsigned int width, height;

  HWND hwnd;
  MSG msg;
  HDC hdc;
};

static struct p_state internal_state = {0};

_Bool p_is_running(void) { return internal_state.is_running; }

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
  case WM_SIZE: {
    int width = LOWORD(lParam);  // Macro to get the low-order word.
    int height = HIWORD(lParam); // Macro to get the high-order word.
    internal_state.width = width;
    internal_state.height = height;
    printf("window size: %d %d", width, height);
  } break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void p_start(void) {

  HINSTANCE hInstance = GetModuleHandle(NULL);
  STARTUPINFO si;
  GetStartupInfo(&si);  
  int nCmdShow = si.wShowWindow;

  // Register the window class.
  const char class_name[] = "Sample Window Class";
  const char window_title[] = "Game Window";

  WNDCLASS wc = {0};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = class_name;

  RegisterClass(&wc);

  // Create the window.

  internal_state.hwnd =
      CreateWindowEx(0,                           // Optional window styles.
                     class_name,                  // Window class
                     window_title, // Window text
                     WS_OVERLAPPEDWINDOW,         // Window style

                     // Size and position
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                     NULL,      // Parent window
                     NULL,      // Menu
                     hInstance, // Instance handle
                     NULL       // Additional application data
      );

  if (internal_state.hwnd == NULL) {
    perror("failed to create window\n");
    exit(1);
  }

  internal_state.hdc = GetDC(internal_state.hwnd);
    if (internal_state.hdc == NULL) {
        DestroyWindow(internal_state.hwnd);
        perror("Failed to get Window's device context!");
        return -1;
    }

    // Set the pixel format for the device context:
    PIXELFORMATDESCRIPTOR pfd = { };
    pfd.nSize = sizeof(pfd);
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  // Set the size of the PFD to the size of the class
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   // Enable double buffering, opengl support and drawing to a window
    pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
    pfd.cColorBits = 32;        // Give us 32 bits of color information (the higher, the more colors)
    pfd.cDepthBits = 32;        // Give us 32 bits of depth information (the higher, the more depth levels)
    pfd.iLayerType = PFD_MAIN_PLANE;    // Set the layer of the PFD
    int format = ChoosePixelFormat(internal_state.hdc, &pfd);
    if (format == 0 || SetPixelFormat(internal_state.hdc, format, &pfd) == FALSE) {
        ReleaseDC(internal_state.hwnd, internal_state.hdc);
        DestroyWindow(internal_state.hwnd);
        perror("Failed to set a compatible pixel format!");
        return -1;
    }

    // Create and enable a temporary (helper) opengl context:
    HGLRC temp_context = NULL;
    if (NULL == (temp_context = wglCreateContext(internal_state.hdc))) {
        ReleaseDC(internal_state.hwnd, internal_state.hdc);
        DestroyWindow(internal_state.hwnd);
        perror("Failed to create the initial rendering context!");
        return -1;
    }
    wglMakeCurrent(internal_state.hdc, temp_context);

     // Load WGL Extensions:
    gladLoaderLoadWGL(internal_state.hdc);

    // Set the desired OpenGL version:
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,   // Set the MAJOR version of OpenGL to 3
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,   // Set the MINOR version of OpenGL to 2
        WGL_CONTEXT_FLAGS_ARB,
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
        0
    };

    // Create the final opengl context and get rid of the temporary one:
    HGLRC opengl_context = NULL;
    if (NULL == (opengl_context = wglCreateContextAttribsARB(internal_state.hdc, NULL, attributes))) {
        wglDeleteContext(temp_context);
        ReleaseDC(internal_state.hwnd, internal_state.hdc);
        DestroyWindow(internal_state.hwnd);
        perror("Failed to create the final rendering context!");
        return -1;
    }
    wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
    wglDeleteContext(temp_context); // Delete the temporary OpenGL context
    wglMakeCurrent(internal_state.hdc, opengl_context);    // Make our OpenGL 3.2 context current
    
    // Glad Loader!
    if (!gladLoaderLoadGL()) {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(opengl_context);
      ReleaseDC(internal_state.hwnd, internal_state.hdc);
      DestroyWindow(internal_state.hwnd);
      perror("Glad Loader failed!");
      return -1;
    }

  ShowWindow(internal_state.hwnd, nCmdShow);

  internal_state.is_running = 1;
}

void p_process_events(void) {
  while (PeekMessage(&internal_state.msg, internal_state.hwnd, 0, 0, PM_REMOVE)) {
    TranslateMessage(&internal_state.msg);
    DispatchMessage(&internal_state.msg);

    if (internal_state.msg.message == WM_QUIT ||
        (internal_state.msg.message == WM_KEYDOWN && internal_state.msg.wParam == VK_ESCAPE))
      internal_state.is_running = 0;
  }
  printf("message loop end\n");
  SwapBuffers(internal_state.hdc);
}

void p_stop(void) {

}

#endif // __GNUC__
#endif // __MINGW32__
