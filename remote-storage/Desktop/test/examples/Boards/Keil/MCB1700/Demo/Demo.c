/*-----------------------------------------------------------------------------
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All
 * rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1.Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   3.Neither the name of Arm nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * Name:    Demo.c
 * Purpose: Demo example for MCB1700
 * Note(s):
 *----------------------------------------------------------------------------*/

#include "cmsis_os2.h"
#include <stdio.h>
#include "Board_LED.h"
#include "Board_Buttons.h"
#include "Board_Joystick.h"
#include "Board_ADC.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/* Main stack size must be multiple of 8 Bytes */
#define APP_MAIN_STK_SZ (1024U)
static uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

/* Joystick position definitions */
#define JOY_X        (9U*16U)
#define JOY_Y        (5U*24U + 6U)

/* Buttons bit masks */
#define BTN_INT0     (1U << 0)

/* Periodic timer definition */
static void Timer_Callback (void *arg);
//osTimerDef (PeriodicTimer, Timer_Callback);
static osTimerId_t TimerId;

/* GLCD string buffer */
#define STRINGBUF_LEN 21U

/* Extern variables */
extern uint8_t  Arrows_16bpp_red[];
extern uint8_t  Button_16bpp[];
extern uint8_t  Bulb_16bpp[];

/* Static variables */
static uint32_t LEDOn, LEDOff;

__NO_RETURN void app_main (void *arg);

/*-----------------------------------------------------------------------------
  Periodic timer callback
 *----------------------------------------------------------------------------*/
static void Timer_Callback (void *arg) {
  static uint8_t TickLed;

  (void) arg;
  switch (TickLed++) {
    case  1: LEDOn   = 1; break;
    case  6: LEDOff  = 1; break;
    case 10: TickLed = 0; break;
  }
}



/*-----------------------------------------------------------------------------
  Blink LEDs
 *----------------------------------------------------------------------------*/
static void BlinkLed (void) {
  static uint32_t LED = 0U;
  static uint32_t LEDDir = 1U;

  if (LEDOn) {
    LEDOn = 0U;
    LED_On (LED);                         /* Turn specified LED on            */
  }

  if (LEDOff) {
    LEDOff = 0U;
    LED_Off (LED);                        /* Turn specified LED off           */

    if (LEDDir == 1U) {
      LED++;                              /* Change LED number                */
      if (LED == LED_GetCount () - 1U) {
        LEDDir = 0U;                      /* Change direction to down         */
      }
    }
    else {
      LED--;                              /* Change LED number                */
      if (LED == 0U) {
        LEDDir = 1U;                      /* Change direction to up           */
      }
    }
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main'
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  uint32_t    ofs;
  uint32_t    btnMsk, joyMsk;
  int32_t     adcVal;
  int32_t     adc  = -1;
  uint32_t    btn  = 0xFFFFFFFFU;
  uint32_t    joy  = 0xFFFFFFFFU;

  (void) arg;

  TimerId = osTimerNew (Timer_Callback, osTimerPeriodic, (void *)0, NULL);

  if (TimerId) {
    osTimerStart (TimerId, 10U);
  }

  /* Prepare display for ADC, Buttons, Joystick */
  GLCD_ClearScreen        ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (0U, 0U*24U, "       MCB1700      ");
  GLCD_DrawString         (0U, 1U*24U, "    Demo Example    ");
  GLCD_SetBackgroundColor (GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor (GLCD_COLOR_BLUE);
  GLCD_DrawString         (0U, 2U*24U, "                    ");
  GLCD_DrawString         (0U, 3U*24U, "AD value:           ");
  GLCD_DrawString         (0U, 4U*24U, "Buttons :           ");
  GLCD_DrawString         (0U, 5U*24U, "                    ");
  GLCD_DrawString         (0U, 6U*24U, "Joystick:           ");
  GLCD_DrawString         (0U, 7U*24U, "                    ");
  GLCD_DrawString         (0U, 8U*24U, "                    ");
  GLCD_DrawString         (0U, 9U*24U, "                    ");

  while (1) {
    BlinkLed();

    btnMsk = Buttons_GetState();    /* Show buttons state                 */
    if (btn ^ btnMsk) {
      btn = btnMsk;
      GLCD_SetForegroundColor   (GLCD_COLOR_BLACK);
      GLCD_SetFont              (&GLCD_Font_16x24);
      if (btnMsk & BTN_INT0)    { GLCD_DrawString ( 9U*16U, 4U*24U, "INT0"); }

      GLCD_SetForegroundColor (GLCD_COLOR_LIGHT_GREY);
      if (!(btnMsk & BTN_INT0)) { GLCD_DrawString ( 9U*16U, 4U*24U, "INT0"); }
    }
    
    joyMsk = Joystick_GetState();   /* Show joystick arrows               */
    if (joy ^ joyMsk) {
      joy = joyMsk;
    
      /* Left arrow white/red? */
      ofs = (joy & JOYSTICK_LEFT)   ? (2416U) : (0U);
      GLCD_DrawBitmap(JOY_X+ 0U, JOY_Y+20U, 19U, 14U, &Arrows_16bpp_red[ofs + 0U*532U]);
    
      /* Right arrow white/red? */
      ofs = (joy & JOYSTICK_RIGHT)  ? (2416U) : (0U);
      GLCD_DrawBitmap(JOY_X+35U, JOY_Y+20U, 19U, 14U, &Arrows_16bpp_red[ofs + 1U*532U]);
    
      /* Center dot white/red */
      ofs = (joy & JOYSTICK_CENTER) ? (2416U) : (0U);
      GLCD_DrawBitmap(JOY_X+21U, JOY_Y+21U, 12U, 12U, &Arrows_16bpp_red[ofs + 4U*532U]);
    
      /* Up arrow white/red? */
      ofs = (joy & JOYSTICK_UP)     ? (2416U) : (0U);
      GLCD_DrawBitmap(JOY_X+20U, JOY_Y+ 0U, 14U, 19U, &Arrows_16bpp_red[ofs + 2U*532U]);
    
      /* Down arrow white/red? */
      ofs = (joy & JOYSTICK_DOWN)   ? (2416U) : (0U);
      GLCD_DrawBitmap(JOY_X+20U, JOY_Y+35U, 14U, 19U, &Arrows_16bpp_red[ofs + 3U*532U]);
    }
    
    ADC_StartConversion();          /* Show A/D conversion bargraph       */
    adcVal = ADC_GetValue();
    if ((adcVal != -1) && (adc ^ adcVal)) {
      adc = adcVal;
      GLCD_SetForegroundColor (GLCD_COLOR_GREEN);
      GLCD_DrawBargraph       (144U, 3U*24U, 160U, 20U, ((uint32_t)adcVal * 100U) / ((1U << ADC_GetResolution()) - 1U));
    }
  }
}
