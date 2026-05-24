#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
using namespace std;


//   Trie Node

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEnd;

    TrieNode() {
        isEnd = false;
    }
};


//   Trie

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* node = root;

        for(char c : word) {
            if(node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }

            node = node->children[c];
        }

        node->isEnd = true;
    }

    bool search(string word) {
        TrieNode* node = root;

        for(char c : word) {
            if(node->children.find(c) == node->children.end()) {
                return false;
            }

            node = node->children[c];
        }

        return node->isEnd;
    }
};


//   Search Engine

class SearchEngine {
private:

    unordered_map<string, vector<int>> invertedIndex;

    unordered_map<int, string> documents;

    unordered_map<int, unordered_map<string, int>> frequency;

    Trie trie;

public:

    void addDocument(int docID, string text) {

        documents[docID] = text;

        stringstream ss(text);
        string word;

        while(ss >> word) {

            // Convert to lowercase
            for(char &c : word)
                c = tolower(c);

            invertedIndex[word].push_back(docID);

            frequency[docID][word]++;

            trie.insert(word);
        }
    }

    void search(string keyword) {

        for(char &c : keyword)
            c = tolower(c);

        cout << "\nSearching for: " << keyword << endl;

        if(invertedIndex.find(keyword) == invertedIndex.end()) {
            cout << "No results found.\n";
            return;
        }

        vector<pair<int,int>> rankedResults;

        for(int docID : invertedIndex[keyword]) {
            int score = frequency[docID][keyword];
            rankedResults.push_back({score, docID});
        }

        sort(rankedResults.rbegin(), rankedResults.rend());

        cout << "\n===== SEARCH RESULTS =====\n";

        for(auto result : rankedResults) {

            int score = result.first;
            int docID = result.second;

            cout << "Doc ID: " << docID
                << " | Score: " << score << endl;

            cout << documents[docID] << endl;
            cout << "------------------------\n";
        }
    }

    void prefixSearch(string prefix) {

        for(char &c : prefix)
            c = tolower(c);

        if(trie.search(prefix)) {
            cout << "Word exists in Trie\n";
        }
        else {
            cout << "Word not found in Trie\n";
        }
    }
};


int main() {

    SearchEngine engine;

    engine.addDocument(1,
        "C++ is powerful and fast programming language");

    engine.addDocument(2,
        "Search engines use trie and inverted index");

    engine.addDocument(3,
        "C++ search engine project using trie");

    engine.addDocument(4,
        "Trie data structure enables fast searching");

    engine.search("trie");

    engine.prefixSearch("search");

    return 0;
}