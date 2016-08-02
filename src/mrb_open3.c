/*
** mrb_open3.c - Open3 module
**
** Copyright (c) Takashi Kokubun 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_open3.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

// `spawn` is defined under `Open3` since it's incomplete and it should be added
// to `Process` by "mruby-process" mrbgem.
static mrb_value mrb_open3_spawn(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "spawn!");
}

void mrb_mruby_open3_gem_init(mrb_state *mrb)
{
  struct RClass *open3;
  open3 = mrb_define_module(mrb, "Open3");
  mrb_define_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_open3_gem_final(mrb_state *mrb)
{
}
