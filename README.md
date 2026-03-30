# String Resolver

A low-level C program exploring runtime string construction and obfuscation techniques without hardcoding sensitive values in plaintext. The encoded hex variable can be built in runtime to not be hardcoded. Will change in future push.

For usage, simply compile with "gcc -o test test.c"

## Overview

Instead of embedding strings (e.g. IP addresses) directly in source code where they'd be trivially readable via `strings(1)` or static analysis, this project builds target strings at runtime using:

- **Heap allocation arithmetic** — exploiting the consistent 32-byte alignment between consecutive `malloc()` calls to derive known integer constants, which are then bitmask-assembled byte-by-byte into the target string
- **XOR encoding** — sensitive values are stored as encoded hex constants and decoded at runtime using a dynamically generated XOR key (also derived from heap allocation deltas)
- **Environment fingerprinting** — checks RAM (`/proc/meminfo`) and disk size (`/proc/partitions`) to detect sandbox or VM environments before proceeding

## Concepts Demonstrated

- Manual byte extraction via bitshifting and masking
- Heap memory layout and `malloc` block alignment
- Runtime key generation without hardcoded secrets
- Linux `/proc` filesystem for system introspection

## Notes

This is an experimental/learning project. Some legacy code is intentionally left in with comments explaining the reasoning behind it. First legacy code left behind is a hardcoded to build string "127.0.0.1" using the same bitmasking technique without the XOR obfuscation.
I do realize the issue of different ASLR between different systems, possible writing of an arena or a personal malloc() func in the code to gain consistency.
