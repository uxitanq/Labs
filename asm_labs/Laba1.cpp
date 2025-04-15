#include <iostream>
#include <cstdint>

int main() {
    uint16_t a1 = 2;
    uint16_t a2 = 0;
    uint16_t a3 = 0;
    uint16_t b1 = 8;
    uint16_t b2 = 0;
    uint16_t b3 = 0;
    uint16_t c1, c2, c3, carry;

    __asm {
        mov ax, a1
        add ax, b1
        mov c1, ax
        jnc no_carry1
        mov carry, 1
        jmp next1
        no_carry1 :
        mov carry, 0
            next1 :
            mov ax, a2
            add ax, b2
            add ax, carry
            mov c2, ax
            jnc no_carry2
            mov carry, 1
            jmp next2
            no_carry2 :
        mov carry, 0
            next2 :
            mov ax, a3
            add ax, b3
            add ax, carry
            mov c3, ax
    }

    std::cout << "C1: 0x" << std::hex << c1 << std::endl;
    std::cout << "C2: 0x" << std::hex << c2 << std::endl;
    std::cout << "C3: 0x" << std::hex << c3 << std::endl;

    uint64_t full_result = (uint64_t)c3 << 32 | (uint64_t)c2 << 16 | c1;
    std::cout << "Full result (decimal): " << std::dec << full_result << std::endl;

    return 0;
}