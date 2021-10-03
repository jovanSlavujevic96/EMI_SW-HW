/* @brief ESSENTIAL MACROS */
#define ARD_PIN_SEG_A 0x00u
#define ARD_PIN_SEG_B 0x01u
#define ARD_PIN_SEG_C 0x02u
#define ARD_PIN_SEG_D 0x03u
#define ARD_PIN_SEG_E 0x04u
#define ARD_PIN_SEG_F 0x05u
#define ARD_PIN_SEG_G 0x06u

#define ARD_PIN_SW_1  0x07u
#define ARD_PIN_SW_2  0x08u
#define ARD_PIN_SW_3  0x09u

#define ARD_PIN_LED_1 0x0Bu
#define ARD_PIN_LED_2 0x0Cu
#define ARD_PIN_LED_3 0x0Du

#define SEGMENTS      0x07u
#define NUM_OF_SW     0x03u

/* @brief representation of 7-seg enumerations in array of booleans */
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

/* @brief array of pointers that are pointing into array of booleans in order to iterate through them */
static const bool (* const segPtr[])[SEGMENTS] = {&seg0,&seg1,&seg2,&seg3,&seg4,&seg5,&seg6,&seg7,&seg8,&seg9};
static const size_t segPtrSize = (sizeof(segPtr) / sizeof(bool*));

static const bool led_up[]     = {0,0,1};
static const bool led_down[]   = {0,1,0};
static const bool led_random[] = {1,0,0};
static const bool led_none[]   = {0,0,0};

enum e_actions 
{
  UP = 0,
  DOWN,
  RAND,
  NONE
};

/* @brief class handles SW1 & SW3 init, reading and informs system of state changing */
class CSwitchHanlder
{
private:  
  int SW_last_state[NUM_OF_SW];
  int SW_current_state[NUM_OF_SW];
  bool SW_enabled[NUM_OF_SW];

  void initSw();
  void initLed();
  void readSw();
public:
  CSwitchHanlder();
  ~CSwitchHanlder() = default;

  void updateLed(e_actions action) const;
  void checkSw();
  bool isSwStateUpdated() const;
  inline bool isSwEnabled(unsigned int index) const { return SW_enabled[index]; }
};

void CSwitchHanlder::initSw()
{
  for(uint8_t i=ARD_PIN_SW_1; i<=ARD_PIN_SW_3; ++i)
  {
    pinMode(i, INPUT);
  }
}

void CSwitchHanlder::initLed()
{
  for(uint8_t i=ARD_PIN_LED_1; i<=ARD_PIN_LED_3; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

void CSwitchHanlder::readSw()
{
  //read from switches
  for (uint8_t i=0, j=ARD_PIN_SW_1; i<NUM_OF_SW; ++i, j++)
  {
    SW_current_state[i] = digitalRead(j);
  }
}

CSwitchHanlder::CSwitchHanlder() :
  SW_last_state{-1},
  SW_enabled{false}
{
  CSwitchHanlder::initSw();
  CSwitchHanlder::initLed();
}

void CSwitchHanlder::updateLed(e_actions action) const
{
  const bool* ignition_ptr = NULL;
  switch(action)
  {
    case e_actions::UP:
    {
      ignition_ptr = led_up;
      break;
    }
    case e_actions::DOWN:
    {
      ignition_ptr = led_down;
      break;
    }
    case e_actions::RAND:
    {
      ignition_ptr = led_random;
      break;
    }
    case e_actions::NONE:
    {
      ignition_ptr = led_none;
      break;
    }
    default:
    {
      return;
    }
  }
  for (uint8_t i=0, j=ARD_PIN_LED_1; j<=ARD_PIN_LED_3; i++, ++j)
  {
    digitalWrite(j, (ignition_ptr[i]) ? HIGH : LOW);
  }
}

void CSwitchHanlder::checkSw()
{
  CSwitchHanlder::readSw();

  // check is current state same as last
  for (uint8_t i=0; i<NUM_OF_SW; ++i)
  {
    if (SW_current_state[i] != SW_last_state[i])
    {
      memset(SW_enabled, false, sizeof(SW_enabled));
      SW_enabled[i] = true;
      memcpy(SW_last_state, SW_current_state, sizeof(SW_current_state));
      break;
    }
  }
}

bool CSwitchHanlder::isSwStateUpdated() const
{
  for (uint8_t i=0; i<NUM_OF_SW; ++i)
  {
    if (SW_enabled[i])
    {
      return true; 
    }
  }
  return false;
}

/* @brief class handles 7-Seg init, updates counter according to switch states and writes LOW/HIGH signals to 7-seg outputs */
class C7SegHanlder
{
private:
  uint8_t counter;
  int8_t incrementStep;
  e_actions action;

  void initDisplay();
  void updateDisplay(const bool* segArr);
public:
  C7SegHanlder();
  ~C7SegHanlder() = default;

  void updateCounter(const CSwitchHanlder& sw);
};

void C7SegHanlder::initDisplay()
{
  // initialize digital pin LED_BUILTIN as an output.
  for(uint8_t i=ARD_PIN_SEG_A; i<=ARD_PIN_SEG_G; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

C7SegHanlder::C7SegHanlder() :
  counter{0},
  incrementStep{1},
  action{e_actions::NONE}
{
  C7SegHanlder::initDisplay();
}

void C7SegHanlder::updateDisplay(const bool* segArr)
{
  for(uint8_t i=ARD_PIN_SEG_A; i<=ARD_PIN_SEG_G; ++i)
  {
    digitalWrite(i, (segArr[i]) ? HIGH : LOW);
  }
}

void C7SegHanlder::updateCounter(const CSwitchHanlder& sw) 
{ 
  if (sw.isSwStateUpdated())
  {
    for (uint8_t i=0; i<NUM_OF_SW; ++i)
    {
      if (sw.isSwEnabled(i))
      {
        action = static_cast<e_actions>(i);
      }
    }
  }
  if (e_actions::NONE == action)
  {
    goto update_led;
  }
  else if (e_actions::RAND == action)
  {
    counter = random(0, segPtrSize);
  }
  else
  {
    incrementStep = (e_actions::UP == action) ? (+1) : (-1);
    if (!counter && -1 == incrementStep)
    {
      // return to UPPER value
      counter = segPtrSize;
    }
    counter = (counter+incrementStep) % segPtrSize;
  }
  C7SegHanlder::updateDisplay(*segPtr[counter]);
update_led:
  sw.updateLed(action);
}

static C7SegHanlder _7seg;
static CSwitchHanlder _sw;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // nothing to do -> all init is done on _7seg & _sw constructors
}

// the loop function runs over and over again forever
void loop() 
{
  _sw.checkSw();
  _7seg.updateCounter(_sw);
  delay(1000); // wait for a second
}
