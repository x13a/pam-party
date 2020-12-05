# pam-party

PAM module. The idea was to have "secret" password that can be used to
bypass authentication and run some command before unlock. For little
security salt is prepanded and the result token is hashed with sha256.

This module requires pam and openssl libraries to compile.

## Security

Do not consider this module as secure. To do right not sha256, but scrypt or
alternative password hashing algo must be used.

## Installation
```sh
$ make
$ sudo make install
```

## Usage

First, before build, you must modify some vars.

You have to change user (empty it if you do not need to check user):
```sh
$ sed -ie 's/_USER_/YOUR_USER/' ./src/pam_party.c
```

Then you have to change salt:
```sh
$ sed -ie 's/_SALT_/YOUR_SALT/' ./src/pam_party.c
```

Then you have to generate hash:
```sh
$ echo -n 'YOUR_SALTYOUR_PASSWORD' | sha256sum
$ sed -ie 's/_SHA256_HASH_/YOUR_HASH/' ./src/pam_party.c
```

Now build and install.

After that you have to modify pam.d config files. For example you can add
first line to common-auth:
```text
auth  sufficient                  pam_party.so  /usr/local/sbin/your-command
auth  [success=1 default=ignore]  pam_unix.so   nullok_secure
```

Now you can bypass authentication using your "secret" password. One more thing
is that your-command must return 0 or PAM_IGNORE will be returned.

## Friends
- [pam_panic](https://github.com/pampanic/pam_panic)
