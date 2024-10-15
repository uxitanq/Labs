#include <fstream>
#include <string>

int main() {
  std::ifstream fin("input.txt");
  std::ofstream fout("output1.txt");
  std::ofstream kout("output2.txt");
  std::string s;

  while (getline(fin, s)) {
    std::size_t vibe = s.find("(");
    std::size_t viber = s.rfind(")");
    if (vibe != std::string::npos && viber != std::string::npos && vibe < viber) {
      bool fanumtax = false;
      for (int i = vibe + 1; i < viber; i++) {
        if (s[i] != ' ') {
          fanumtax = true;
          break;
        }
      }
      if (fanumtax) {
        fout << s << std::endl;
      }
    }

    vibe = s.find("[");
    viber = s.rfind("]");
    if (vibe != std::string::npos && viber != std::string::npos && vibe < viber) {
      bool fanumtax = false;
      for (int i = vibe + 1; i < viber; i++) {
        if (s[i] != ' ') {
          fanumtax = true;
          break;
        }
      }
      if (fanumtax) {
        kout << s << std::endl;
      }
    }
  }
  return 0;
}