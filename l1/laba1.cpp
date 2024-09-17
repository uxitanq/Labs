#include <iostream>
#include <cmath>
#include <iomanip>

int main (){
  double k1;
  std::cout << "k = ";
  std::cin >> k1;
  double x;
  std::cout << std::endl << "x =";
  std::cin >> x;
  double k = 1;
  for (int i = 0; i < k1; i++)
      k *= 0.1;
  //std::cout << k << std::endl;
  double res1 = sqrt (x + 1), res2 = 0;
  double sl = 22222;
  int n = 1;
  while (fabs (sl) >= k){
    if (n == 1){
      sl = 1;
      res2 = 1;
    }
    sl *= abs ((2 * (n - 1) - 1));
    sl /= (2 * (n - 1) + 2);
    sl *= x;
    if (!(n % 2 == true))
      res2 -= sl;
    else  
      res2 += sl;
    //std::cout << abs ((2 *(n - 1) - 1)) << " " << (2 * (n - 1) + 2) << " " << sl << " " <<
    //res2 << std::endl;
    n++; 
  }
  //int po = abs(log10(k));
  std::cout << std::fixed << std::setprecision(k1) << "res1 = " << res1 << std::endl <<
  "res2 = " << res2 << std::endl << "difference " << fabs (res2 - res1) << std::endl;
  return 0;
}