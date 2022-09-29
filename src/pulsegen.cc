#include "PrologixGpibUsb.h"

using namespace std;
static void define_signal_handler();
static bool STOPFLAG = false;


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
  cerr << "SIGINT detected. (hit Enter to exit)" << endl;
}
static void signal_handler_SIGTERM(int flag) {
  signal_handler_SIGINT(flag);
  
}

static void dev_setup(PrologixGpibUsb *dev) {
    dev->SendCommand("SOURCE1:VOLT:HIGH 0V");
    sleep(1);

    dev->SendCommand("SOURCE1:VOLT:LOW -0.8V");
    sleep(1);

    dev->SendCommand("SOURCE1:PULSE:WIDTH 1us");
    sleep(1);
    
    dev->SendCommand("OUTPUT1:POL INV");
    sleep(1);


    dev->SendCommand("OUTPUT1:STAT ON");
    sleep(1);

    dev->SendCommand("SOURCE1:BURST:STAT ON");
    sleep(1);
    dev->SendCommand("SOURCE1:BURST:MODE TRIG");
    sleep(1);
    dev->SendCommand("SOURCE1:BURST:NCYC 1");
    sleep(1);
    dev->SendCommand("TRIG:SOURCE EXT");
    sleep(1);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./pulsegen <GPID addr>" << endl;
    exit(0);
  }
  define_signal_handler();
  try {
    int addr = atoi(argv[1]);
    PrologixGpibUsb *dev = new PrologixGpibUsb(addr);
    char buf[256];

    dev_setup(dev);

    while (true) {
      dev->SendCommand("TRIG");
      printf("TRIG!\n");
      sleep(1);
      if (STOPFLAG) break;
    }

    delete dev;

  } catch (string &e) {
    cerr << e << endl;
    exit(1);
  }

  return 0;
}
