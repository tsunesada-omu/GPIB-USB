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

int main(int argc, char *argv[]) {
  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(16);
    char buf[256];
    
    device->SendCommand("*RST");
    //    device->SendCommand("*OPT?");
    //    device->ReceiveResponse(buf);   
    //    cerr << buf;  // Displayed "0,200X-SCAN": OK.
    define_signal_handler();
    
    device->SendCommand(":SYSTEM:BEEPER 0");
    device->SendCommand(":SYSTEM:KCLICK 0");

    FILE *fp = fopen("R.dat", "a");
    while (true) {
      time_t t;
      time(&t);


      // Read resistance of the thermister (channel 4)
      device->SendCommand(":ROUT:CLOS (@4)");
      usleep(100);
      device->SendCommand("FUNCTION 'RES'");
      usleep(100);
      device->SendCommand("READ?");
      device->ReceiveResponse(buf);
      double thermo1 = atof(buf);
      
      // Read resistance of the PT thermosensor (channel 5)
      device->SendCommand(":ROUT:CLOS (@5)");
      usleep(100);
      device->SendCommand("FUNCTION 'FRES'");  // 4-wire measurement
      usleep(100);
      device->SendCommand("READ?");
      device->ReceiveResponse(buf);
      double pt = atof(buf);
      char datestr[256];
      strftime(datestr, sizeof(datestr), "%Y/%m/%d %H:%M:%S", localtime(&t));
      fprintf(fp, "%s %e %e\n", datestr, thermo1, pt);
      fflush(fp);
      sleep (10);
      if (STOPFLAG == true) break;
    }
    fclose(fp);
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
