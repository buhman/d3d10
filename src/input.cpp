#include <windows.h>
#include <assert.h>
#include <XInput.h>

#include "print.hpp"
#include "input.hpp"

Joystate g_Joystate;

typedef DWORD (WINAPI * lpXInputGetCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES *);
typedef DWORD (WINAPI * lpXInputGetState)(DWORD, XINPUT_STATE *);

//lpXInputGetCapabilities g_XInputGetCapabilities;
lpXInputGetState g_XInputGetState;

HRESULT InitInput(HINSTANCE hInstance)
{
  const char * libs[] = {
    "xinput1_4.dll",
    "xinput1_3.dll",
    "xinput9_1_0.dll",
    "xinput1_2.dll",
    "xinput1_1.dll",
  };
  const int libs_length = (sizeof (libs)) / (sizeof (libs[0]));

  HMODULE hLib;
  for (int i = 0; i < libs_length; i++) {
    hLib = LoadLibraryA(libs[i]);
    if (hLib != NULL) {
      //print("using xinput: %s\n", libs[i]);
      break;
    }
  }
  if (hLib == NULL) {
    print("no xinput module found\n");
    return E_FAIL;
  }

  //g_XInputGetCapabilities = (lpXInputGetCapabilities)GetProcAddress(hLib, "XInputGetCapabilities");
  //assert(g_XInputGetCapabilities != NULL);
  g_XInputGetState = (lpXInputGetState)GetProcAddress(hLib, "XInputGetState");
  assert(g_XInputGetState != NULL);

  return S_OK;
}

void UpdateInput()
{
  XINPUT_STATE state;
  for (DWORD i = 0; i < 4; i++) {
    DWORD ret = g_XInputGetState(i, &state);
    if (ret != ERROR_SUCCESS) {
      continue;
    }
    break;
  }

  const float trigger = 1.0f / 255.0f;
  const float thumb = 1.0f / 32768.0f;
  g_Joystate.buttons = state.Gamepad.wButtons;
  g_Joystate.triggerL = ((float)state.Gamepad.bLeftTrigger) * trigger;
  g_Joystate.triggerR = ((float)state.Gamepad.bRightTrigger) * trigger;
  g_Joystate.thumbLX = ((float)state.Gamepad.sThumbLX) * thumb;
  g_Joystate.thumbLY = ((float)state.Gamepad.sThumbLY) * thumb;
  g_Joystate.thumbRX = ((float)state.Gamepad.sThumbRX) * thumb;
  g_Joystate.thumbRY = ((float)state.Gamepad.sThumbRY) * thumb;
}
