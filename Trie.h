#ifndef TRIE_H
#define TRIE_H
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const int ALPHABET_SIZE = 26;

// узел префиксного дерева
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

// префиксное дерево
class Trie {
private:
    TrieNode* root;
    // рекурсивная функция для поиска всех слов с данным префиксом
    void findWords(TrieNode* node, string currentPrefix, vector<string>& suggestions) {
        if (!node) return;
        // если текущий узел - конец слова, добавляем слово в список
        if (node->isEndOfWord) {
            suggestions.push_back(currentPrefix);
        }
        // рекурсивно проверяем всех детей
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                findWords(node->children[i], currentPrefix + char('a' + i), suggestions);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }
    // вставка слова в Trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }
    // поиск слов по префиксу
    vector<string> getSuggestions(const string& prefix) {
        TrieNode* node = root;
        vector<string> suggestions;
        // находим узел, соответствующий последней букве префикса
        for (char ch : prefix) {
            int index = ch - 'a';
            if (!node->children[index]) {
                return suggestions; // если узел не найден, возвращаем пустой список
            }
            node = node->children[index];
        }
        // найти все слова, начинающиеся с данного префикса
        findWords(node, prefix, suggestions);
        return suggestions;
    }
};

#endif // TRIE_H