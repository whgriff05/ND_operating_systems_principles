# Operating Systems Principles: The Beginnings


## Motivation: The Computing Stack

1. Users
2. Applications (Systems Programming)
3. Libraries (Systems Programming)
4. Operating System / Kernel (Operating Systems)
5. Hardware (Operating Systems)

## Computer Hardware

A computer is a __general purpose information processing__ machine

A __digital computer__ is an electronic device that processes __binary data__

### Architecture

Inside the __processor__
- __ALU__: performs the arithmetic operations
- __Controller__: controls the ALU's input/output and operations

The processor connects to __RAM__, which is fast but limited. If we need more storage
we go through the RAM to the __disk (storage)__ which is slower but larger.

RAM also connects to __input devices__ and __output devices__

### Devices

A modern computer is a collection of smaller __specialized__ computing devices that 
__interface / work together__ by __communicating__ using a variety of protocols and interfaces

## Boot Sequence (BIOS/UEFI)

1. BIOS (Basic Input Output System) / UEFI (Unified Extensible Firmware Interface)
    - Checks computer usability (input, output, RAM, etc.)
    - Looks for MBR (Master Boot Record) / GPT (General Partition Table) --> how the hard drive is partitioned
2. MBR / GPT
    - Shows how the hard drive is partitioned (to find the bootloader)
3. Bootloader
    - Prepares the computer to load the kernel
4. Kernel
5. Init System (typically __systemd__ on Linux)
    - To open applications / services
    - To be the "last resort" of processes

*Note: on some machines, step 2 (and sometimes step 3) can be skipped*

We need all of these steps due to hardware restraints (MBR is ~400 B, Bootloader is in the MB scale)
