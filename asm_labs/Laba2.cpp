#include <iostream>

int main() {
    int x;
    std::cin >> x;

    //Задание 1
    double cpp = (2.0 * x - 1) * (2.0 * x + 1) * (x + 3) / (2.0 * x);
    int int_cpp = int(cpp);
    int asm_ans;

    __asm {
        mov eax, x
        mov ebx, eax
        add ebx, ebx
        mov ecx, ebx
        sub ecx, 1
        mov edx, ebx
        add edx, 1
        imul ecx, edx
        mov edx, eax
        add edx, 3
        imul ecx, edx
        mov eax, ecx
        cdq
        idiv ebx
        mov asm_ans, eax
    }

    std::cout << "cpp " << int_cpp << " asm " << asm_ans << std::endl;

    //Задание 2
    unsigned int reversed_cpp = 0;
    unsigned int temp = x;
    while (temp != 0) {
        reversed_cpp = reversed_cpp * 10 + temp % 10;
        temp /= 10;
    }

    unsigned int reversed_asm;

    __asm{
        mov eax, x
        xor ebx, ebx

        reverse_loop:
        test eax, eax
            jz end_loop
            mov ecx, 10
            xor edx, edx
            div ecx
            imul ebx, 10
            add ebx, edx
            jmp reverse_loop
            end_loop :
        mov reversed_asm, ebx
    }

    std::cout << " c++: " << reversed_cpp <<  " asm: " << reversed_asm << std::endl;

    return 0;
}