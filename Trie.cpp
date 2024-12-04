#include "Trie.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    Trie trie;
    // заранее заданный словарь
    vector<string> dictionary = {"dog", "dot", "dove", "duck", "cat", "car", "cart", "call", "bat", "ball", "banana"};
    // строим Trie из словаря
    for (const string& word : dictionary) {
        trie.insert(word);
    }
    cout << "Система автозаполнения инициализирована." << endl;
    string input;
    while (true) {
        cout << "Введите префикс (или введите «exit», чтобы выйти): ";
        cin >> input;
        if (input == "exit") {
            break;
        }
        // получаем предложения по введенному префиксу
        vector<string> suggestions = trie.getSuggestions(input);
        // выводим результаты
        if (suggestions.empty()) {
            cout << "No suggestions found for prefix: " << input << endl;
        } else {
            cout << "Suggestions: ";
            for (const string& suggestion : suggestions) {
                cout << suggestion << " ";
            }
            cout << endl;
        }
    }

    return 0;
}