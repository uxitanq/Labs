#include <iostream>
#include <cmath>
#include <cstdlib>

#define ll long long 

const ll nn = 10000;

ll sum (ll (&a)[nn], ll n){
  ll ss = 0;
  for (int i = 0; i < n; i++)
    if (a[i] > 0)
       ss += a[i];
  return ss;
}

ll min (ll a, ll b){
  if (a < b)
     return a;
  return b;
}

ll max(ll a, ll b){
  if (a > b)
     return a;
  return b;
}

ll product (ll (&a)[nn], ll n){
   ll min_ind = 0;
   ll max_ind = 0;
   for (int i = 0; i < n; i++){
        if (abs(a[min_ind]) >= abs(a[i]))
           min_ind = i;
        if (abs(a[max_ind]) < abs(a[i]))
           max_ind = i;
   }
   
  std::cout << max_ind << " " << min_ind << std:: endl;

   ll res = 1;
   for (int i = min(min_ind, max_ind); i <= max(min_ind, max_ind); i++){
       res *= a[i];
   }
   return res;    
}

int main (){
  ll a[nn];
  ll n;
  std::cout << "Array length - ";
  std::cin >> n;
  if (n > nn){
    std::cout << "size should be less than 10,000. Enter again" << std::endl;
    return 0;
  }
  std::cout << "Random (1) or enter (2)?";
  int k;
  std::cin >> k;
  if (k == 1) {
    std::cout << "beginning + end";
    ll beg, endd; 
    std::cin >> beg >> endd;
    std::cout << "Random array: ";  
    srand(time(0));
    if (beg < 0 && endd <= 0){
      for (int i = 0; i < n; i++){
        a[i] = rand() % abs(endd) + beg;
        a[i] *= -1;
      std::cout << a[i] << " ";  
     }
    }
    else if (beg < 0 && endd >= 0){
      for (int i = 0; i < n; i++){
        a[i] = rand() % (endd - beg + 1) + endd;
        std::cout << a[i] << " ";  
      }
    }
    else {
      for (int i = 0; i < n; i++){
        a[i] = rand() % (abs (beg - endd)) + beg;
        std::cout << a[i] << " ";  
      }
    }
    std::cout << std::endl;
  }
  else if (k == 2){
    for (int i = 0; i < n; i++)
         std::cin >> a[i];
  }
  else {
    std::cout << "you need to write either random (1) or enter (2)" << std::endl;
    return 0;
  }
  ll ss = sum (a, n);
  std::cout << "summa = " << ss << std::endl;
  ll res = product (a, n);
  std::cout << "product = " << res << std::endl;
  for (int i = 1; i < n; i+=2) {
        for (int j = 1; j < n - i; j+=2) {
            if (a[j] < a[j + 2])
                std::swap(a[j], a[j + 2]);
        }
    }

  for (int i = 0; i < n; i++)
     std::cout << a[i] << " ";
  std::cout << std::endl;
  return 0;
}