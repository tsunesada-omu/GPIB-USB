#include "PrologixGpibUsb.h"

using namespace std;

static void define_signal_handler();
static bool STOPFLAG = false;

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(11);  // 11: AFG3102
    char buf[256];
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);

    device->SetGpibAddr(20);  // 20: TDS3202B
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);

    device->SetGpibAddr(11);  // 11: AFG3102
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    
    //    device->SendCommand("SOURCE1:FUNC:SHAPE GAUS");
    device->SendCommand("SOURCE1:FUNC:SHAPE PULSE");
    device->SendCommand("SOURCE1:PULSE:WIDTH 100ns");
    device->SendCommand("SOURCE1:VOLTAGE:LEVEL:IMM:HIGH 100mV");
    device->SendCommand("SOURCE1:VOLTAGE:LEVEL:IMM:LOW 0mV");

    delete device;
  } catch (string& e) {
    cerr << e << endl;
  }
  return 0;
}
