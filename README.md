# pam-party

PAM module. If bad guys will force you to unlock, you can use this 
module to run some command before unlock using "secret" credits.

The difference with using pam_exec is that all authentication is done 
internally and authtok is not exposed.

Requires pam and argon2 dev libraries to compile.

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

Now you have to change salt (min 8):
```sh
$ sed -ie 's/_SALT_/YOUR_SALT/' ./src/pam_party.c
```

And you have to generate hash:
```sh
$ echo -n "YOUR_PASSWORD" | argon2 "YOUR_SALT" -id -t 1 -m 16 -p 4
$ sed -ie 's/_ARGON2ID_HASH_/YOUR_HASH/' ./src/pam_party.c
```

Now build and install.

After that you have to modify pam.d config files. Add to the beginning of 
screen lock configuration file and change <USER> to your username:
```text
auth  sufficient  /usr/local/lib/security/pam_party.so  /usr/bin/mkdir -p /home/<USER>/Desktop/pam_party_test_dir
```

Now you can unlock using your "secret" credits. One more thing is that 
<your-command> should exit success or PAM_IGNORE will be returned.

## Friends
- [pam_panic](https://github.com/pampanic/pam_panic)
