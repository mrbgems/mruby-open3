/*
** mrb_open3.c - Open3 module
**
** Copyright (c) Takashi Kokubun 2016
**
** See Copyright Notice in LICENSE
*/

#include <unistd.h>
#include "mruby.h"
#include "mruby/value.h"
#include "mrb_open3.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

// `spawn` is defined under `Open3` since it's incomplete and it should be added
// to `Process` by "mruby-process" mrbgem.
static mrb_value mrb_open3_spawn(mrb_state *mrb, mrb_value self)
{
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);

  pid_t pid = fork();
  if (pid == 0) {
    execlp("echo", "echo", "hello world", NULL);
  }
  return mrb_fixnum_value(pid);
}

void mrb_mruby_open3_gem_init(mrb_state *mrb)
{
  struct RClass *open3;
  open3 = mrb_define_module(mrb, "Open3");
  mrb_define_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_ANY());
  mrb_define_class_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_ANY());
  DONE;
}

void mrb_mruby_open3_gem_final(mrb_state *mrb)
{
}
