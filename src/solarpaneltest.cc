/**
 * TEMPtest.cc
 * 
 */
#include "PrologixGpibUsb.h"
#include <ctime>

using namespace std;
static const int TSHORT = 300000;
static const int TLONG = 300000;
enum FUNCTYPE {VOLT, TEMP, NONE};

static void define_signal_handler();
static bool STOPFLAG = false;

void digital_write(int i) {
  char cmd[256];
  sprintf(cmd, "sudo /home/pi/Source/DAC_ctrl.py %d", i);
  system(cmd);
}

namespace dmm {
  void change(PrologixGpibUsb* device, FUNCTYPE functype) {
    char buf[256];
    while (1) {
      if (functype == TEMP)
        device->SendCommand(":FUNCTION 'FRES'");
      else if (functype == VOLT)
        device->SendCommand(":FUNCTION 'VOLTAGE:DC'");
      else if (functype == NONE)
        return;
      else
        exit(1);

      usleep(TSHORT);
      device->SendCommand(":FUNCTION?");
      usleep(TLONG);
      device->ReceiveResponse(buf);
      usleep(TSHORT);
  
      if (functype == TEMP) {
        if (buf[1] == 'F') break;
      } else if (functype == VOLT) {
        if (buf[1] == 'V') break;
      } else exit(1);
    }
  }
  
  double read(PrologixGpibUsb* device, int i, FUNCTYPE functype = NONE) {
    char cmd[256];
    char buf[256];
    sprintf(cmd, ":route:close (@%d)", i);
    device->SendCommand(cmd);
    //usleep(TLONG);
    usleep(TSHORT);
  
    dmm::change(device, functype);

    device->SendCommand(":READ?");
    usleep(TLONG);
    device->ReceiveResponse(buf);      
    usleep(TSHORT);
    return atof(buf);
  }
}


int main(int argc, char *argv[]) {
  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(16);
    device->SendCommand(":SYSTEM:BEEPER 0");
    sleep(1);

    define_signal_handler();
    
    //    digital_write(3000);

    const int MAX = 65535;
    for (int i = 0; i < MAX; ++i) {
      cerr << i << endl;
      time_t t;
      time(&t);


      double vola = dmm::read(device, 1, VOLT);  // analog 
      usleep(TSHORT);
      double volm = dmm::read(device, 2);        // monitor -> analog2
      usleep(TSHORT);
      double vol3 = dmm::read(device, 3);        // RPi 3.3V output 
      usleep(TSHORT);
      double volr = dmm::read(device, 4);        // regulated reference 
      usleep(TSHORT);
      double temp = dmm::read(device, 5, TEMP); // temperature
      usleep(TSHORT);

      char timestr[256];
      strftime(timestr, sizeof(timestr), "%Y/%m/%d %H:%M:%S", localtime(&t));
      printf("%s %d %f %f %f %f %f\n", timestr, i, vola, volm, vol3, volr, temp);
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

