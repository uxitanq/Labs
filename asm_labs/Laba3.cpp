#include <iostream>
#include <limits>

int gcd(int a, int b) {
    int result;
    __asm {
        mov eax, a
        mov ebx, b
        loop_start :
        cmp ebx, 0
            je loop_end
            xor edx, edx
            div ebx
            mov eax, ebx
            mov ebx, edx
            jmp loop_start
            loop_end :
        mov result, eax
    }
    return result;
}

int main() {
    /**int a;
    std::cout << "a = ";
    std::cin >> a;

    //Задание 1
    int res;
    __asm {
        mov eax, a
        mov ebx, eax
        imul ebx, ebx
        imul ebx, ebx
        mov ecx, ebx
        imul ecx, ebx
        imul ecx, ebx
        imul ebx, ebx
        add ecx, ebx
        add ecx, eax
        mov res, ecx
    }

    std::cout << res << std::endl;
    
    //Задание 2
    std::cout << std::endl;
    int count = 0;
    for (int x = 1; x <= 24; x++) {
        int y, remainder;
        __asm {
              mov eax, 50
              mov ebx, 2
              imul ebx, x
              sub eax, ebx
              mov ebx, 3
              cdq
              idiv ebx
              mov y, eax
              mov remainder, edx
            }

         if (remainder == 0 && y > 0){
             count++;
             int check;
             __asm {
                 mov eax, 2
                 imul eax, x
                 mov ebx, 3
                 imul ebx, y
                 add eax, ebx
                 mov check, eax
              }

             std::cout << "2*" << x << " + 3*" << y << " = " << check << std::endl;
            }
        }
    std::cout << "Total solutions: " << count << std::endl;**/

    //Задание 3
    unsigned int prev = 0;  // F(0)
    unsigned int curr = 1;  // F(1)
    unsigned int next;
    unsigned int count = 2;
    std::cout << "F(0) = " << prev << std::endl;
    std::cout << "F(1) = " << curr << std::endl;
    while (true) {
        if (curr > std::numeric_limits<unsigned int>::max() - prev) {
            std::cout << "overflow" << std::endl;
            break;
        }
        __asm {
            mov eax, prev
            add eax, curr
            mov next, eax
        }
        std::cout << "F(" << count << ") = " << next << "\n";
        prev = curr;
        curr = next;
        count++;
    }
    std::cout << "Total Fibonacci numbers that fit in 32-bit: " << count << std::endl;

    //Задание 4
    int a, b;
    std::cout << "a = ";
    std::cin >> a;
    std::cout << "b = ";
    std::cin >> b;
    if (b <= 0) {
        std::cout << "b must be a natural number" << std::endl;
        return 1;
    }
    int c = gcd(abs(a), b);
    int aa = a / c;
    int bb = b / c;
    std::cout << "Original: " << a << "/" << b << std::endl;
    std::cout << "Simplified: " << aa << "/" << bb << std::endl;
    return 0;
}