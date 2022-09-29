#include "PrologixGpibUsb.h"
#include "ruby.h"

#define RB_FUNC(f) reinterpret_cast<VALUE (*)(...)>(f)
#define RB_FINALIZER(func) ((void (*)(...))func)

static VALUE cGpib;
static VALUE cDataBuffer;

extern "C" void Init_gpib();

typedef struct _databuffer {
  size_t len;
  char *buf;
} DataBuffer;

DataBuffer* create_databuffer(size_t len) {
  DataBuffer *buf = (DataBuffer *) malloc(sizeof(DataBuffer));
  buf->len = len;
  buf->buf = (char *) malloc(len);
  return buf;
}

void free_databuffer(DataBuffer *buf) {
  free(buf->buf);
  free(buf);
}

DataBuffer *getDataBuffer(VALUE self) {
  DataBuffer *buf;
  VALUE obj = rb_iv_get(self, "body");
  Data_Get_Struct(obj, DataBuffer, buf);
  return buf;
}

static VALUE rb_databuffer_initialize(int argc, VALUE *argv, VALUE self) {
  DataBuffer *buf;
  size_t len;
  switch (argc) {
  case 1:
    len = FIX2INT(argv[0]);
    buf = create_databuffer(len);
    break;
  default:
    rb_raise(rb_eArgError, "Wrong number of arguments (%d for 1)", argc);
  }
  rb_iv_set(self, "body", Data_Wrap_Struct(cDataBuffer, 0, free_databuffer, buf));
  return Qnil;
}

static PrologixGpibUsb* getGpib(VALUE self) {
  PrologixGpibUsb *dev;
  VALUE obj = rb_iv_get(self, "body");
  Data_Get_Struct(obj, PrologixGpibUsb, dev);
  return dev;
}

static void gpib_free(PrologixGpibUsb *dev) {
  delete dev;
}

static VALUE rb_gpib_initialize(int argc, VALUE *argv, VALUE self) {
  if (argc != 1) {
    rb_raise(rb_eArgError, "Wrong number of arguments (%d for 1)", argc);
  }
  int addr = FIX2INT(argv[0]);
  PrologixGpibUsb *dev = new PrologixGpibUsb(addr);
  VALUE obj = Data_Wrap_Struct(cGpib, 0, gpib_free, dev);
  rb_iv_set(self, "body", obj);
  return Qnil;
}

static VALUE rb_gpib_send(int argc, VALUE *argv, VALUE self) {
  PrologixGpibUsb *dev = getGpib(self);;
  char *p;
  switch (argc) {
  case 1:
    Check_Type(argv[0], T_STRING);
    p = StringValueCStr(argv[0]);
    dev->SendCommand(p);
    return Qnil;
    break;
  case 2:
    FIXNUM_P(argv[0]);
    Check_Type(argv[1], T_STRING);
    p = StringValueCStr(argv[1]);
    dev->SendCommand(FIX2INT(argv[0]), p);
    return Qnil;
    break;
  default:
    rb_raise(rb_eArgError, "Wrong number of arguments (%d for 1 or 2)", argc);
  }
}

static VALUE rb_gpib_receive(int argc, VALUE *argv, VALUE self) {
  PrologixGpibUsb *dev = getGpib(self);
  switch (argc) {
  case 0:
    char buf[1024];
    dev->ReceiveResponse(buf);
    return rb_str_new2(buf);
    break;
  case 1:
    //    FIXNUM_P(argv[0]);

    //    dev->ReceiveResponse(cmd, buf);
    break;
  default:
    rb_raise(rb_eArgError, "Wrong number of arguments (%d for 1 or 2)", argc);
  }
  return Qnil;
}

extern "C" void Init_gpib() {
  cGpib = rb_define_class("Gpib", rb_cObject);

  rb_define_private_method(cGpib, "initialize", RUBY_METHOD_FUNC(rb_gpib_initialize), -1);

  rb_define_method(cGpib, "send", RUBY_METHOD_FUNC(rb_gpib_send), -1);
  rb_define_method(cGpib, "receive", RUBY_METHOD_FUNC(rb_gpib_receive), -1);

  cDataBuffer = rb_define_class("DataBuffer", rb_cObject);

  rb_define_private_method(cDataBuffer, "initialize", RUBY_METHOD_FUNC(rb_databuffer_initialize), -1);
}

