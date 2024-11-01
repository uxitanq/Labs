#include <fstream>
#include <string>
#include <iostream>

//Я ЗАБЫЛА ЗАГРУЗИТЬ ФИНАЛЬНУЮ ВЕРСИЮ НА ГИТХАБ, УЖАС

int main() {
  std::ifstream fin("input.txt");
  std::ofstream fout("output1.txt");
  std::ofstream kout("output2.txt");
  std::string s;

  while (getline(fin, s)) {
    std::size_t vibe = 0;
    std::size_t viber = 0;
    bool lol = false;
    bool lmao = false;
    
    for (std::size_t i = 0; i < s.size(); i++) {
      if (s[i] == '(' && !lol) {
        vibe = i;
        lol = true;
      } else if (s[i] == ')' && lol){
        viber = i;
        lol = false;

        for (std::size_t j = vibe + 1; j < viber; j++){
          if (s[j] != ' ') {
            lmao = true;
            break;
          }
        }

        if (lmao) {
          fout << s << std::endl;
        }
        lmao = false;
      }
    }

    std::size_t skibiditoilet1 = 0;
    std::size_t skibiditoilet2 = 0;
    lol = false;
    lmao = false;
    
    for (std::size_t i = 0; i < s.size(); i++) {
      if (s[i] == '[' && !lol) {
        skibiditoilet1 = i;
        lol = true;
      } else if (s[i] == ']' && lol) {
        skibiditoilet2 = i;
        lol = false;

        for (std::size_t j = skibiditoilet1 + 1; j < skibiditoilet2; j++) {
          if (s[j] != ' ') {
            lmao = true;
            break;
          }
        }

        if (lmao) {
          kout << s << std::endl;
        }
        lmao = false; 
      }
    }
  }
  return 0;
}