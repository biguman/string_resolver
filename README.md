# String Resolver
A low-level C project messing around with runtime string construction and 
obfuscation — no plaintext strings sitting around for `strings(1)` to grab.

## Files
- **test.c** — Linux-only (64-bit). Has environment fingerprinting to bail 
  on sandboxes/VMs before doing anything.
- **wintest.c** — Trying to make it cross-platform. Linux works fine; 
  Windows decode is still broken (WIP).

## How it works
Strings get built at runtime instead of sitting in the binary:

- **Heap allocation arithmetic** — consecutive allocation deltas produce 
  consistent constants, assembled byte-by-byte into the target string
- **XOR encoding** — sensitive values stored as encoded hex, decoded at 
  runtime using a key derived from the same heap deltas
- **Environment fingerprinting** *(test.c only)* — enumerates CPU model via 
  `/proc/cpuinfo` (AMD/Intel detection), checks RAM (`/proc/meminfo`) and 
  disk size (`/proc/partitions`) before proceeding

## Platform
`test.c` is **64-bit Linux only** — the heap arithmetic depends on glibc's 
allocator behavior. Tested on Ubuntu WSL2 (22.04) and Ubuntu 24.04 x86_64.

`wintest.c` swaps allocators via `#ifdef _WIN32` (`HeapCreate`/`HeapAlloc` 
on Windows, `malloc` on Linux). Windows heap layout is different enough that 
the key derivation doesn't port cleanly yet.

## Usage
```bash
gcc -o test test.c && ./test
gcc -o wintest wintest.c && ./wintest
```
For Windows: MinGW or MSVC targeting x86_64.

## Notes
Experimental/learning project.

ASLR behavior differs across architectures (32-bit vs 64-bit) causing 
inconsistent heap deltas — considering a custom arena or personal `malloc()` 
wrapper to normalize allocation gaps across targets.
