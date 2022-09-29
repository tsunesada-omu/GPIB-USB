#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TDatime.h"
#include <time.h>

class DCVoltage {
private:
  int nLines;
  TH1F *hDCV;
  TH2F *hBox;
  TGraph *gDCV;
  void Fill(const char *fileName);
public:
  DCVoltage();
  DCVoltage(const char *fileName);  
  ~DCVoltage();
  void Draw(const char *title);
};

static int count_lines(const char *fileName, TDatime *tstart, TDatime *tend);

DCVoltage::DCVoltage() {
  
}
DCVoltage::DCVoltage(const char *fileName) {
  TDatime tstart, tend;
  nLines = count_lines(fileName, &tstart, &tend);
  hDCV = new TH1F(fileName, fileName, 200, 23.9, 24.1);
  hDCV->SetLineColor(4);
  hDCV->SetLineWidth(2);
  hDCV->GetXaxis()->SetTitle("DC output [V]");
  hDCV->GetYaxis()->SetTitle("Entries");
  
  char name[1024];
  sprintf(name, "%s-time", fileName);
  gDCV = new TGraph(nLines);
  gDCV->GetXaxis()->SetTimeDisplay(1);

  gDCV->GetXaxis()->SetTimeOffset(tstart.Convert(kTRUE));
  gDCV->GetXaxis()->SetTimeFormat("%m\/%d %H:%M");
  gDCV->SetMarkerColor(2);
  gDCV->SetMarkerStyle(20);
  gDCV->SetLineColor(2);
  gDCV->SetLineStyle(1);
  gDCV->SetLineWidth(2);
  
  double tlen = tend.Convert(kTRUE) - tstart.Convert(kTRUE);
  hBox = new TH2F("DC24V", "DC24V", tlen, 0, tlen, 1, 23.9, 24.1);
  hBox->SetStats(kFALSE);
  hBox->GetXaxis()->SetTimeDisplay(1);
  hBox->GetXaxis()->SetTimeOffset(tstart.Convert(kTRUE));
  hBox->GetXaxis()->SetTimeFormat("%m\/%d %H:%M");
  hBox->GetXaxis()->SetTitle("Time");
  hBox->GetYaxis()->SetTitle("DC output [V]");
  Fill(fileName);
}
DCVoltage::~DCVoltage() {
  
}

static time_t str2time(const char *datestr, const char *timestr);
DCVoltage::Fill(const char *fileName) {
  TDatime tstart, tend;
  nLines = count_lines(fileName, &tstart, &tend);
  FILE *fp = fopen(fileName, "r");
  char datestr[32], timestr[32];
  char buf[256];
  double dcv;
  int n = 0;
  while (fgets(buf, sizeof(buf), fp)) {
    if (buf[0] == '#') continue;
    int k = sscanf(buf, "%s %s %lf", datestr, timestr, &dcv);
    if (k == 3) {
      TDatime t = str2Datime(datestr, timestr);
      hDCV->Fill(dcv);
      gDCV->SetPoint(n, t.Convert(kTRUE) - tstart.Convert(kTRUE), dcv);
      n++;
    } else {
      continue;
    }     
  }
  fclose(fp);
}
DCVoltage::Draw(const char *title) {
  TCanvas *c = new TCanvas(title, "DC24V", 1024, 768);
  c->SetFillColor(0);
  c->SetFrameFillColor(0);
  c->Divide(1, 2);
  c->cd(1);
  gPad->SetGrid();
  gPad->SetFillColor(0);
  gPad->SetFrameFillColor(0);
  hBox->Draw();
  gDCV->Draw("same");

  
  c->cd(2);
  gPad->SetGrid();
  gPad->SetFillColor(0);
  gPad->SetFrameFillColor(0);
 
  hDCV->Draw();
}
void dc24v() {

  DCVoltage v = DCVoltage("24V-turtle.dat");
  v.Draw("Turtle");
  
  DCVoltage v = DCVoltage("24V-2.dat");
  v.Draw("Eta");  
}


static time_t str2Datime(const char *datestr, const char *timestr);
static int count_lines(const char *fileName, TDatime *tstart, TDatime *tend) {
  FILE *fp = fopen(fileName, "r");
  char datestr[32], timestr[32];
  char buf[256];
  double dcv;
  int n = 0;
  while (fgets(buf, sizeof(buf), fp)) {
    if (buf[0] == '#') continue;
    int k = sscanf(buf, "%s %s %lf", datestr, timestr, &dcv);
    if (k == 3) {
      n++;
    } else {
      continue;
    }    
    *tend = str2Datime(datestr, timestr);   
    if (n == 1) {
      *tstart = *tend;
    }
  }
  fclose(fp);
  return n;
}

static TDatime str2Datime(const char *datestr, const char *timestr) {
  int yr, day, mon, h, m, s;
  sscanf(datestr, "%4d/%02d/%02d", &yr, &mon, &day);
  sscanf(timestr, "%02d:%02d:%02d", &h, &m, &s);  
  return TDatime(yr, mon, day, h, m, s);
}