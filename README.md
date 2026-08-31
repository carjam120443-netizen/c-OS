# c-OS 🐧🖥️

**c-OS** is an experimental 32-bit x86 operating system built from scratch as a learning and development project.

> 🚧 **Early development:** c-OS is currently a work in progress and is not ready for everyday use.

## 🐧 c-OS identity

```text
  .--.
 |o_o |
 |:_/ |
//   \\ \\
(|     | )
/\\_   _/\\
\\___)=(___/
```

The project uses an original Tux-inspired ASCII logo as its distro art.

## 🎯 Goals

- Build and understand a real operating system from the ground up
- Create a custom kernel
- Add keyboard and display support
- Develop a command-line shell and userspace
- Build a native package manager inspired by BSD package tooling
- Add a lightweight native desktop
- Eventually add memory management, processes, filesystems, and networking
- Keep the project open source and easy to experiment with

## 🛠️ Current stack

- **Language:** C
- **Architecture:** i686 / 32-bit x86
- **Bootloader:** GRUB / Multiboot
- **Kernel:** Custom c-OS kernel
- **Filesystem:** FAT-based filesystem work in progress
- **Shell:** Native c-OS shell
- **Package manager:** `pkg`
- **Repository configuration:** `/etc/pkg/bsd`
- **Desktop:** Native framebuffer desktop with lightweight BSD-inspired design
- **Build toolchain:** `i686-elf-gcc`, `i686-elf-as`, and `i686-elf-ld`

## 📦 Package manager

c-OS's `pkg` command is being designed around ideas from BSD package systems. The repository configuration currently includes an OpenBSD repository definition for repository metadata and future compatible package builds.

```text
pkg update
pkg search <name>
pkg install <name>
pkg remove <name>
pkg list
pkg info <name>
```

OpenBSD's `neofetch` port exists as `sysutils/neofetch` and depends on Bash. c-OS cannot execute an OpenBSD binary package directly, so the repository contains a dedicated c-OS Neofetch configuration for a future native port instead of pretending the OpenBSD package is already compatible. citeturn0search6turn0search5

## 🖥️ Desktop

c-OS includes a lightweight native framebuffer desktop foundation with shortcuts for Terminal, Files, Settings, and Package Manager. It is intentionally native to c-OS rather than attempting to embed an OpenBSD X11 desktop wholesale.

## 🚀 Current status

c-OS can build a bootable ISO with the custom kernel, keyboard polling, interactive shell, filesystem components, users/sudo foundation, package-manager foundation, and framebuffer desktop work.

## 🗺️ Roadmap

- [x] Boot a minimal kernel
- [x] Print text to the screen
- [x] Handle keyboard input
- [x] Create a basic shell
- [x] Add a filesystem foundation
- [x] Add users and sudo foundation
- [x] Add package-manager foundation
- [x] Add a native framebuffer desktop foundation
- [ ] Add interrupts
- [ ] Complete memory management
- [ ] Complete package repository/network backend
- [ ] Add process management
- [ ] Add networking
- [ ] Finish persistent userspace
- [ ] Finish native Neofetch implementation
- [ ] Create a more complete graphical environment

## 📦 Building

Build instructions will be expanded as the project develops. The current build targets the `i686-elf` cross-toolchain and produces a GRUB-bootable c-OS ISO.

## 📜 License

License information will be added when the project reaches a stable enough stage to define its licensing model.

---

**c-OS — starting from nothing, one subsystem at a time.** 🐧🚀