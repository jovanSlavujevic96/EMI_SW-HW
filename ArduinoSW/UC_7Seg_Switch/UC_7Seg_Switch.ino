#include "CSwitchHandler.h"
#include "C7SegHandler.h"

static CSwitchHandler handler_sw;
static C7SegHandler handler_7seg;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // nothing to do -> all init is done on handler_7seg & handler_sw constructors
}

// the loop function runs over and over again forever
void loop() 
{
  handler_sw.checkSw();
  handler_7seg.updateCounter(&handler_sw);
  delay(1000); // wait for a second
}
