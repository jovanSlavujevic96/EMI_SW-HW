/**
 * @file   C7SegHandler.cpp
 * @brief  Library for handling board for EMI/EMC university project.
 * @author Created by Jovan Slavujevic
 * @date   October 3, 2020.
 *
 * Released into the public domain.
 */

#include "C7SegHandler.h"
#include "CSwitchHandler.h"

/**
 * @brief ESSENTIAL MACROS
 */
#define ARD_PIN_SEG_A 0x00u
#define ARD_PIN_SEG_B 0x01u
#define ARD_PIN_SEG_C 0x02u
#define ARD_PIN_SEG_D 0x03u
#define ARD_PIN_SEG_E 0x04u
#define ARD_PIN_SEG_F 0x05u
#define ARD_PIN_SEG_G 0x06u

#define SEGMENTS      0x07u

/**
 * @brief representation of 7-seg enumerations in array of booleans
 */
///////////////////////////{a,b,c,d,e,f,g}
static const bool seg0[] = {1,1,1,1,1,1,0};
static const bool seg1[] = {0,1,1,0,0,0,0};
static const bool seg2[] = {1,1,0,1,1,0,1};
static const bool seg3[] = {1,1,1,1,0,0,1};
static const bool seg4[] = {0,1,1,0,0,1,1};
static const bool seg5[] = {1,0,1,1,0,1,1};
static const bool seg6[] = {1,0,1,1,1,1,1};
static const bool seg7[] = {1,1,1,0,0,0,0};
static const bool seg8[] = {1,1,1,1,1,1,1};
static const bool seg9[] = {1,1,1,1,0,1,1};
///////////////////////////{a,b,c,d,e,f,g}

/**
 * @brief array of pointers that are pointing into array of booleans in order to iterate through them
 */
static const bool (* const seg_ptr[])[SEGMENTS] = {&seg0,&seg1,&seg2,&seg3,&seg4,&seg5,&seg6,&seg7,&seg8,&seg9};
static const size_t seg_ptr_size = (sizeof(seg_ptr) / sizeof(bool*));

void C7SegHandler::initDisplay()
{
  // initializes 7-SEG pins as output pins
  for(uint8_t i=ARD_PIN_SEG_A; i<=ARD_PIN_SEG_G; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

C7SegHandler::C7SegHandler() :
  mCounter{0},
  mIncrementStep{1},
  mAction{eActions::NONE}
{
  // do initialization
  C7SegHandler::initDisplay();
}

void C7SegHandler::updateDisplay(const bool* seg_arr)
{
  // update 7-SEG display according to passed array
  for(uint8_t i=ARD_PIN_SEG_A; i<=ARD_PIN_SEG_G; ++i)
  {
    digitalWrite(i, (seg_arr[i]) ? HIGH : LOW);
  }
}

void C7SegHandler::updateCounter(const CSwitchHandler* sw)
{
  // checks with switch is enabled and assigns action
  if (sw->isAnySwEnabled())
  {
    for (uint8_t i=0; i<NUM_OF_SW; ++i)
    {
      if (sw->isSwEnabled(i))
      {
        // casts iterating number to enum action
        mAction = static_cast<eActions>(i);
        break;
      }
    }
  }
  if (eActions::NONE == mAction)
  {
    // just update LED
    goto update_led;
  }
  else if (eActions::RAND == mAction)
  {
    // assign random value to counter
    mCounter = random(0, seg_ptr_size);
  }
  else
  {
    // checks increments according to action
    mIncrementStep = (eActions::UP == mAction) ? (+1) : (-1);

    // if DOWN action
    if (!mCounter && -1 == mIncrementStep)
    {
      // return to UPPER value
      mCounter = seg_ptr_size;
    }

    // update (increment/decrement) counter
    mCounter = (mCounter + mIncrementStep) % seg_ptr_size;
  }

  // update 7-SEG display
  C7SegHandler::updateDisplay(*seg_ptr[mCounter]);

update_led:
  // update LED
  sw->updateLed(mAction);
}
