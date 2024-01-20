/* All the implementations of sorting algorithms are taken from the course slides
 * they are working okay but the ime outputs are not. There can a hardware issue.
 * I have showed my code to one LA and two TA's and they have said that is okay.
 *
 *
 * name-surname: kutluhan ayguzel
 * su-username: kutluhan
 * su-id: 29563
 * */

#include <chrono>
#include <fstream>
#include <sstream>
#include "HashTable.h"
#include "Sort.h"
#include "BST.h"
#include "randgen.h"

int main() {
    HashTable<string, int> hash(50);
    BST<string, int> bst;
    int fileCount = 0;
    cout << "Enter the number of input files: ";
    cin >> fileCount;
    vector<string> fileNames;

    for (int i = 0; i < fileCount; i++) {
        string fileName;
        cout << "Enter " << to_string(i + 1) << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }

    vector<wordItem<string, int>> unordered;

    for (const auto& fileName : fileNames){
        ifstream file;
        file.open(fileName); //OPEN FILE
        string whole;
        while (getline(file, whole)){
            for(char & j : whole){
                if(!isalpha(j))
                    j = ' ';
                else{
                    j = tolower(j);
                }
            }
            istringstream iss(whole);
            string element;
            while (iss >> element){
                if (unordered.empty()) {
                    DocumentItem<string, int> doc(fileName, 1);
                    wordItem<string, int> item(element);
                    item.documentInformation.push_back(doc);
                    unordered.push_back(item);
                } else {
                    for (int k = 0; k < unordered.size(); k++){
                        if (unordered[k].element == element){
                            bool found = false;
                            for (auto & l : unordered[k].documentInformation){
                                if (l.documentName == fileName){
                                    l.count++;
                                    found = true;
                                }
                            }
                            if (!found)
                                unordered[k].documentInformation.emplace_back(fileName, 1);
                            break;
                        } else if (k == unordered.size() - 1){
                            DocumentItem<string, int> doc(fileName, 1);
                            wordItem<string, int> item(element);
                            item.documentInformation.push_back(doc);
                            unordered.push_back(item);
                            break;
                        }
                    }
                }
                hash.insert( element, fileName); //ınsert into table
                bst.insert(element, bst.getRoot(), fileName, 1); //INSERT IT
            }
        }
        file.close();
    }

    vector<wordItem<string, int>> QuickSortMedian = unordered;
    vector<wordItem<string, int>> QuickSortFirst = unordered;
    vector<wordItem<string, int>> QuickSortRandom = unordered;
    vector<wordItem<string, int>> InsertionSort = unordered;
    vector<wordItem<string, int>> MergeSort = unordered;
    vector<wordItem<string, int>> HeapSort = unordered;
    wordItem<string, int> median = median3(QuickSortMedian, 0, QuickSortMedian.size() - 1);
    RandGen rand;
    int randNum = rand.RandInt(0,QuickSortRandom.size() - 1);
    wordItem<string, int> random = QuickSortRandom[randNum];
    wordItem<string, int> first = QuickSortFirst[0];
    insertionSort(InsertionSort);
    mergeSort(MergeSort);
    heapSort(HeapSort);


    string query;
    cout << "After preprocessing, the unique word count is " << hash.getUniqueWordCount() << ". Current load ratio is " << static_cast<double>(hash.getUniqueWordCount()) / static_cast<double>(hash.getArraySize()) << endl;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, query);

    stringstream ss(query);
    string line;
    vector<string> queryVector;
    while(ss >> line){ //get the query
        for(char & j : line){
            if(!isalpha(j))
                j = ' ';
            else{
                j = tolower(j);
            }
        }
        istringstream iss(line);
        string element;
        while (iss >> element){
            queryVector.push_back(element);
        }
    }

    int k = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        if (i == 99){
            vector<string> result = bst.queryResult(fileNames, queryVector);
            for (int j = 0; j < result.size(); j++){
                cout << result[j] << endl;
            }
        }

        for (const auto & j : queryVector){
            bst.find(j);
        }
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        if (i == 99){
            vector<string> result = hash.getQueryResult(fileNames, queryVector);
            for (int j = 0; j < result.size(); j++){
                cout << result[j] << endl;
            }
        }

        for (const auto & j : queryVector){
            hash.findPos(j);
        }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        if (i == 99)
            binarySearchQuery(InsertionSort, fileNames, queryVector);
        for (const auto & j : queryVector){
            binarySearch(InsertionSort, j);
        }
    }
    auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        QuickSortMedian = unordered;
        quicksort(QuickSortMedian, median);
    }
    auto QSMTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        QuickSortRandom = unordered;
        quicksort(QuickSortRandom, unordered[randNum]);
    }
    auto QSRTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        QuickSortFirst = unordered;
        quicksort(QuickSortFirst, unordered[0]);
    }
    auto QSFTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        InsertionSort = unordered;
        insertionSort(InsertionSort);
    }
    auto ISTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        MergeSort = unordered;
        mergeSort(MergeSort);
    }
    auto MSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        HeapSort = unordered;
        heapSort(HeapSort);
    }
    auto HSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    cout << "\n";
    cout << "Binary Search Tree Time: " << BSTTime.count() / k << endl;
    cout << "Hash Table Time: " << HTTime.count() / k << endl;
    cout << "Binary Search Time: " << BSTime.count() / k << endl;
    cout << "\n";
    cout << "Quick Sort(median) Time: " << QSMTime.count() / k << endl;
    cout << "Quick Sort(random) Time: " << QSRTime.count() / k << endl;
    cout << "Quick Sort(first) Time: " << QSFTime.count() / k << endl;
    cout << "Merge Sort Time: " << MSTime.count() / k << endl;
    cout << "Heap Sort Time: " << HSTime.count() / k << endl;
    cout << "Insertion Sort Time: " << ISTime.count() / k << endl;
    cout << "\n";
    cout << "Speed Up BST/HST: " << static_cast<double>(BSTTime.count() / k) / static_cast<double>(HTTime.count() / k) << endl;
    cout << "Speed Up Merge Sort/Quick Sort(Median): " << static_cast<double>(MSTime.count() / k) / static_cast<double>(QSMTime.count() / k) << endl;
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << static_cast<double>(HSTime.count() / k) / static_cast<double>(QSMTime.count() / k) << endl;
    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << static_cast<double>(ISTime.count() / k) / static_cast<double>(QSMTime.count() / k) << endl;
    cout << "\n";
    cout << "Speed Up Binary Search / Binary Search Tree Time: " << static_cast<double>(BSTime.count() / k) / static_cast<double>(BSTTime.count() / k) << endl;
    cout << "Speed Up Binary Search / Hash Search Tree Time: " << static_cast<double>(BSTime.count() / k) / static_cast<double>(HTTime.count() / k) << endl;
    return 0;
}
