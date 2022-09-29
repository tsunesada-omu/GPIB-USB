#ifndef ___MYDEVICE_H___
#define ___MYDEVICE_H___


#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <time.h>
#include <ftd2xx.h>

class MyDevice {
  virtual DWORD SendCommand(const char *wBuf);
  virtual DWORD ReceiveResponse(char rBuf[]);
  virtual DWORD ReceiveResponse(const char *cmd /*!< Command to send */, char rBuf[] /*!< Response from the device */);
};

#endif
