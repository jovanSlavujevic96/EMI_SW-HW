/* @brief ESSENTIAL MACROS */
#define ARD_PIN_SEG_A 0x00u
#define ARD_PIN_SEG_G 0x06u

#define ARD_PIN_SW_1  0x07u
#define ARD_PIN_SW_3  0x08u

#define SEGMENTS      0x07u
#define NUM_OF_SW     0x02u

/* @brief representation of 7-seg enumerations in array of booleans */
///////////////////////////{a,b,c,d,e,f,g}
static const bool seg0[] = {0,0,0,0,0,0,1};
static const bool seg1[] = {1,0,0,1,1,1,1};
static const bool seg2[] = {0,0,1,0,0,1,0};
static const bool seg3[] = {0,0,0,0,1,1,0};
static const bool seg4[] = {1,0,0,1,1,0,0};
static const bool seg5[] = {0,1,0,0,1,0,0};
static const bool seg6[] = {0,1,0,0,0,0,0};
static const bool seg7[] = {0,0,0,1,1,1,1};
static const bool seg8[] = {0,0,0,0,0,0,0};
static const bool seg9[] = {0,0,0,0,1,0,0};

/* @brief array of pointers that are pointing into array of booleans in order to iterate through them */
static const bool (* const segPtr[])[SEGMENTS] = {&seg0,&seg1,&seg2,&seg3,&seg4,&seg5,&seg6,&seg7,&seg8,&seg9};

static const size_t segPtrSize = (sizeof(segPtr) / sizeof(bool*));

/* @brief class handles SW1 & SW3 init, reading and informs system of state changing */
class CSwitchHanlder
{
private:  
  int SW1_last_state;
  int SW3_last_state;

  int SW1_current_state;
  int SW3_current_state;

  bool SW1_enabled;
  bool SW3_enabled;

  void initSw();
  void readSw();
public:
  CSwitchHanlder();
  ~CSwitchHanlder() = default;

  void checkSw();
  bool isSwStateUpdated() const;
  inline bool isSW1Enabled() const { return SW1_enabled; }
  inline bool isSW3Enabled() const { return SW3_enabled; }
};

void CSwitchHanlder::initSw()
{
  for(uint8_t i=ARD_PIN_SW_1; i<=ARD_PIN_SW_3; ++i)
  {
    pinMode(i, INPUT);
  }
}

void CSwitchHanlder::readSw()
{
  //read from switches
  SW1_current_state = digitalRead(ARD_PIN_SW_1);
  SW3_current_state = digitalRead(ARD_PIN_SW_3);
}

CSwitchHanlder::CSwitchHanlder() :
  SW1_last_state{-1},
  SW3_last_state{-1},
  SW1_enabled{false},
  SW3_enabled{false}
{
  CSwitchHanlder::initSw();
}

void CSwitchHanlder::checkSw()
{
  CSwitchHanlder::readSw();
  // reset booleans
  SW1_enabled = SW3_enabled = false;

  // check is current state same as last
  SW1_enabled = (SW1_current_state != SW1_last_state);
  if (SW1_enabled)
  {
    // if not avoid check for other SW
    goto assign;
  }
  // check is current state same as last
  SW3_enabled = (SW3_current_state != SW3_last_state);
assign:
  SW1_last_state = SW1_current_state;
  SW3_last_state = SW3_current_state;
}

bool CSwitchHanlder::isSwStateUpdated() const
{
  return (SW1_enabled | SW3_enabled);
}

/* @brief class handles 7-Seg init, updates counter according to switch states and writes LOW/HIGH signals to 7-seg outputs */
class C7SegHanlder
{
private:
  uint8_t counter;
  int8_t incrementStep;

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
  incrementStep{1}
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
    incrementStep = (sw.isSW1Enabled()) ? 1 : -1;
  }
  if (!counter && -1 == incrementStep)
  {
    // return to UPPER value
    counter = segPtrSize;
  }
  counter = (counter+incrementStep) % segPtrSize;
  C7SegHanlder::updateDisplay(*segPtr[counter]);
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
