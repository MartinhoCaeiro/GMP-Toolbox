gcc -Wall -g -o cca2526-23917-23919-ex23 cca2526-23917-23919-ex23.c -lgmp -lcrypto
# GMP Toolbox

Collection of small C programs that explore number theory and cryptography tasks using the GNU Multiple Precision Library (GMP). A simple text menu (`src/menu.c`) lets you pick and run any exercise binary inside `src/scripts`.

## What is inside
- Prime utilities: primality checks, prime listing with/without file output, and factorization helpers.
- Exponentiation utilities: basic exponentiation, timing comparisons, and performance graphs.
- Combinatorics: combinations counter.
- RSA utilities: several key generators and timing experiments.
- File encryption/decryption: toy exercises including RC4 and symmetric key variations.

Executables live in `src/scripts/` and are named `cca2526-23917-23919-exN` (and matching `.c` sources). The `makefile` in that folder builds any `exN` by name.

## Prerequisites
- C compiler with POSIX `system` available (tested with `gcc`).
- GMP development files (`-lgmp`).
- OpenSSL/LibreSSL crypto library for the encryption exercises (`-lcrypto`).

## Build
From `src/scripts/`, build any exercise (for example, exercise 23):

```sh
make cca2526-23917-23919-ex23   # uses gcc -Wall -g -lgmp by default
```

To build the menu launcher from the repo root:

```sh
gcc -Wall -g -o menu src/menu.c
```

If your toolchain is not picking up GMP or libcrypto automatically, add include/library paths as needed, e.g. `-I/path/to/include -L/path/to/lib -lgmp -lcrypto`.

## Run
- Run a specific exercise directly, e.g. `./src/scripts/cca2526-23917-23919-ex12`.
- Or launch the menu and pick an option:

```sh
./menu
```

## Notes
- Some exercises that rely on cryptography use `-lcrypto`; others only need `-lgmp`.
- The prime utilities include large probable-prime searches; expect longer runtimes for large inputs.

## Prime numbers
- About the prime numbers, the number 19999999999999900019 is "PROBABLY A PRIME". It was hard to find a number to this condition.

## Authors
- Martinho José Novo Caeiro — 23917
- Paulo António Tavares Abade — 23919

## License
- This repository is licensed under the GNU General Public License v3.0 (GPL-3.0).