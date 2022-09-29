/* 
 * File:   PrologixGpibUsb.cc
 * Author: yoshiki
 * 
 * Created on 2011/05/28, 16:06
 */

#include "PrologixGpibUsb.h"

PrologixGpibUsb::PrologixGpibUsb() {
  FT_STATUS ftStatus;
  DWORD numDevs;
  char *BufPtrs[2]; // pointer to array of 3 pointers
  char Buffer1[64]; // buffer for description of first device

  // initialize the array of pointers
  BufPtrs[0] = Buffer1;
  ftStatus = FT_ListDevices(BufPtrs, &numDevs, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);
  if (ftStatus == FT_OK) {
  } else {
  }  
  strcpy(controllerName, Buffer1);
  
  ftStatus = FT_Open(0, &ftHandle);
  if (ftStatus != FT_OK) { // FT_Open failed
    printf("GPIB-USB controller: open failed\n");
    exit(1);
  }  
  this->SetDefault();
  this->SetTimeouts(1000, 0);
}

PrologixGpibUsb::PrologixGpibUsb(int addr /*!< GPIB address */) {
  FT_STATUS ftStatus;
  DWORD numDevs;
  char *BufPtrs[2]; // pointer to array of 3 pointers
  char Buffer1[64]; // buffer for description of first device

  // initialize the array of pointers
  BufPtrs[0] = Buffer1;
  ftStatus = FT_ListDevices(BufPtrs, &numDevs, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);
  if (ftStatus == FT_OK) {
  } else {
  }  
  strcpy(controllerName, Buffer1);
  
  ftStatus = FT_Open(0, &ftHandle);
  if (ftStatus != FT_OK) { // FT_Open failed
    printf("GPIB-USB controller: open failed\n");
    exit(1);
  }  
  this->SetDefault();
  this->SetTimeouts(1000, 0);
  this->SetGpibAddr(addr);
}

PrologixGpibUsb::PrologixGpibUsb(const PrologixGpibUsb& orig) {
}

PrologixGpibUsb::~PrologixGpibUsb() {
  FT_Close(ftHandle);
}


DWORD PrologixGpibUsb::SendCommand(const char *wBuf) {
  DWORD bytesWritten;
  char writeBuf[256]; 
  sprintf(writeBuf, "%s\n", wBuf);   // Add LF
  size_t len = strlen(writeBuf);
  FT_STATUS status = FT_Write(ftHandle, (LPVOID) writeBuf, (unsigned long) len, &bytesWritten);

  if ((status != FT_OK) || (bytesWritten != len)) {
    printf("Unable to write to controller. Status: %d\r\n", status);
    return 0;
  }

  return bytesWritten;
}

DWORD PrologixGpibUsb::SendCommand(int addr, const char *wBuf) {
  this->SetGpibAddr(addr);
  return this->SendCommand(wBuf);
}
static const char LF   = 0x0A;
static const char CR   = 0x0D;
static const char ESC  = 0x1B;
static const char PLUS = 0x2B;


/*---------------------------------------------------------------------------*/
bool IsSpecial(const char c)
{
  // CR, LF and ESC characters have special meaning and
  // must be escaped if present in data
  //    return ((c == CR)||(c == LF)||(c == ESC)||(c == PLUS));
  return ((c == CR)||(c == LF)||(c == ESC));
}

DWORD PrologixGpibUsb::ReceiveResponse(char rBuf[]) {
  char *ptr = rBuf;
  DWORD nRead = 0;
  while (true) {
    DWORD bytesRead;
    FT_Read(ftHandle, ptr, 1, &bytesRead);  // Read 1 byte
    nRead += bytesRead;                     // Bytes read
    if (IsSpecial(*ptr)) {                  // Break if a terminator found
      ptr++;
      break;
    }
    ptr++;
  } 
  *ptr = '\0';  // terminate the string
  return nRead;
}

DWORD PrologixGpibUsb::ReceiveResponse(const char* cmd, char rBuf[]) {
  this->SendCommand(cmd);
  return this->ReceiveResponse(rBuf);  
}

DWORD PrologixGpibUsb::ReceiveResponse(int addr, const char* cmd, char rBuf[]) {
  this->SetGpibAddr(addr);
  return this->ReceiveResponse(cmd, rBuf);  
}

void PrologixGpibUsb::SetGpibAddr(int addr) {
  char buf[256];
  sprintf(buf, "++addr %d", addr);
  this->SendCommand(buf);
}

void PrologixGpibUsb::SetDefault() {
  SendCommand("++mode 1");  // controller mode
  SendCommand("++auto 1");  // automatical read after write
  SendCommand("++eos 3");   // no terminators added
  SendCommand("++eoi 1");  
}
