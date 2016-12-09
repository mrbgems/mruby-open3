/*
** mrb_open3.c - Open3 module
**
** Copyright (c) Takashi Kokubun 2016
**
** See Copyright Notice in LICENSE
*/

#include <unistd.h>
#include "mruby.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "mrb_open3.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void
open3_spawn_process_options(mrb_state *mrb, mrb_value options, mrb_int *out_dst, mrb_int *err_dst)
{
  mrb_value out_value, err_value;

  out_value = mrb_hash_get(mrb, options, mrb_str_new_cstr(mrb, "out"));
  err_value = mrb_hash_get(mrb, options, mrb_str_new_cstr(mrb, "err"));
  *out_dst = mrb_int(mrb, out_value);
  *err_dst = mrb_int(mrb, err_value);
}

static char **
mrb_str_buf_to_cstr_buf(mrb_state *mrb, mrb_value *strs, mrb_int num)
{
  char **ret;
  mrb_int i;

  ret = (char **)mrb_malloc(mrb, sizeof(char *) * (num+1));
  for (i = 0; i < num; i++) {
    ret[i] = mrb_str_to_cstr(mrb, strs[i]);
  }
  ret[num] = NULL;
  return ret;
}

// `spawn` is defined under `Open3` since it's incomplete and it should be added
// to `Process` by "mruby-process" mrbgem.
//
// Limitation: Currently it expects the last argument to be a Hash like `{ "out"=>..., "err"=>... }`.
static mrb_value
mrb_open3_spawn(mrb_state *mrb, mrb_value self)
{
  char **cmd;
  pid_t pid;
  mrb_value *argv;
  mrb_int argc, out_dst, err_dst;
  mrb_get_args(mrb, "*", &argv, &argc);

  cmd = mrb_str_buf_to_cstr_buf(mrb, argv, argc-1);
  open3_spawn_process_options(mrb, argv[argc-1], &out_dst, &err_dst);

  pid = fork();
  if (pid == 0) {
    dup2(out_dst, STDOUT_FILENO);
    dup2(err_dst, STDERR_FILENO);
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
