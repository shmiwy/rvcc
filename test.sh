#!/bin/bash

assert() {
    input="$1"
    expect="$2"

    ./rvcc "$1" > tmp.s || exit

    "$RISCV"/bin/riscv64-unknown-linux-gnu-gcc -static -o tmp tmp.s
    
    "$RISCV"/bin/qemu-riscv64 -L "$RISCV"/sysroot ./tmp

    actual="$?"

    if [ "$actual" = "$expect" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expect expected, but got $actual"
        exit 1
    fi
}


assert 1 1
assert 42 42

assert '10+20+4' 34

echo OK