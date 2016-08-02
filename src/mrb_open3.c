/*
** mrb_open3.c - Open3 module
**
** Copyright (c) Takashi Kokubun 2016
**
** See Copyright Notice in LICENSE
*/

#include <unistd.h>
#include "mruby.h"
#include "mruby/string.h"
#include "mrb_open3.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static const char**
mrb_str_buf_to_cstr_buf(mrb_state *mrb, mrb_value *strs, mrb_int num)
{
  const char **ret;
  int i;

  ret = (const char **)mrb_malloc(mrb, sizeof(char *) * (num+1));
  for (i = 0; i < num; i++) {
    ret[i] = mrb_string_value_ptr(mrb, strs[i]);
  }
  ret[num] = NULL;
  return ret;
}

// `spawn` is defined under `Open3` since it's incomplete and it should be added
// to `Process` by "mruby-process" mrbgem.
//
// Limitation: Currently it expects the last argument to be a Hash.
static mrb_value
mrb_open3_spawn(mrb_state *mrb, mrb_value self)
{
  const char **cmd;
  pid_t pid;
  mrb_value *argv, options;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);

  cmd = mrb_str_buf_to_cstr_buf(mrb, argv, argc-1);
  // options = argv[argc-1];

  pid = fork();
  if (pid == 0) {
    execvp(cmd[0], cmd);
  }
  return mrb_fixnum_value(pid);
}

void
mrb_mruby_open3_gem_init(mrb_state *mrb)
{
  struct RClass *open3;
  open3 = mrb_define_module(mrb, "Open3");
  mrb_define_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_ANY());
  mrb_define_class_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_ANY());
  DONE;
}

void
mrb_mruby_open3_gem_final(mrb_state *mrb)
{
}
