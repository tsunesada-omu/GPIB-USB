#include "PrologixGpibUsb.h"

using namespace std;

static void define_signal_handler();
static bool STOPFLAG = false;

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(16);
    char buf[256];
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    
    define_signal_handler();
    
    while (true) {
      time_t t;
      time(&t);
      device->ReceiveResponse(":MEASURE:VOLTAGE:DC?", buf);
      char timestr[256];
      strftime(timestr, sizeof(timestr), "%Y/%m/%d %H:%M:%S", localtime(&t));
      printf("%s %6.3f\n", timestr, atof(buf));
      sleep (10);
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
