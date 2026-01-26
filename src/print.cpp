#include <windows.h>
#include <strsafe.h>

#include "print.hpp"

void print(LPCSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512];
  STRSAFE_LPSTR end = NULL;

  StringCbVPrintfExA(buf,
                     (sizeof (buf)),
                     &end,
                     NULL,
                     STRSAFE_NULL_ON_FAILURE,
                     fmt,
                     args);
  va_end(args);
  #ifdef _DEBUG
  OutputDebugStringA(buf);
  //size_t length = end - &buf[0];
  //HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  //WriteConsoleA(hOutput, buf, (DWORD)length, NULL, NULL);
  #endif
}

void printW(LPCWSTR fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  WCHAR buf[512];
  STRSAFE_LPWSTR end = NULL;

  StringCbVPrintfExW(buf,
                     (sizeof (buf)),
                     &end,
                     NULL,
                     STRSAFE_NULL_ON_FAILURE,
                     fmt,
                     args);
  va_end(args);
  #ifdef _DEBUG
  OutputDebugStringW(buf);
  //size_t length = end - &buf[0];
  //HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  //WriteConsoleW(hOutput, buf, (DWORD)length, NULL, NULL);
  #endif
}
