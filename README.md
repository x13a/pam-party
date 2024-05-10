# pam-party

**Partially rewritten to Zig. Not working on latest version. For archive.**

PAM module. If bad guys will force you to unlock, you can use this 
module to run some command before unlock using "secret" credits.

The difference with using pam_exec is that all authentication is done 
internally and `authtok` is not exposed.

## Installation
```sh
$ make
$ sudo make install
```

## Usage

You have to change user (empty it if you do not need to check user):
```sh
$ sed -ie 's/_USER_/YOUR_USER/' ./src/pam.zig
```

You have to generate hash:
```sh
$ echo -n "YOUR_PASSWORD" | argon2 "YOUR_SALT" -id -t 1 -m 16 -p 4
$ sed -ie 's/_HASH_/YOUR_ENCODED_HASH/' ./src/pam.zig
```

Now build and install.

After that you have to modify pam.d config files. Add to the beginning of 
screen lock configuration file and change `USER` to your username:
```text
auth  sufficient  /usr/local/lib/security/pam_party.so  /usr/bin/mkdir -p /home/<USER>/Desktop/pam_party_test_dir
```

Now you can unlock using your "secret" credits. One more thing is that 
`your-command` should exit success or PAM_IGNORE will be returned.

## Related

* [pam_panic](https://github.com/pampanic/pam_panic)
