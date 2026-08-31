# c-OS package manager (`pkg`)

`pkg` is the planned command-line package manager for c-OS. Its command style is inspired by OpenBSD's package tooling, while the package format, repository format, and implementation will be designed specifically for c-OS.

OpenBSD's `pkg_add` supports installing and updating packages and resolving dependencies; c-OS will eventually implement a simpler system around its own package format and repository. citeturn0search3turn0search7

## Planned commands

```text
pkg install <package>
pkg remove <package>
pkg info [package]
pkg search <term>
pkg update
pkg upgrade
pkg help
```

## Prototype status

The current `pkg.c` file is only a command-line prototype. It parses the planned commands but does not install software yet.

Future work includes:

- Package archives
- Package metadata
- Dependency resolution
- Repository indexes
- Checksums/signatures
- Installed-package database
- Safe file extraction
- Package upgrades and removal
- Integration with the c-OS filesystem and userspace
