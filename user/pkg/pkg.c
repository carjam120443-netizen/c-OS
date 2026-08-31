/*
 * c-OS package manager prototype.
 *
 * Inspired by the command-line workflow of OpenBSD's package tools,
 * but implemented specifically for c-OS. This is currently a userspace
 * prototype: package installation is not connected to the c-OS filesystem yet.
 */

#include <stdio.h>
#include <string.h>

static void usage(void) {
    puts("cOS-pkg - c-OS package manager");
    puts("");
    puts("Usage:");
    puts("  pkg install <package>   Install a package");
    puts("  pkg remove <package>    Remove a package");
    puts("  pkg info [package]      Show package information");
    puts("  pkg search <term>       Search the package repository");
    puts("  pkg update              Refresh package metadata");
    puts("  pkg upgrade             Upgrade installed packages");
    puts("  pkg help                Show this help message");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "help") == 0) {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "install") == 0 ||
        strcmp(argv[1], "remove") == 0 ||
        strcmp(argv[1], "info") == 0 ||
        strcmp(argv[1], "search") == 0) {
        if (argc < 3) {
            fprintf(stderr, "pkg: '%s' requires a package name or search term\n", argv[1]);
            return 2;
        }
        printf("pkg: %s '%s' (not implemented yet)\n", argv[1], argv[2]);
        return 0;
    }

    if (strcmp(argv[1], "update") == 0 ||
        strcmp(argv[1], "upgrade") == 0) {
        printf("pkg: %s (not implemented yet)\n", argv[1]);
        return 0;
    }

    fprintf(stderr, "pkg: unknown command '%s'\n", argv[1]);
    usage();
    return 2;
}
