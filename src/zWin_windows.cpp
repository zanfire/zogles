#include "zWin_windows.h"
#include "zWinFactory.h"
#include "zLogger.h"

#include <Windows.h>


LRESULT CALLBACK  main_loop(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  zWinFactory* factory = zWinFactory::get_instance();
  zWin_windows* window = (zWin_windows*)factory->get_by_id(hWnd);
  if (window != NULL) {
    return window->handle_message(uiMsg, wParam, lParam);
  }
  else {
    return DefWindowProc(hWnd, uiMsg, wParam, lParam); 
  }
}

zWin_windows::zWin_windows(zWinFactory* factory) : zWin(factory) {
  _deviceContext = 0;
}


zWin_windows::~zWin_windows(void) {
  DestroyWindow(_id);
}


void zWin_windows::impl_create(zRect const& pos) {
  WNDCLASS windowClass;
  HINSTANCE hInstance = GetModuleHandle(NULL);

  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc = (WNDPROC)main_loop;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = NULL;
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = L"OGLES";

  RegisterClass(&windowClass);
  RECT rectangle;
  rectangle.left = pos.left;
  rectangle.top = pos.top;
  rectangle.right = pos.right;
  rectangle.bottom = pos.bottom;
  AdjustWindowRectEx(&rectangle, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

  _id = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, L"OGLES", L"main", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, pos.left, pos.top, pos.width(), pos.height(), NULL, NULL, hInstance, NULL);
  if (_id == NULL) {
    _logger->error("Failed to create window, error: %x", GetLastError());
  }
  else {
    ShowWindow(_id, SW_SHOW);
    SetForegroundWindow(_id);
    SetFocus(_id);

    _deviceContext = GetDC(_id);
  }
}


EGLDisplay zWin_windows::impl_get_display(void) {
  if (_deviceContext == 0) {
    return EGL_NO_DISPLAY;
  }
  else {
    return eglGetDisplay(_deviceContext);
  }
}


LRESULT zWin_windows::handle_message(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  //_logger->debug("Incoming message %d", (int)uiMsg);
  switch (uiMsg) { 
    case WM_SIZE: 
      {
        int w = (int)(short)LOWORD(lParam);
        int h = (int)(short)HIWORD(lParam);
        handle_size(w, h);
        return 0; 
      }
    case WM_CLOSE:
      {
        _request_close = true;
        return 0;
      }
    /*
    case WM_CREATE: 
      // Initialize the window. 
      return 0; 
    case WM_PAINT: 
      // Paint the window's client area. 
      return 0; 
    case WM_DESTROY: 
      // Clean up window-specific data objects. 
      return 0; */
    // 
    // Process other messages. 
    // 
    default: 
        return DefWindowProc(_id, uiMsg, wParam, lParam); 
  }
}


bool zWin_windows::peek_message(void) {
  MSG message;
  if (PeekMessage(&message, _id, 0, 0, PM_REMOVE) == 0) {
    return false;
  }
  // Dispatch event.
  handle_message(message.message, message.wParam, message.lParam);
  return false;
}