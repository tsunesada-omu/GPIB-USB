#ifndef ___WFMPRE_H___
#define ___WFMPRE_H___

/**
 * テクトロニクスのTDS3000シリーズの波形データプリアンブル
 */
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>

//#include "TObject.h"

using namespace std;


enum Wfmpre_Encoding {
  ENC_ASCII,
  ENC_BINARY,
};

enum Wfmpre_Source {
  SRC_CH1,
  SRC_CH2,
};

enum Wfmpre_BnFmt {
  FMT_RI,
  FMT_RP,
};

enum Wfmpre_ByteOrder {
  ORD_LSB,
  ORD_MSB,
};

enum Wfmpre_PtFmt {
  PTFMT_Y,
  PTFMT_ENV,
};

//class Wfmpre : public TObject {
class Wfmpre {
 private:
  char wfmpre[256];
  int ch;      // 1 for CH1, 2 for CH2
  int byt_nr;  // Number of bits per waveform point (1 or 2)
  int bit_nr;  // the data width for the waveform to be transferred (8 or 16)
  Wfmpre_Encoding enc;
  Wfmpre_BnFmt bn_fmt; // Format of binary data 
  Wfmpre_ByteOrder byt_or;  //
  int nr_pt; // Record length (500 or 10000)
  char wfid[256];
  Wfmpre_PtFmt pt_fmt;  // Format of the reference waveform
  double xincr;   //Interval (seconds per point) between samples
  double pt_off;  // Always 0
  double xzero;   // the position of the first sample of the waveform in XUNits.
  double ymult;   // Vertical scale factor
  double yzero;   // Vertical offset of the waveform in YUNits (V).
  double yoff;    // Vertical position of the waveform in waveform data point levels (not Volt, -128-127 or 0-255)

  void scan();
 public:
  Wfmpre();
  Wfmpre(const char *str);
  ~Wfmpre();
  static const int WFMPRELEN = 200;
  void print();
  int Channel()  const { return ch; }
  int BYT_Nr() const { return byt_nr; }
  int BIT_Nr() const { return bit_nr; }
  Wfmpre_Encoding ENCdg()  const { return enc; }
  Wfmpre_BnFmt BN_Fmt() const { return bn_fmt; }
  Wfmpre_ByteOrder BYT_Or() const { return byt_or; }
  int NR_Pt() const { return nr_pt; }
  const char* WFID() const { return wfid; }
  Wfmpre_PtFmt PT_FMT() const { return pt_fmt; }
  double XINcre() const { return xincr; }
  double PT_Off() const { return pt_off; }
  double XZERo() const { return xzero; }
  double YMUlt() const { return ymult; }
  double YZERo() const { return yzero; }
  double YOFf() const { return yoff; }
  const char* toString() const { return wfmpre; }
  void CalcVolt(const unsigned char data[], double volt[]);
  //  ClassDef(Wfmpre,2)
};

#endif

