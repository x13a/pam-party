const Builder = @import("std").build.Builder;

pub fn build(b: *Builder) void {
    const mode = b.standardReleaseOptions();
    const lib = b.addSharedLibrary("pam_party", "src/pam.zig", .unversioned);
    lib.setBuildMode(mode);
    lib.addIncludeDir("/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/usr/include/");
    lib.linkSystemLibrary("pam");
    lib.install();
}
