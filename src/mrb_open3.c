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

struct spawn_options {
  mrb_int out_dst;
  mrb_int err_dst;
  char *chdir;
};

static void
open3_spawn_process_options(mrb_state *mrb, mrb_value options_value, struct spawn_options *options)
{
  mrb_value out_value, err_value, chdir_value;

  out_value = mrb_hash_get(mrb, options_value, mrb_symbol_value(mrb_intern_lit(mrb, "out")));
  err_value = mrb_hash_get(mrb, options_value, mrb_symbol_value(mrb_intern_lit(mrb, "err")));
  chdir_value = mrb_hash_get(mrb, options_value, mrb_symbol_value(mrb_intern_lit(mrb, "chdir")));
  options->out_dst = mrb_int(mrb, out_value);
  options->err_dst = mrb_int(mrb, err_value);
  if (!mrb_nil_p(chdir_value)) {
    options->chdir = mrb_str_to_cstr(mrb, chdir_value);
  } else {
    options->chdir = NULL;
  }
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
// Limitation: Currently it expects the last argument to be a Hash like `{ out: ..., err: ... }`.
static mrb_value
mrb_open3_spawn(mrb_state *mrb, mrb_value self)
{
  char **cmd;
  pid_t pid;
  mrb_value *argv;
  mrb_int argc;
  struct spawn_options options;

  mrb_get_args(mrb, "*", &argv, &argc);
  if (argc == 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments (given 0, expected 1+)");
  }

  cmd = mrb_str_buf_to_cstr_buf(mrb, argv, argc-1);
  open3_spawn_process_options(mrb, argv[argc-1], &options);

  pid = fork();
  if (pid == 0) {
    dup2(options.out_dst, STDOUT_FILENO);
    dup2(options.err_dst, STDERR_FILENO);
    if (options.chdir != NULL) {
      chdir(options.chdir);
    }
    execvp(cmd[0], cmd);
  }
  return mrb_fixnum_value(pid);
}

void
mrb_mruby_open3_gem_init(mrb_state *mrb)
{
  struct RClass *open3;
  open3 = mrb_define_module(mrb, "Open3");
  mrb_define_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_REQ(1) | MRB_ARGS_REST());
  mrb_define_class_method(mrb, open3, "spawn", mrb_open3_spawn, MRB_ARGS_REQ(1) | MRB_ARGS_REST());
  DONE;
}

void
mrb_mruby_open3_gem_final(mrb_state *mrb)
{
}
