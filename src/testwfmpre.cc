/*
 * This is an example program to use Tektronix digital oscilloscope.
 *
 * Measure time delay between two signals, from CH1 to CH2.
 */
#include "PrologixGpibUsb.h"
#include "Wfmpre.h"

using namespace std;

static void define_signal_handler();
static bool STOPFLAG = false;

static const int DATALEN = 10000;
static void initOscilloScope(PrologixGpibUsb *device);

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(20);
    char buf[65536];
    char wfmpre1[256], wfmpre2[256];
    //    unsigned short wave[DATALEN];
    unsigned char wave[DATALEN];
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    
    define_signal_handler();

    initOscilloScope(device);
    device->SendCommand("DATA:SOURCE CH1");
    device->ReceiveResponse("WFMPRE?", buf);
    strcpy(wfmpre1, buf);
    device->SendCommand("DATA:SOURCE CH2");
    device->ReceiveResponse("WFMPRE?", buf);
    strcpy(wfmpre2, buf);
    fprintf(stderr, "%s", wfmpre1);
    fprintf(stderr, "%s", wfmpre2);

    Wfmpre *pre1 = new Wfmpre(wfmpre1);
    Wfmpre *pre2 = new Wfmpre(wfmpre2);

    pre1->print();
    pre2->print();

    delete pre2;
    delete pre1;
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

  // unsigned, little endian
  device->SendCommand("DATA:ENC RPBINARY");
  device->SendCommand("DATA:WIDTH 1");

}




