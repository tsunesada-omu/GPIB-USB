/**
 * hvtest.cc
 * 
 * BASJEチェレンコフ光観測に使うHVをテストするためのプログラム
 * 
 * HVは入力としてDC24Vを、出力制御用の0-2.5V入力、およびモニタ用の0-2.5V出力がある。
 * この３つをKeithley DMM2000 で測定する。
 * 
 * 2011/Jun
 */
#include "PrologixGpibUsb.h"

using namespace std;

static void define_signal_handler();
static bool STOPFLAG = false;

void digital_write(int i) {
  char cmd[256];
  sprintf(cmd, "ssh pi@192.168.1.156 \"sudo ~/Source/DAC_ctrl.py %d\"", i);
  system(cmd);
}

int main(int argc, char *argv[]) {
  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(16);
    char buf[256];
    
    //device->SendCommand("*RST");
    //device->SendCommand("*OPT?");
    //device->ReceiveResponse(buf);   
    //cerr << buf;  // Displayed "0,200X-SCAN": OK.
    //device->SendCommand(":ROUTE:SCAN:LSELECT INTERNAL");
    //device->SendCommand(":ROUTE:SCAN:INTERNAL (@1:3)");
    //device->SendCommand(":ROUTE:SCAN:INTERNAL?");
    //device->ReceiveResponse(buf);
    //cout << buf;

    //device->SendCommand(":FUNCTION 'VOLTAGE:DC'");
    //device->SendCommand(":FUNCTION?");
    //device->ReceiveResponse(buf);
    //cout << buf;
    device->SendCommand(":SYSTEM:BEEPER 0");

    //define_signal_handler();
    
    //while (true) {
    for (int i = 0; i < 4096; ++i) {
    cout << "test in for" << endl;
      time_t t;
      time(&t);
      //device->SendCommand(":READ?");
      //device->ReceiveResponse(buf);
      //double volc = atof(buf);  // Control (0-2.5V)
      digital_write(i);
      device->SendCommand(":READ?");
    cout << "test befor recres" << endl;
      device->ReceiveResponse(buf);      
    cout << "test after recres" << endl;
      double volm = atof(buf);  // Monitor out (0-2.5V)
      //device->SendCommand(":READ?");
      //device->ReceiveResponse(buf);      
      //double vol24 = atof(buf);  // Vs (24V)

      char timestr[256];
      strftime(timestr, sizeof(timestr), "%Y/%m/%d %H:%M:%S", localtime(&t));
      printf("%s %d %f\n", timestr, i, volm);
      //sleep (10);
      //if (STOPFLAG == true) break;
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
