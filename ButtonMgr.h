/*
    A Simple module to debounce digital input pins.
    Assumes port is pulled up, so pin low means button is pressed
*/
#pragma once

enum ButtonState
{
    BUTTON_UP,      //Button not pressed
    BUTTON_PRESSED, //Initial press
    BUTTON_DEBOUCE, //debounce period
    BUTTON_DOWN,    //After debounce
    BUTTON_HELD,     //After hold time,
    BUTTON_RELEASED
};

struct ButtonCtx
{
    uint8_t         pin;
    ButtonState state;
    unsigned long time;
};

class ButtonMgr
{
public:

    static const int DEBOUNCE_TIME = 50; //ms
    static const int HOLD_TIME = 2000; //ms

    static bool isHeld(int buttonIdx)
    {
      return buttonCtx_[buttonIdx].state == BUTTON_HELD;
    }

    static bool isDown(int buttonIdx)
    {
      return buttonCtx_[buttonIdx].state == BUTTON_DOWN;
    }

    static bool isPressed(int buttonIdx)
    {
      return buttonCtx_[buttonIdx].state == BUTTON_PRESSED;
    }

    static bool isReleased(int buttonIdx)
    {
      return buttonCtx_[buttonIdx].state == BUTTON_RELEASED;
    }

public:
    static void init(ButtonCtx* buttonCtx, int buttonCnt);
    static void update();

private:
    static ButtonCtx* buttonCtx_;
    static uint8_t    buttonCnt_;

};
