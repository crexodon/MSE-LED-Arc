#pragma once

#include "globals.h"
#include "idleAnimation.h"
#include "artnet.h"

typedef enum effect {
    fx_undefined        = -1,
    fx_static           = 0,
    fx_artnet           = 1,
    fx_colorpalette     = 2,
    fx_animation        = 100,  // internal only
} fx_t;

// Transistion time is being used for animation speed. Defines the speed in seconds for one animation loop (typically 256 steps)
typedef struct {
    bool state;
    uint8_t brightness;
    uint32_t color;
    fx_t effect;
    uint32_t animationTime;
} ledState_t;

extern const char* effectNames[];
extern int numEffects;
extern CRGB leds[];

void setLedState(ledState_t state);
void setLedState(bool state);
void setLedBrightness(uint8_t brightness);
void setLedColor(uint32_t color);               // 0xWWRRGGBB
void setLedWhiteValue(uint8_t whiteValue);
void setLedAnimationTime(uint32_t transition);  // ms
void setLedEffect(String effect);
const char* getCurLedEffectName();
ledState_t getLedState();
void setLedProgress(uint8_t percent);
void initLeds();
void loopLeds();