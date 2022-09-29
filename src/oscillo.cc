/*
 * This is an example program to use Tektronix digital oscilloscope.
 *
 * Measure time delay between two signals, from CH1 to CH2.
 */
#include "PrologixGpibUsb.h"

using namespace std;

static void define_signal_handler();
static bool STOPFLAG = false;

static const int DATALEN = 65536;
static void initOscilloScope(PrologixGpibUsb *device);

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(20);
    char buf[256];
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    
    define_signal_handler();

    initOscilloScope(device);

    while (true) {
      time_t t;
      time(&t);
      device->ReceiveResponse("MEASU:IMM:VALUE?", buf);
      printf("%f\n", atof(buf)*1000);    // [ms]
      //      sleep (1);
      if (STOPFLAG == true) break;
    }

    delete device;
  } catch (string& e) {
    cerr << e << endl;
  }
  return 0;
}

static void signal_handler_SIGINT(int flag);
static void signal_handler_SIGTERM(int flag);
static void define_signal_handler()
{
  struct sigaction act1, act2;
  act1.sa_handler = &signal_handler_SIGINT;
  sigfillset(&act1.sa_mask);
  act1.sa_flags = 0;
  sigaction(SIGINT, &act1, 0);
  
  act2.sa_handler = &signal_handler_SIGTERM;
  sigfillset(&act2.sa_mask);
  act2.sa_flags = 0;
  sigaction(SIGTERM, &act2, 0);  
}

static void signal_handler_SIGINT(int flag) {
  STOPFLAG = true;
  cerr << "SIGINT detected." << endl;
}
static void signal_handler_SIGTERM(int flag) {
  signal_handler_SIGINT(flag);
  
}

static void initOscilloScope(PrologixGpibUsb *device) {
  device->SendCommand("HOR:SCALE 400e-6");
  
  for (int i = 1; i <= 2; i++) {
    char cmd[256];
    sprintf(cmd, "CH%1d:SCALE 500e-3", i);
    device->SendCommand(cmd);
    sprintf(cmd, "CH%1d:IMP FIFTY", i);
    device->SendCommand(cmd);
  }

  /* Delay measurement:
   * From CH1 to CH2
   */
  device->SendCommand("MEASU:IMM:TYPE DELAY");
  device->SendCommand("MEASU:IMM:DELAY:SOURCE1 CH1");
  device->SendCommand("MEASU:IMM:DELAY:SOURCE2 CH2");
  device->SendCommand("MEASU:IMM:DELAY:DIRECTION FORWARD");
  device->SendCommand("MEASU:IMM:DELAY:EDGE1 FALL");
  device->SendCommand("MEASU:IMM:DELAY:EDGE2 FALL");
}
