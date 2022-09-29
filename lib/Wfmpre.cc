#include "Wfmpre.h"

//ClassImp(Wfmpre)

Wfmpre::Wfmpre() {
}

Wfmpre::Wfmpre(const char *str) {
  strcpy(wfmpre, str);
  scan();
}

Wfmpre::~Wfmpre() {
}

void Wfmpre::scan() {
  const char *ptr = wfmpre;
  char buf[256], *ptr2 = buf;

  // BYT_Nr
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  byt_nr = atoi(buf);
  ptr2 = buf;

  // BIT_Nr
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  bit_nr = atoi(buf);
  ptr2 = buf;

  // Encoding
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  if (strcmp(buf, "ASC") == 0) {
    enc = ENC_ASCII;
  } else {
    enc = ENC_BINARY;
  }

 // Format
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  if (strcmp(buf, "RI") == 0) {
    bn_fmt = FMT_RI;
  } else {
    bn_fmt = FMT_RP;
  }

  // Byte order
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  if (strcmp(buf, "LSB") == 0) {
    byt_or = ORD_LSB;
  } else {
    byt_or = ORD_MSB;
  }

 // Number of data points
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  nr_pt = atoi(buf);
  ptr2 = buf;

  // WFID
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  strcpy(wfid, buf);
  ptr2 = buf;
  sscanf(wfid, "\"Ch%1d", &ch);

  // PT_FMT
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  if (strcmp(buf, "Y") == 0) {
    pt_fmt = PTFMT_Y;
  } else {
    pt_fmt = PTFMT_ENV;
  }
  // XINcr
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  xincr = atof(buf);

  // PT_OFf
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  pt_off = atof(buf);
  // XZERo
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  xzero = atof(buf);

  // XUNits
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;

 // YMUlt
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  ymult = atof(buf);
  // YZERo
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  yzero = atof(buf);
 // YOFf
  while (*ptr != ';') {
    *ptr2++ = *ptr++;
  }
  *ptr2 = '\0';
  ptr++;
  ptr2 = buf;
  yoff = atof(buf);
}

void Wfmpre::print() {
  printf("Channel: %d\n", ch);
  printf("BYT_Nr: %d\n", byt_nr);
  printf("BIT_Nr: %d\n", bit_nr);
  if (enc == ENC_ASCII) {
    printf("ENCdg: ASC\n");
  } else {
    printf("ENCdg: BIN\n");
  }
  if (bn_fmt == FMT_RI) {
    printf("BN_Fmt: RI\n");
  } else {
    printf("BN_Fmt: RP\n");
  }
  if (byt_or == ORD_LSB) {
    printf("BYT_Or: LSB\n");
  } else {
    printf("BYT_Or: MSB\n");
  }
  printf("NT_Pt: %d\n", nr_pt);
  printf("WFID: %s\n", wfid);
  if (pt_fmt == PTFMT_Y) {
    printf("PT_FMT: Y\n");
  } else {
    printf("PT_FMT: ENV\n");
  }
  printf("XINcr: %4.3E\n", xincr);
  printf("PT_OFf: %2.1E\n", pt_off);
  printf("XZERo: %4.3E\n", xzero);
  printf("YMUlt: %4.3E\n", ymult);
  printf("YZERo: %2.1E\n", yzero);
  printf("YOFf: %4.3E\n", yoff);
}

void Wfmpre::CalcVolt(const unsigned char data[], double volt[]) {
  for (int i = 0; i < nr_pt; i++) {
    volt[i] = yzero + ymult*(data[i] - yoff);
    if (volt[i] >= 9.9) volt[i] = 9.9;
    else if (volt[i] < -9.9) volt[i] = -9.9;
    else if (isnan(volt[i]) || isinf(volt[i])) volt[i] = 9.9;
    else;
  }
}
