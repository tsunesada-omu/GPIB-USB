#include "HttpDevice.h"

HttpDevice::HttpDevice() {
}

HttpDevice::HttpDevice(const char *addr) {
  strcpy(ipAddr, addr);
}

HttpDevice::~HttpDevice() {
}
