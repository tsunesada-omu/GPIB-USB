/*
 * An interactive session with a GPIB device
 *
 * 2011/Sep/02 by Y.Tsunesada
 */
#include "PrologixGpibUsb.h"
#include <readline/history.h>
#include <readline/readline.h>

using namespace std;

static int parse_argv(int argc, char *argv[]);
static void define_signal_handler();
static bool STOPFLAG = false;

static const int DATALEN = 65536;

int main(int argc, char *argv[]) {

  try {
    int addr = parse_argv(argc, argv);
    PrologixGpibUsb *device = new PrologixGpibUsb(addr);
    char buf[65536];
    define_signal_handler();

    while (true) {
      char *cmd = readline("command> ");
      if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) break;
      device->SendCommand(cmd);
      if (cmd[strlen(cmd)-1] == '?') {
	device->ReceiveResponse(buf);
	printf("response: %s", buf);
      }
      add_history(cmd);
      free(cmd);
      if (STOPFLAG == true) break;
    }

    delete device;
  } catch (string& e) {
    cerr << e << endl;
  }
  return 0;
}

static void signal_handler_SIGINT(int flag);
static void signal_handler_SIGTERM(int flag);
static void define_signal_handler()
{
  struct sigaction act1, act2;
  act1.sa_handler = &signal_handler_SIGINT;
  sigfillset(&act1.sa_mask);
  act1.sa_flags = 0;
  sigaction(SIGINT, &act1, 0);
  
  act2.sa_handler = &signal_handler_SIGTERM;
  sigfillset(&act2.sa_mask);
  act2.sa_flags = 0;
  sigaction(SIGTERM, &act2, 0);  
}

static void signal_handler_SIGINT(int flag) {
  STOPFLAG = true;
  cerr << "SIGINT detected. (hit Enter to exit)" << endl;
}
static void signal_handler_SIGTERM(int flag) {
  signal_handler_SIGINT(flag);
  
}

static int parse_argv(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./interact <GPIB addr>" << endl;
    exit(0);
  }
  return atoi(argv[1]);
}
