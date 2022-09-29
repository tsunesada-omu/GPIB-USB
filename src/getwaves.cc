/*
 * This is an example program to use Tektronix digital oscilloscope.
 *
 * Measure time delay between two signals, from CH1 to CH2.
 */
#include "PrologixGpibUsb.h"
#include "Wfmpre.h"
#include <time.h>




using namespace std;

static const int DATALEN = 10000;


static void initOscilloScope(PrologixGpibUsb *device);
static void storeRootFile(const Wfmpre *wpre1, const Wfmpre *wpre2,
			  const unsigned char wave[2][DATALEN],
			  const double volt[2][DATALEN]);

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(20);
    char buf[65536];
    //    unsigned short wave[DATALEN];
    unsigned char wave[2][DATALEN];

    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    
    initOscilloScope(device);
    time_t t = time(NULL);
    char timestr[256];
    strftime(timestr, sizeof(timestr), "# %Y/%m/%d %H:%M:%S", localtime(&t));

    device->SendCommand("DATA:SOURCE CH1");
    device->ReceiveResponse("WFMPRE?", buf);
    printf("%s\n", buf);
    Wfmpre *wpre1 = new Wfmpre(buf);
    printf("%s\n", wpre1->toString());

    device->SendCommand("DATA:SOURCE CH1");
    device->ReceiveResponse("CURVE?", buf);
    memcpy(wave[0], buf, DATALEN);
    //    wpre1->CalcVolt(wave[0], volt[0]);

    device->SendCommand("DATA:SOURCE CH2");
    device->ReceiveResponse("WFMPRE?", buf);
    Wfmpre *wpre2 = new Wfmpre(buf);
    device->SendCommand("DATA:SOURCE CH2");
    device->ReceiveResponse("CURVE?", buf);
    memcpy(wave[1], buf, DATALEN);
    //    wpre2->CalcVolt(wave[1], volt[1]);

    FILE *fp = fopen("waves.bin", "wb");
    printf("%d %d\n", strlen(wpre1->toString()), strlen(wpre2->toString()));
    fwrite(wpre1->toString(), sizeof(char), Wfmpre::WFMPRELEN, fp);
    fwrite(wpre2->toString(), sizeof(char), Wfmpre::WFMPRELEN, fp);
    fwrite(&t, sizeof(time_t), 1, fp);
    fwrite(wave[0], sizeof(unsigned char), DATALEN, fp);
    fwrite(wave[1], sizeof(unsigned char), DATALEN, fp);

    fclose(fp);



    delete device;
  } catch (string& e) {
    cerr << e << endl;
  }
  return 0;
}

static void initOscilloScope(PrologixGpibUsb *device) {
  
  for (int i = 1; i <= 2; i++) {
    char cmd[256];
    //    sprintf(cmd, "CH%1d:SCALE 200e-3", i);
    //    device->SendCommand(cmd);
    //    sprintf(cmd, "CH%1d:IMP MEG", i);
    //    device->SendCommand(cmd);
  }

  // unsigned, little endian
  device->SendCommand("DATA:ENC RPBINARY");
  device->SendCommand("DATA:WIDTH 1");

}
