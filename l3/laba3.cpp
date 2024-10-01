#include <iostream>
#include <cmath>
#include <cstdlib>

#define ll long long 

void idk (ll sum){
    std::cout << "sum = " << sum << std::endl; 
}

int main (){
  std::cout << "matrix size (n * n)";
  ll n;
  std::cin >> n;
  if (n <= 0){
     std::cout << "N should be positive" << std::endl;
     return 0;
  }
  if (n > 10){
     std::cout << "N should be less than 11" << std::endl;
     return 0;
  }
  ll** a {new ll*[n]{}};

  for (unsigned i{}; i < n; i++){
        a[i] = new ll[n]{};
    }

  bool f = false;
while (!f){
  std::cout << "Matrix (" << n << ", " << n << ") - ";
  ///ll* a = new ll[n][n];
  for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
          std::cin >> a[i][j];

  std::cout << std::endl;

  for (int i = 0; i < n; i++){
     for (int j = 0; j < n; j++)
         ///sum += a[i*j];
        std::cout << a[i][j] << " ";
    std::cout << std::endl;
  }
  
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
        if (a[i][j] != a[n-j-1][n - i - 1]){
            std::cout << "a[i][j] should be equal to a[n-j-1][n - i - 1]. Pls enter your matrix again";
            std::cout << std::endl;
            f = true; 
        }
    }
  }
}

  ll nlm = 0;
  for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++){
          bool f = true;
            if ((i != 0 && a[i-1][j] <= a[i][j]) || 
                (i < n-1  && a[i+1][j] <= a[i][j]) ||
                (j != 0 && a[i][j-1] <= a[i][j]) ||
                (j < n-1 && a[i][j+1] <= a[i][j]) || 
                (i != 0 && j != 0 && a[i-1][j-1] <= a[i][j]) ||
                (i != 0 && j < n-1 && a[i-1][j+1] <= a[i][j]) ||
                (i < n-1 && j != 0 && a[i+1][j-1] <= a[i][j]) ||
                (i < n-1 && j < n-1 && a[i+1][j+1] <= a[i][j]))
                   f = false;

            if (f) {
                nlm++;
            }
        }

  std::cout << nlm << std::endl;
  ll sum = 0;
  for (int i = 0; i < n; i++){
     for (int j = i + 1; j < n; j++){
         sum += a[i][j];
         ///std::cout << a[i*j] << " ";
     }
  }
  
  idk(sum);

  for (unsigned i{}; i < n; i++){
        delete[] a[i];
    }
  delete []a;
  return 0;
}