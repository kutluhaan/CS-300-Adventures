/**
 * Kutluhan Aygüzel
 * username: kutluhan
 * su-id: 29563
 */

#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "HashTable.h"
#include "BST.h"
using namespace std;

void findTable(vector<string> query, HashTable<string, int> & table){//finds the queries in table
    for (int i = 0; i < query.size(); i++){
        table.findPos(query[i]);
    }
}

void findTree(vector<string> query, BST<string, int> & tree) //finds the queries in tree
{
    for (int i = 0; i < query.size(); i++){
        tree.find(query[i]);
    }
}

string removeSpaces(const string& input) {
    string result;
    for (char ch : input) {
        if (ch != ' ') {
            result += ch;
        }
    }
    return result;
}

void formatter(string & line){ //replaces non-alphabetic characters with ' '
    for(char & i : line){
        if(!isalpha(i))
            i = ' ';
        else{
            i = tolower(i);
        }
    }
}

int main() {
    HashTable<string, int> table(50);
    BST<string, int> tree;
    vector<string> fileNames;
    int fileCount = 0;
    cout << "Enter the number of input files: ";
    cin >> fileCount;

    //GET FILE NAMES
    for (int i = 0; i < fileCount; i++){
        string fileName;
        cout << "Enter " << to_string(i + 1) << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }

    ifstream fileInput;
    for (int i = 0; i < fileCount; i++){
        fileInput.open(fileNames[i]); //OPEN FILE
        string line;
        while (getline(fileInput, line)){
            formatter(line);
            istringstream iss(line);
            string word;
            while (iss >> word){
                removeSpaces(word);
                table.insert( word, fileNames[i]); //ınsert into table

                wordItem<string, int>* check = tree.find(word);
                if (check->element == "NO SUCH ITEM"){//IF WORD IS NOT IN TREE
                    tree.insert(word, tree.getRoot(), fileNames[i], 1); //INSERT IT
                } else {
                    //IF WORD IS IN TREE
                    vector<DocumentItem<string, int>> temp = check->documentInformation; //GET THE FILE INFO
                    bool found = false;
                    //IF FILE IS PRESENT FOR WORD
                    for (int j = 0; j < temp.size(); j++){
                        if (check->documentInformation[j].documentName == fileNames[i]){
                            check->documentInformation[j].count++; //INCREMENT THE COUNT OF WORD FOR DOCUMENT
                            found = true;
                        }
                    }
                    //IF FILE IS NOT PRESENT ADD FILE TO NODE WITH COUNT 1
                    if (!found)
                        check->documentInformation.emplace_back(fileNames[i],1);
                }
            }
        }
        fileInput.close();
    }

    string query;
    cout << "\nAfter preprocessing, the unique word count is " << table.getInserted() << ". Current load ratio is " << static_cast<double>(table.getInserted()) / static_cast<double>(table.getSize()) << endl;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, query);
    stringstream ss(query);
    string line;
    vector<string> queryVector;
    while(ss >> line){ //get the query
        formatter(line);
        istringstream iss(line);
        string word;
        while (iss >> word){
            removeSpaces(word);
            queryVector.push_back(word);
        }
    }
    vector<string> result = tree.queryResult(fileNames, queryVector);
    for (int j = 0; j < result.size(); j++) {
        cout << result[ j ] << endl;
    }

    result = table.getQueryResult(fileNames, queryVector);
    for (int j = 0; j < result.size(); j++){
        cout << result[ j ] << endl;
    }

    //output the time
    int k = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        findTree(queryVector, tree);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        findTable(queryVector, table);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << BSTTime.count() / k << "\n";
    cout << "\nTime: " << HTTime.count() / k << "\n";
    cout << "Speed Up: " << static_cast<double>(BSTTime.count() / k) / static_cast<double>(HTTime.count() / k) << endl;
    return 0;
}
