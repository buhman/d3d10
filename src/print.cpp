#include <windows.h>
#include <strsafe.h>

#include "print.hpp"

void print(LPCSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512];
  STRSAFE_LPSTR end;

  StringCbVPrintfExA(buf,
                     (sizeof (buf)),
                     &end,
                     NULL,
                     STRSAFE_NULL_ON_FAILURE,
                     fmt,
                     args);
  va_end(args);
  size_t length = end - &buf[0];
  #ifdef _DEBUG
  OutputDebugStringA(buf);
  #endif
  //HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  //WriteConsoleA(hOutput, buf, (DWORD)length, NULL, NULL);
}

void printW(LPCWSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  WCHAR buf[512];
  StringCbVPrintfExW(buf,
                     (sizeof (buf)),
                     NULL,
                     NULL,
                     STRSAFE_NULL_ON_FAILURE,
                     fmt,
                     args);
  va_end(args);
  #ifdef _DEBUG
  OutputDebugStringW(buf);
  #endif
}
