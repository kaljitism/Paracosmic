#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

// Todo: Global for now
global_variable bool Running;

internal void Win32ResizableDBISection(int width, int height) {}

internal void Win32UpdateWindow(HWND Window, int X, int Y, int Width, int Height) {}

LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                                         UINT Message,
                                         WPARAM WParam,
                                         LPARAM LParam) {
  
  LRESULT Result = 0;
  
  switch (Message) {
    case WM_SIZE: {
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top;
      Win32ResizableDBISection(Width, Height);
      OutputDebugStringA("WM_SIZE\n");
    }
      break;
    case WM_CLOSE: {
      // Todo - handle this with a message to user
      Running = false;
    }
      break;
    case WM_ACTIVATEAPP: {
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    }
      break;
    case WM_DESTROY: {
      // todo: handle this as an error. Recreate window
      Running = false;
    }
      break;
    
    case WM_PAINT: {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.top;
      int Width = Paint.rcPaint.right - Paint.rcPaint.left;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      Win32UpdateWindow(Window, X, Y, Width, Height);
      EndPaint(Window, &Paint);
    }
      break;
    
    default: {
//      OutputDebugStringA("default\n");
      Result = DefWindowProcA(Window, Message, WParam, LParam);
    }
      break;
  }
  return (Result);
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance,
                     LPSTR CommandLine, int ShowCode) {
  
  WNDCLASS WindowClass = {};
  
  // Todo: Check Validity of owndc/ hredraw/ vredraw
  WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  WindowClass.lpfnWndProc = Win32MainWindowCallback;
  WindowClass.hInstance = Instance;
  //    WindowClass.hIcon;
  WindowClass.lpszClassName = "ParacosmicWindowsClass";
  
  if (RegisterClass(&WindowClass)) {
    HWND WindowHandle = CreateWindowEx(
        0,
        WindowClass.lpszClassName,
        "Paracosmic",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        Instance,
        nullptr);
    if (WindowHandle) {
      Running = true;
      
      while (Running) {
        MSG Message;
        BOOL MessageResult = GetMessageA(&Message, nullptr, 0, 0);
        if (MessageResult > 0) {
          TranslateMessage(&Message);
          DispatchMessage(&Message);
        } else {
          break;
        }
      }
#pragma clang diagnostic pop
      
    } else {
    
    }
  } else {
    // TODO: logging
  }
  
  return (0);
}
