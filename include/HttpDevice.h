#ifndef ___HTTPDEVICE_H___
#define ___HTTPDEVICE_H___

#include "MyDevice.h"

class HttpDevice {
 private:
  char ipAddr[32];
 public:
  HttpDevice();
  HttpDevice(const char *addr /*!< IP address */);
  ~HttpDevice();
};

#endif
