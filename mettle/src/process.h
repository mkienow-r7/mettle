#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <ev.h>
#include "buffer_queue.h"

struct process;
struct progmgr;

struct procmgr * procmgr_new(struct ev_loop *loop);

void procmgr_free(struct procmgr *mgr);

typedef void (*process_exit_cb_t)(struct process *, int exit_status, void *arg);

typedef	void (*process_read_cb_t)(struct process *, struct buffer_queue *queue, void *arg);

struct process_options {
	const char *args;               /* Process arguments (none if not specified) */
	char **env;                     /* Process environment (inherited if not specified) */
	const char *process_name;       /* Alternate process name */
	const char *cwd;                /* Current working directory */
	const char *user;               /* User to start the process as */
};

/*
 * Create a new process
 */

// Supported flag values
#define PROCESS_CREATE_SUBSHELL		0x00000001

struct process * process_create_from_executable(struct procmgr *mgr,
	const char *file,
	struct process_options *opts, unsigned int flags);

struct process * process_create_from_binary_image(struct procmgr *mgr,
	const unsigned char *bin_image, size_t bin_image_len,
	struct process_options *opts, unsigned int flags);

void process_set_callbacks(struct process *p,
	process_read_cb_t stdout_cb,
	process_read_cb_t stderr_cb,
	process_exit_cb_t exit_cb,
	void *cb_arg);

/*
 * Write to the process stdin
 */
ssize_t process_write(struct process *p, const void *buf, size_t nbyte);

/*
 * Reads from stdout/stderr
 */
ssize_t process_read(struct process *p, void *buf, size_t nbyte);

/*
 * Kill the given process. Cleanup happens asynchronously from this call.
 */
int process_kill(struct process* process);

/*
 * Enable nonblocking stdio on this process
 */
void process_set_nonblocking_stdio(void);

/*
 * Returns the managed process for a given PID
 */
struct process * process_by_pid(struct procmgr *mgr, pid_t pid);

/*
 * Kill the managed process for a given PID
 */
int process_kill_by_pid(struct procmgr *mgr, pid_t pid);

/*
 * Returns the PID of the given process
 */
pid_t process_get_pid(struct process *p);

#endif
