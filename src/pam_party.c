#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PAM_SM_AUTH

#include <security/pam_appl.h>
#include <security/pam_modules.h>

#include "argon2.h"

#define VERSION "0.2.0"

#define HASH_LEN 32
#define TIME_COST 1
#define MEMORY_COST 1<<16
#define PARALLELISM 4

#define USER "_USER_"
#define SALT "_SALT_"
#define HASH "_ARGON2ID_HASH_"

PAM_EXTERN int
pam_sm_authenticate(
	pam_handle_t *pamh, 
	int flags, 
	int argc, 
	const char *argv[]
) {
	if (strlen(USER)) {
		const char *user = NULL;
		if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS || !user) {
			return PAM_IGNORE;
		}
		if (strcmp(user, USER) != 0) {
			return PAM_IGNORE;
		}
	}
	const char *authtok = NULL;
	if (
		pam_get_authtok(pamh, PAM_AUTHTOK, &authtok, NULL) != PAM_SUCCESS || 
		!authtok
	) {
		return PAM_IGNORE;
	}
	uint8_t raw_hash[HASH_LEN];
	if (argon2id_hash_raw(
		TIME_COST,
		MEMORY_COST,
		PARALLELISM,
		authtok,
		strlen(authtok),
		SALT,
		strlen(SALT),
		raw_hash,
		HASH_LEN
	) != ARGON2_OK) {
		return PAM_IGNORE;
	}
	char hash[(HASH_LEN<<1) + 1];
	for (unsigned int i = 0; i < HASH_LEN; i++) {
		if (sprintf(&hash[i << 1], "%02x", raw_hash[i]) < 0) {
			return PAM_IGNORE;
		}
	}
	if (strcmp(hash, HASH) != 0) {
		return PAM_IGNORE;
	}
	if (argc) {
		char *args[argc + 1];
		int i, status;
		pid_t pid = fork();
		switch (pid) {
		case -1:
			return PAM_IGNORE;
		case 0:
			for (i = 0; i < argc; i++) {
				args[i] = strdup(argv[i]);
			}
			args[argc] = NULL;
			execv(args[0], args);
			int err = errno;
			i = 0;
			while (args[i]) {
				free(args[i++]);
			}
			_exit(err);
		default:
			if (waitpid(pid, &status, 0) != pid || status != 0) {
				return PAM_IGNORE;
			}
		}
	}
	return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
	return PAM_IGNORE;
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY("pam_party");
#endif
