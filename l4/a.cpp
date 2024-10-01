#include <iostream>
#include <cmath>
#include <cstdlib>

#define ll long long 
char *_my_strrchr(const char *s, char c) {
    char *res = NULL;
    while (*s){
        if (*s == c)
            res = (char *)s;
        s++;
    }
     if (c == '\0') {
        return (char *)s;
    }
    return res;
}

int main (){
  char s[301];
  std::cout << "s = " << std::endl;
  fgets(s, 301, stdin); 

  char c;
  std::cout << "c = " << std::endl;
  std::cin >> c;

  char *res = _my_strrchr(s, c);

  if (res != NULL) 
    std::cout << "The last occurrence of " << c << " - " << (void *)res << std::endl;
  else 
    std::cout << "The character " << c << "wasn't found in the string" << std::endl;
  return 0;
}