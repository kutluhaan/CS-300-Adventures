//
// Created by kutlu on 30.12.2023.
//

#ifndef CS300_HW4_SORT_H
#define CS300_HW4_SORT_H
#include <vector>
using namespace std;

template <class Comparable>
void insertionSort (vector <Comparable> & a)
{
    int j;
    // loop over the passes
    for (int p = 1;  p < a.size(); p++)
    {
        Comparable tmp = a[p];
        // loop over the elements
        for (j = p; j > 0 && tmp.element < a[j - 1].element; j --)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

template <class Comparable>
void shellSort (vector <Comparable> & a)
{
    int j;
    // Loop over increments
    for (int gap = a.size()/2;  gap > 0; gap/=2) {
        // Loop over elements
        for (int i = gap; i < a.size(); i++)
        {
            Comparable tmp = a[i];
            // Loop over elements that are gap apart
            for (j = i; j >= gap &&  tmp < a[j - gap]; j -= gap)
                a[j] = a[j -gap];
            a[j] = tmp;
        }
    }
}

template <class Comparable>
void heapSort(vector<Comparable> & a)
{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() -1; j >0; j--)
    {
        Swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}

inline int leftChild( int i )
{
    return 2*i+1;
}

template <class Comparable>
void percDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child )
    {
        child = leftChild(i);
        if ( child != n-1 && a[ child  ].element < a[ child+1 ].element )
            child++;
        if ( a[child ].element > tmp.element )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}


template <class Comparable>
void mergeSort( vector<Comparable> & a )
{
    vector<Comparable> tmpArray( a.size( ) );

    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

template <class Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right )
{
    if ( left < right )
    {
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );
    }
}

template <class Comparable>
void merge(  vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd )
{
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;
    // Main loop
    while ( leftPos <= leftEnd && rightPos <= rightEnd )
        if ( a[ leftPos ].element <= a[ rightPos ].element )
            tmpArray[ tmpPos++ ] = a[ leftPos++ ];
        else
            tmpArray[ tmpPos++ ] = a[ rightPos++ ];

    while ( leftPos <= leftEnd )    // Copy rest of first half
        tmpArray[ tmpPos++ ] = a[ leftPos++ ];

    while ( rightPos <= rightEnd )  // Copy rest of right half
        tmpArray[ tmpPos++ ] = a[ rightPos++ ];

    // Copy tmpArray back
    for ( int i = 0; i < numElements; i++, rightEnd-- )
        a[ rightEnd ] = tmpArray[ rightEnd ];
}

template <class Comparable>
void quicksort( vector<Comparable> & a, Comparable pivot)
{
    quicksort( a, 0, a.size( ) - 1 , pivot);
}

template <class Comparable>
inline void Swap( Comparable & obj1,
                  Comparable & obj2 )
{
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

template <class Comparable>
const Comparable & median3( vector<Comparable> & a, int left, int right )
{
    int center = ( left + right ) / 2;
    if ( a[ center ].element < a[ left ].element )
        Swap( a[ left ], a[ center ] );
    if ( a[ right ].element < a[ left ].element )
        Swap( a[ left ], a[ right ] );
    if ( a[ right ].element < a[ center ].element )
        Swap( a[ center ], a[ right ] );

    // Place pivot at position right - 1
    Swap( a[ center ], a[ right - 1 ] );
    return a[ right - 1 ];
}

template <class Comparable>
void quicksort( vector<Comparable> & a, int left, int right , Comparable pivot)
{
    if ( left + 10 <= right )
    {
        // Begin partitioning
        int i = left, j = right - 1;
        for ( ; ; )
        {
            while ( a[ ++i ].element < pivot.element ) { }

            while ( pivot.element < a[ --j ].element ) { }

            if ( i < j )
                Swap( a[ i ], a[ j ] );
            else
                break;
        }
        Swap( a[ i ], a[ right - 1 ] );   // Restore pivot

        quicksort( a, left, i - 1, pivot );       // Sort small elements
        quicksort( a, i + 1, right, pivot );    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertionSort( a, left, right );
}

template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
    for ( int p = left + 1; p <= right; p++ )
    {
        Comparable tmp = a[ p ];
        int j;

        for ( j = p; j > left && tmp.element < a[ j - 1 ].element; j-- )
            a[ j ] = a[ j - 1 ];
        a[ j ] = tmp;
    }
}

// the function that i have written for the outputing the given the query by searching it using binary search
template<class Comparable>
void binarySearchQuery(vector<Comparable> & sortedVec, vector<string> fileNames, vector<string> queryVector){
        int count = 0;
        vector<string> container; // the output vector
        for (int i = 0; i < fileNames.size(); i++){
            string start = "in Document " + fileNames[i] + ", ";
            bool absent = false;
            for (int j = 0; j < queryVector.size(); j++){
                int index = binarySearch(sortedVec, queryVector[j]); //search with binary

                if (index == -1){ //not found
                    absent = true;
                    count++; //COUNT FOR ABSENT WORDS
                    break;
                } else {
                    bool docPresent = false;
                    int docIndex = 0;
                    for (int k = 0; k < sortedVec[index].documentInformation.size(); k++){ //updating if it is already in it
                        if (sortedVec[index].documentInformation[k].documentName == fileNames[i]){
                            docIndex = k;
                            docPresent = true;
                            break;
                        }
                    }
                    if (!docPresent){
                        absent = true;
                        count++; //COUNT FOR ABSENT WORDS
                        break;
                    } else {
                        start += queryVector[j] + " found " + to_string(sortedVec[index].documentInformation[docIndex].count) + " times, ";
                    }
                }
            }
            if (!absent){
                container.push_back(start.substr(0, start.length() - 2) + "."); //adding the output string to output vector
            }
        }
    if (count != fileNames.size()){ //IF SOME FILE QUERY IS PRESENT
        for (const auto & i : container){
            cout << i << endl;
        }
    }
    else{//IF ANY OF THE FILES DOES NOT CONTAIN THE GIVEN QUERY
        vector <string> oneTime;
        cout <<"No document contains the given query.";
    }
}

template<class Comparable, class Key>
int binarySearch(const vector<Comparable>& sortedVec, const Key& key) {
    int left = 0;
    int right = sortedVec.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if the middle element is the key
        if (sortedVec[mid].element == key)
            return mid;

        // If key is greater, ignore left half
        if (sortedVec[mid].element < key)
            left = mid + 1;
        // If key is smaller, ignore right half
        else
            right = mid - 1;
    }

    // Key was not found
    return -1;
}

#endif //CS300_HW4_SORT_H
