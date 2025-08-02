#ifdef __GNUC__
#ifdef __MINGW32__

struct p_state {
  _Bool is_running;
  unsigned int width, height;
};

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
}

void p_start(void) {
  // Register the window class.
  const wchar_t CLASS_NAME[] = L"Sample Window Class";

  WNDCLASS wc = {};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);

  // Create the window.

  HWND hwnd =
      CreateWindowEx(0,                           // Optional window styles.
                     CLASS_NAME,                  // Window class
                     L"Learn to Program Windows", // Window text
                     WS_OVERLAPPEDWINDOW,         // Window style

                     // Size and position
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                     NULL,      // Parent window
                     NULL,      // Menu
                     hInstance, // Instance handle
                     NULL       // Additional application data
      );

  if (hwnd == NULL) {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);
}

void p_process_events(void) {
  while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (msg.message == WM_QUIT ||
        (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE))
      internal_state.is_running = 0;
  }
}

void p_stop(void) {

}

#endif // __GNUC__
#endif // __MINGW32__
