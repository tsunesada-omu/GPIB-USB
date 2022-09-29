/*
 * This is an example program to use Tektronix digital oscilloscope.
 *
 * Measure time delay between two signals, from CH1 to CH2.
 */
#include "PrologixGpibUsb.h"
#include "Wfmpre.h"
#include "TH1.h"
#include "TFile.h"

using namespace std;

static const int DATALEN = 10000;
static void initOscilloScope(PrologixGpibUsb *device);
static void storeRootFile(const Wfmpre *wpre1,
			  const unsigned char wave[2][DATALEN],
			  const double volt[2][DATALEN]);

int main(int argc, char *argv[]) {

  try {
    PrologixGpibUsb *device = new PrologixGpibUsb(20);
    char buf[65536];
    //    unsigned short wave[DATALEN];
    unsigned char wave[2][DATALEN];
    double volt[2][DATALEN];
    /*
    device->SendCommand("*IDN?");
    device->ReceiveResponse(buf);
    fprintf(stderr, "%s", buf);
    */
      initOscilloScope(device);
    device->SendCommand("DATA:SOURCE CH1");
    device->ReceiveResponse("WFMPRE?", buf);
    printf("%s\n", buf);
    Wfmpre *wpre1 = new Wfmpre(buf);
 

    device->SendCommand("DATA:SOURCE CH1");
    device->ReceiveResponse("CURVE?", buf);
    memcpy(wave[0], buf, DATALEN);
    wpre1->CalcVolt(wave[0], volt[0]);

    /*
    FILE *fp = fopen("wave.dat", "w");
    fprintf(fp, "# This data was obtained by getwaves program with the GPIB-USB library.\n");
    fprintf(fp, "# XINcre [sec]: %e\n", wpre1->XINcre());
    fprintf(fp, "# CH1 WFMPRE: %s", wpre1->toString());
    fprintf(fp, "# CH2 WFMPRE: %s", wpre2->toString());
    fprintf(fp, "# index ch1[V] ch2[V]\n");
    for (int i = 0; i < DATALEN; i++) {
      fprintf(fp, "%d %f %f\n", i, volt[0][i], volt[1][i]);
    }
    fclose(fp);
    */
    storeRootFile(wpre1, wave, volt);

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

static void storeRootFile(const Wfmpre *wpre1,
			  const unsigned char wave[2][DATALEN],
			  const double volt[2][DATALEN]) {

  TFile *f = new TFile("waves.root", "RECREATE");
  TH1F *h[2];
  TH1F *hvolt[2];
  for (int i = 0; i < 1; i++) {
    char str[256];
    sprintf(str, "CH%d-Raw", i+1);
    h[i] = new TH1F(str, str, DATALEN, 0, DATALEN);
    sprintf(str, "CH%d", i+1);
    hvolt[i] = new TH1F(str, str, DATALEN, 0, DATALEN*wpre1->XINcre());
    for (int j = 1; j <= DATALEN; j++) {
      h[i]->SetBinContent(j, wave[i][j]);
      hvolt[i]->SetBinContent(j, volt[i][j]);
    }
    f->WriteTObject(h[i]);
    f->WriteTObject(hvolt[i]);
  }
  f->Close();
}

