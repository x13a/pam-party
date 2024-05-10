const std = @import("std");
const mem = std.mem;
const c = @cImport({
    @cInclude("security/pam_appl.h");
    @cInclude("security/pam_modules.h");
});

const HASH = "_HASH_";
const USER = "_USER_";

pub export fn pam_sm_authenticate(
    pamh: ?*c.pam_handle_t,
    _: c_int,
    argc: c_int,
    argv: [*]?[*:0]const u8,
) c_int {
    if (argc == 0) return c.PAM_IGNORE;
    if (USER.len != 0) {
        var user: ?[*:0]const u8 = null;
        if (c.pam_get_user(pmah, &user, null) != c.PAM_SUCCESS)
            return c.PAM_IGNORE;
        if (!mem.eql(u8, &user, &USER))
            return c.PAM_IGNORE;
    }
    var authtok: ?[*:0]const u8 = null;
    if (c.pam_get_authtok(pamh, c.PAM_AUTHTOK, &authtok, null) != c.PAM_SUCCESS)
        return c.PAM_IGNORE;
    std.crypto.pwhash.argon2.strVerify(
        HASH,
        mem.span(authtok orelse return c.PAM_IGNORE),
        .{ .allocator = std.testing.allocator },
    ) catch return c.PAM_IGNORE;
    _ = argv[0] orelse return c.PAM_IGNORE;
    var proc = std.ChildProcess.init(&argv, std.heap.page_allocator);
    proc.stdin_behavior = .Ignore;
    proc.stdout_behavior = .Ignore;
    proc.stderr_behavior = .Ignore;
    proc.spawn() catch |_| return c.PAM_IGNORE;
    const term = proc.wait() catch |_| return c.PAM_IGNORE;
    std.testing.expectEqual(term, std.ChildProcess.Term{ .Exited = 0 }) catch |_| return c.PAM_IGNORE;
    return c.PAM_SUCCESS;
}

pub export fn pam_sm_setcred(
    _: ?*c.pam_handle_t,
    _: c_int,
    _: c_int,
    _: [*c][*c]const u8,
) c_int {
    return c.PAM_IGNORE;
}
