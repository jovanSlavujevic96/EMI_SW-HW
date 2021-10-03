/**
 * @file   CSwitchHandler.h
 * @brief  Library for handling board for EMI/EMC university project.
 * @author Created by Jovan Slavujevic
 * @date   October 3, 2020.
 *
 * Released into the public domain.
 */

#ifndef CSwitchHandler_h_
#define CSwitchHandler_h_

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif //(ARDUINO >= 100)

#include "eactions.h"

/**
 * @brief ESSENTIAL MACROS
 */
#define NUM_OF_SW     0x03u

/**
 * @brief class handles SW1 & SW3 init, reading and informs system of state changing
 */
class CSwitchHandler
{
private: //fields/members
  /**
   * @brief mSwLastState - array of previous registered states from switches
   */
  int mSwLastState[NUM_OF_SW];
  /**
   * @brief mSwCurrentState - array of current registered states from switches
   */
  int mSwCurrentState[NUM_OF_SW];
  /**
   * @brief mSwEnabled - array of states changes from switches
   */
  bool mSwEnabled[NUM_OF_SW];
private: //methods
  /**
   * @brief Initialises SWITCH pins as input pins
   */
  void initSw();
  /**
   * @brief Initialises LED pins as output pins
   */
  void initLed();
  /**
   * @brief Reads values from SWITCH pins
   */
  void readSw();
public: //methods
  /**
   * @brief Default constructor - constructor wo/ params
   */
  CSwitchHandler();
  /**
   * @brief Default destructor
   */
  ~CSwitchHandler() = default;

  /**
   * @brief updates LED representation
   *
   * @param action - enum type of running state
   */
  void updateLed(eActions action) const;
  /**
   * @brief registers new state on switch and checks are there any changes.
   *        Updates SW_enabled array.
   */
  void checkSw();
  /**
   * @brief checks is any of switches is pressed (state change)
   */
  bool isAnySwEnabled() const;
  /**
   * @brief checks is particular switch pressed (state change)
   *
   * @param index - index of array
   */
  inline bool isSwEnabled(unsigned int index) const { return mSwEnabled[index]; }
};

#endif //CSwitchHandler_h_
