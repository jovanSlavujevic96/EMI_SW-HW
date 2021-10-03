/**
 * @file   C7SegHandler.h
 * @brief  Library for handling board for EMI/EMC university project.
 * @author Created by Jovan Slavujevic
 * @date   October 3, 2020.
 *
 * Released into the public domain.
 */

#ifndef C7SegHandler_h_
#define C7SegHandler_h_

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif //(ARDUINO >= 100)

#include "eactions.h"
class CSwitchHandler;

/**
 * @brief C7SegHanlder handles 7-Seg init, updates counter according to switch states
 *        and writes LOW/HIGH signals to 7-seg outputs
 */
class C7SegHandler
{
private: //fields
  /**
   * @brief mCounter - it is used as index of boolean array representation of 7-SEG display
   */
  uint8_t mCounter;
  /**
   * @brief mIncrementStep - increments/decrements counter in UP/DOWN states (actions)
   */
  int8_t mIncrementStep;
  /**
   * @brief mAction - enumeration that describes running state (action)
   */
  eActions mAction;
private: //methods
  /**
   * @brief Initialises 7-SEG pins as output pins
   */
  void initDisplay();
  /**
   * @brief updates display according to array representation of number
   */
  void updateDisplay(const bool* seg_arr);
public:
  /**
   * @brief Default constructor - constructor wo/ params
   */
  C7SegHandler();
  /**
   * @brief Default destructor
   */
  ~C7SegHandler() = default;

  /**
   * @brief updates counter and displays number on 7-SEG display
   *
   * @param sw - passed switch handler that provides switch states and does LED lighting
   */
  void updateCounter(const CSwitchHandler* sw);
};

#endif //C7SegHandler_h_
