#include <iostream>
#include <string>
using namespace std;

bool IsKPeriodic(const string& str, int K) 
{
    //проверка на валидность K
    if (K <= 0 || str.length() % K != 0) {
        return false;
    }
    //считываем подстроку длиной K
    string pattern = str.substr(0, K);
    //проверяем все последующие подстроки длиной K
    for (int i = K; i < str.length(); i += K) {
        if (str.substr(i, K) != pattern) {
            return false;  //если хотя бы одна подстрока не совпадает, возвращаем false
        }
    }
    return true;  //если все подстроки совпадают
}

int main() 
{
    cout << "Enter string: " << endl;
    string str;
    cin >> str;
    //string str = "abcabcabcabc";
    int K = 3;
    if (IsKPeriodic(str, K)) {
        cout << "The string is " << K << "-periodic." << endl;
    } else {
        cout << "The string is not " << K << "-periodic." << endl;
    }

    return 0;
}