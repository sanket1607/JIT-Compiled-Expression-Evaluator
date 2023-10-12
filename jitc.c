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
struct jitc {
    void *handle;
};

int jitc_compile(const char *input, const char *output) {

/**
 *  * Loads a dynamically loadable module into the calling process' memory for
 *   * execution.
 *    *
 *     * pathname: the file pathname of the dynamically loadable module
 *      *
 *       * return: an opaque handle or NULL on error
 *        */
    pid_t p_id; 
    int status;
    char *input_file = (char*)input;
    char *output_file = (char*)output;


    p_id = fork();
    if (!p_id) {
     
        char *args[8]; 
        args[0] = "gcc";
        args[1] = "-o3";
        args[2] = "-fpic";
        args[3] = "-shared";
        args[4] = "-o";
        args[5] = output_file;
        args[6] = input_file;
        args[7] = NULL;
        execv("/usr/bin/gcc",args);
        exit(0);
    }
    else {
        waitpid(-1, &status, 0);
        if (WIFEXITED(status)) {
            return 0;
        }
        else
            return -1;
    }

}

struct jitc *jitc_open(const char *pathname) {
    struct jitc *so_file_handle = (struct jitc*) malloc(sizeof(struct jitc));
    so_file_handle->handle = dlopen(pathname, RTLD_LAZY | RTLD_LOCAL);
    if(!so_file_handle)
        return NULL;

    return so_file_handle;
}

void jitc_close(struct jitc *jitc) {
    if (jitc == NULL)
      return; 

    dlclose(jitc->handle);
    free(jitc);
}

long jitc_lookup(struct jitc *jitc, const char *symbol) {

    void *function_address;
    function_address = dlsym(jitc->handle, symbol);
    if(function_address == NULL)
        return 0;
       
    return (long)function_address;
}
