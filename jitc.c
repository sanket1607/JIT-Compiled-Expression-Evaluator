/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * jitc.c
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include "system.h"
#include "jitc.h"

/**
 * Needs:
 *   fork()
 *   execv()
 *   waitpid()
 *   WIFEXITED()
 *   WEXITSTATUS()
 *   dlopen()
 *   dlclose()
 *   dlsym()
 */

/* research the above Needed API and design accordingly */

#include<stdio.h>
#include<stdlib.h>

int jitc_compile(const char *out_c_file, const char *out_so_file) {

  char buffer[1024];
  FILE *out_so_file_ptr;

  FILE *out_c_file_ptr = fopen(out_c_file, "r");
  if (out_c_file_ptr == NULL) {
    printf("Error opening out.c file.\n");
    return 1;
  }

  out_so_file_ptr = fopen(out_so_file, "w+");
  if (out_so_file_ptr == NULL) {
    printf("Error creating out.so file.\n");
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), out_c_file_ptr) != NULL) {
    fputs(buffer, out_so_file_ptr);
  }

  fclose(out_c_file_ptr);
  fclose(out_so_file_ptr);

  return 0;
}

