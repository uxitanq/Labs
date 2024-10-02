#include <iostream>
#include <cctype>

using namespace std;

int length(char *s){
    int k = 0;
    while (s[k] != '\0'){
        k++;
    }
    return k;
}

int main() {
    char s[301];
    cin.getline(s, 301);
    bool f = false;
    int start = 0, end = 0;
    while (end < length (s)){
        if (isspace(s[end])) {
            if (!f) {
                bool name = true;
                for (int i = start; i < end; i++) {
                    for (int j = i + 1; j < end; j++) 
                        if (s[i] == s[j]) {
                            name = false;
                            break;
                        }
                    if (!name) 
                        break;
                }
                if (name) {
                    f = true;
                    cout << "Word: ";
                    for (int i = start; i < end; i++)
                        cout << s[i];
                }
            }
            start = end + 1;
        }
        end++;
    }
    if (!f) {
        bool name = true;
        for (int i = start; i < end; i++) {
            for (int j = i + 1; j < end; j++) 
                if (s[i] == s[j]) {
                    name = false;
                    break;
                }
            if (!name) 
                break;
        }
        if (name) {
            cout << "Word: ";
            for (int i = start; i < end; i++)
                cout << s[i];
        } else 
            cout << "There are no such words";
    }
    return 0;
}