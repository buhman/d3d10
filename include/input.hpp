#pragma once
#ifndef _INPUT_HPP_
#define _INPUT_HPP_

HRESULT InitInput(HINSTANCE hInstance);
void UpdateInput();

struct Joystate {
  int buttons;
  float triggerL;
  float triggerR;
  float thumbLX;
  float thumbLY;
  float thumbRX;
  float thumbRY;
};

extern Joystate g_Joystate;

#endif
