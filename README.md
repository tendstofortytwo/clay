# Clay - a basic x86 operating system

## Goals

* [ ] Become comfortable with C and x86 assembly
* [x] Create a system that boots with no help (other than a bootloader like GRUB) on an x86-based machine
* [ ] Inside that system, create an environment to read and write text to a file
* [ ] Read code from a file and execute it in this operating environment
    -  [ ] TODO: What language?

## Build and run

Install cross-compiler building dependencies from here: https://wiki.osdev.org/GCC_Cross-Compiler#Installing_Dependencies. Also install `xorriso`, `grub2-pc` and `qemu-system-x86` (or equivalent) packages to build and run ISO.

    # Load environment
    source setup/prefix.sh

    # Install dependencies (one-time)
    setup/build-binutils.sh
    setup/build-gcc.sh

    # Build kernel and ISO
    cd kernel
    make

    # Run on QEMU
    # -d int produces debugging output on interrupts
    # -no-reboot, -no-shutdown useful to catch CPU before it triple faults and resets
    qemu-system-i386 -cdrom build/clay.iso -d int -no-reboot -no-shutdown

## Resources

* OSDev wiki: https://wiki.osdev.org
* OSDev subreddit: https://reddit.com/r/OSDev

## License

Code is provided under the MIT license; see LICENSE.md.
