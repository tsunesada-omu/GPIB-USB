require("mkmf")



$INCFLAGS += " -I../include"
$LDFLAGS += " -L/usr/local/lib -L../lib -L."

have_library("ftd2xx")

have_library("gpibusb")

create_makefile("gpib")
