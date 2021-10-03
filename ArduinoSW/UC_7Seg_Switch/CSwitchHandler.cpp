/**
 * @file   CSwitchHandler.cc
 * @brief  Library for handling board for EMI/EMC university project.
 * @author Created by Jovan Slavujevic
 * @date   October 3, 2020.
 *
 * Released into the public domain.
 */

#include "CSwitchHandler.h"

/**
 * @brief ESSENTIAL MACROS
 */
#define ARD_PIN_SW_1  0x07u
#define ARD_PIN_SW_2  0x08u
#define ARD_PIN_SW_3  0x09u

#define ARD_PIN_LED_1 0x0Bu
#define ARD_PIN_LED_2 0x0Cu
#define ARD_PIN_LED_3 0x0Du

/**
 * @brief representation of LED indications in array of booleans
 */
static const bool led_up[]     = {0,0,1};
static const bool led_down[]   = {0,1,0};
static const bool led_random[] = {1,0,0};
static const bool led_none[]   = {0,0,0};

void CSwitchHandler::initSw()
{
  // set switch pins as output pins
  for(uint8_t i=ARD_PIN_SW_1; i<=ARD_PIN_SW_3; ++i)
  {
    pinMode(i, INPUT);
  }
}

void CSwitchHandler::initLed()
{
  // set LED pins as output pins
  for(uint8_t i=ARD_PIN_LED_1; i<=ARD_PIN_LED_3; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

void CSwitchHandler::readSw()
{
  // read from switches
  for (uint8_t i=0, j=ARD_PIN_SW_1; i<NUM_OF_SW; ++i, j++)
  {
    mSwCurrentState[i] = digitalRead(j);
  }
}

CSwitchHandler::CSwitchHandler() :
  mSwLastState{-1},
  mSwEnabled{false}
{
  // do initialization
  CSwitchHandler::initSw();
  CSwitchHandler::initLed();
}

void CSwitchHandler::updateLed(eActions action) const
{
  // pointer to static array of boolean LED representation
  const bool* ignition_ptr = NULL;

  // check action and assign proper array to pointer
  switch(action)
  {
    case eActions::UP:
    {
      ignition_ptr = led_up;
      break;
    }
    case eActions::DOWN:
    {
      ignition_ptr = led_down;
      break;
    }
    case eActions::RAND:
    {
      ignition_ptr = led_random;
      break;
    }
    case eActions::NONE:
    {
      ignition_ptr = led_none;
      break;
    }
    default:
    {
      // just exit
      return;
    }
  }

  // update LED according to pointer
  for (uint8_t i=0, j=ARD_PIN_LED_1; j<=ARD_PIN_LED_3; i++, ++j)
  {
    digitalWrite(j, (ignition_ptr[i]) ? HIGH : LOW);
  }
}

void CSwitchHandler::checkSw()
{
  // read from switch
  CSwitchHandler::readSw();

  // check is current state same as last for every switch
  for (uint8_t i=0; i<NUM_OF_SW; ++i)
  {
    if (mSwCurrentState[i] != mSwLastState[i])
    {
      // set all enabled flags to false, then set changed one to true
      memset(mSwEnabled, false, NUM_OF_SW);
      mSwEnabled[i] = true;

      // assign current states to last states of switches
      memcpy(mSwLastState, mSwCurrentState, sizeof(mSwCurrentState));

      // break when you find first change
      break;
    }
  }
}

bool CSwitchHandler::isAnySwEnabled() const
{
  // iterate through enabled flags and exit when you find first change
  for (uint8_t i=0; i<NUM_OF_SW; ++i)
  {
    if (mSwEnabled[i])
    {
      return true;
    }
  }
  return false;
}
