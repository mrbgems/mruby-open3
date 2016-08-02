/*
** mrb_open3.c - Open3 class
**
** Copyright (c) Takashi Kokubun 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_open3.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
} mrb_open3_data;

static const struct mrb_data_type mrb_open3_data_type = {
  "mrb_open3_data", mrb_free,
};

static mrb_value mrb_open3_init(mrb_state *mrb, mrb_value self)
{
  mrb_open3_data *data;
  char *str;
  int len;

  data = (mrb_open3_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_open3_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_open3_data *)mrb_malloc(mrb, sizeof(mrb_open3_data));
  data->str = str;
  data->len = len;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_open3_hello(mrb_state *mrb, mrb_value self)
{
  mrb_open3_data *data = DATA_PTR(self);

  return mrb_str_new(mrb, data->str, data->len);
}

static mrb_value mrb_open3_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "hi!!");
}

void mrb_mruby_open3_gem_init(mrb_state *mrb)
{
    struct RClass *open3;
    open3 = mrb_define_class(mrb, "Open3", mrb->object_class);
    mrb_define_method(mrb, open3, "initialize", mrb_open3_init, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, open3, "hello", mrb_open3_hello, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, open3, "hi", mrb_open3_hi, MRB_ARGS_NONE());
    DONE;
}

void mrb_mruby_open3_gem_final(mrb_state *mrb)
{
}

