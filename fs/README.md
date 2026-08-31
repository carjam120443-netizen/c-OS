# c-OS Base Filesystem

This is the initial filesystem layout for c-OS. It defines the directories the OS will eventually mount and populate.

Current layout:

- `/bin` essential user commands
- `/boot` boot-related files
- `/dev` device nodes
- `/etc` system configuration
- `/home` user home directories
- `/lib` shared libraries
- `/proc` process/kernel information
- `/root` administrator home
- `/tmp` temporary files
- `/usr` user applications and data
- `/var` variable system data

These are currently represented as a filesystem image specification. The kernel does not mount this filesystem yet.