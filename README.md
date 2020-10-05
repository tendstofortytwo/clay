# Clay - a basic x86 operating system

## Goals

* [ ] Become comfortable with C and x86 assembly
* [x] Create a system that boots with no help (other than a bootloader like GRUB) on an x86-based machine
* [ ] Inside that system, create an environment to read and write text to a file
* [ ] Read code from a file and execute it in this operating environment
    -  [ ] TODO: What language?

## Build and run

    # Load environment
    source setup/prefix.sh

    # Install dependencies (one-time)
    setup/build-binutils.sh
    setup/build-gcc.sh

    # Build kernel
    cd kernel
    make

    # Run on QEMU
    qemu-system-i386 -cdrom build/clay.iso

## Resources

* OSDev wiki: https://wiki.osdev.org
* OSDev subreddit: https://reddit.com/r/OSDev

## License

Code is provided under the MIT license; see LICENSE.md.
