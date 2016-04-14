#include <Arduino.h>
#include "ButtonMgr.h"

ButtonCtx* ButtonMgr::buttonCtx_;
uint8_t ButtonMgr::buttonCnt_(0);

void ButtonMgr::init(ButtonCtx* buttonCtx, int buttonCnt)
{
    buttonCtx_ = buttonCtx;
    buttonCnt_ = buttonCnt;

    for(uint8_t idx = 0; idx < buttonCnt_; idx++)
    {
        ButtonCtx* buttonCtx = &buttonCtx_[idx];
        buttonCtx->state = BUTTON_UP;
    }
}

void ButtonMgr::update()
{
    for(uint8_t idx = 0; idx < buttonCnt_; idx++)
    {
        ButtonCtx* buttonCtx = &buttonCtx_[idx];
        const bool pressed = digitalRead(buttonCtx->pin) ? false : true;
        switch(buttonCtx->state)
        {
            case BUTTON_UP:
            {
                if(pressed)
                {
                    buttonCtx->time = millis();
                    buttonCtx->state = BUTTON_PRESSED;
                }
                break;
            }

            case BUTTON_PRESSED:
            {
                buttonCtx->state = BUTTON_DEBOUCE;
                break;
            }

            case BUTTON_DEBOUCE:
            {
                if( (millis() - buttonCtx->time) > DEBOUNCE_TIME )
                {
                    if(pressed)
                    {
                        buttonCtx->time = millis();
                        buttonCtx->state = BUTTON_DOWN;
                    }
                    else
                    {
                        buttonCtx->state = BUTTON_UP;
                    }
                }
                break;
            }

            case BUTTON_DOWN:
            {
                if(pressed)
                {
                    if( (millis() - buttonCtx->time) > HOLD_TIME )
                    {
                        buttonCtx->state = BUTTON_HELD;
                    }
                }
                else
                {
                    buttonCtx->state = BUTTON_RELEASED;
                }
                break;
            }

            case BUTTON_HELD:
            {
                if(!pressed)
                {
                    buttonCtx->state = BUTTON_UP;
                }
                break;
            }

            case BUTTON_RELEASED:
            {
                buttonCtx->state = BUTTON_UP;
                break;
            }
        }
    }

}
