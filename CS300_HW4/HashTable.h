/**
 * Kutluhan Aygüzel
 * username: kutluhan
 * su-id: 29563
 */
#ifndef CS300_HW4_HASHTABLE_H
#define CS300_HW4_HASHTABLE_H
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
enum LocationType { ACTIVE, EMPTY, DELETED };

template <class Key, class Value>
class HashTable {
public:
    /**
     * constructor
     * initializes an array with given size
     * @param size
     */
    explicit HashTable(int size) : array( nextPrime( size )), wordCount( 0 )
    {
        ITEM_NOT_FOUND = "NOT FOUND";
        makeEmpty();
    }

    /**
     * makes the array full of empty
     */
    void makeEmpty(){
        for (int i = 0; i < array.size(); i++)
            array[i].status = EMPTY;
    }

    /**
     * finds the next prime number after given number
     * @param n
     * @return returns the next prime of given number
     */
    int nextPrime( int n )
    {
        if ( n % 2 == 0 )
            n++;

        for ( ; ! isPrime( n ); n += 2 )
            ;

        return n;
    }

    /**
     * checks whether the given number is a prime number
     * @param n
     * @return if prime true, if not prime false
     */
    bool isPrime( int n )
    {
        if ( n == 2 || n == 3 )
            return true;

        if ( n == 1 || n % 2 == 0 )
            return false;

        for ( int i = 3; i * i <= n; i += 2 )
            if ( n % i == 0 )
                return false;

        return true;
    }

    /**
     * By implementing quadratic approach for hash table
     * it finds a proper position for given Key
     * @param x
     * @return returns the proper location (proper location = EMPTY)
     */
    int findPos( const Key & x )
    {
        int collisionNum = 0;
        int currentPos = hash( x, array.size( ) );

        while ( array[ currentPos ].status != EMPTY &&
                array[ currentPos ].word != x )
        {
            currentPos += pow(++collisionNum, 2) ;  //add the difference
            if ( currentPos >= array.size( ) ) // perform the mod
            {
                currentPos = currentPos % array.size( );  // if necessary
            }
        }
        return currentPos;
    }

    /**
     * it updates the count of the wordItem at the given position
     * by referring to document name
     * @param currentPos
     * @param document
     */
    void update(int currentPos, Key document){
        bool newOne = true;
        for (int i = 0; i < array[ currentPos ].documentInformation.size(); i++)
        {
            if (array[ currentPos ].documentInformation[i].documentName == document){
                array[ currentPos ].documentInformation[i].count++; //update the count if document name is same
                newOne = false;
                break;
            }
        }
        if (newOne){
            array[ currentPos ].documentInformation.push_back(DocumentItem(document, 1)); //if document name is not present then add a new one
        }
    }

    /**
     * checks whether the wordItem at the given position is active
     * @param currentPos
     * @return if active returns true, if not active return false
     */
    bool isActive( int currentPos ) const
    {
        return array[ currentPos ].status == ACTIVE;
    }

    /**
     * make the wordItem DELETED at given index
     * @param x
     */
    void remove( const Key & x )
    {
        int currentPos = findPos( x );
        if ( isActive( currentPos ) )
            array[ currentPos ].status = DELETED;
    }

    /**
     * finds the given Key in the table
     * @param x
     * @return if found returns the word, if not found returns ITEM_NOT_FOUND
     */
    const Key & find( const Key & x ) const
    {
        int currentPos = findPos( x );
        if (isActive( currentPos ))
            return array[ currentPos ].word;

        return ITEM_NOT_FOUND;
    }


    /**
     * inserts a Key with document name into hash table
     * @param x
     * @param document
     */
    void insert( const Key & x, Key document )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if ( isActive( currentPos ) )
        {
            update(currentPos, document); //if there is a word update it
        }
        else{ //insert a new one
            array[ currentPos ] = wordItem( x, ACTIVE );
            array[ currentPos ].documentInformation.push_back(DocumentItem(document, 1));
            wordCount++;
        }

        // enlarge the hash table if necessary
        if ( wordCount >= array.size( ) / 2 )
            rehash( );
    }

    /**
     * hashing function of the hash table
     * @param key
     * @param tableSize
     * @return hash table index
     */
    int hash (const Key & key, int tableSize)
    {
        int hashVal = 0;

        for (int i = 0; i < key.length();i++)
            hashVal = 17 * hashVal + key[ i ];

        hashVal = hashVal % tableSize;

        if (hashVal < 0)
            hashVal = hashVal + tableSize;

        return(hashVal);
    }

    /**
     * rehashes the table by double size and copies the former hash table
     */
    void rehash( )
    {
        vector<wordItem> oldArray = array;

        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for ( int j = 0; j < array.size( ); j++ )
            array[ j ].status = EMPTY;

        // Copy table over
        wordCount = 0;
        for ( int i = 0; i < oldArray.size( ); i++ )
            if ( oldArray[ i ].status == ACTIVE ){
                int currentPos = findPos( oldArray[ i ].word );
                array[ currentPos ] = wordItem( oldArray[ i ].word, ACTIVE );
                array[ currentPos ].documentInformation.clear();
                array[ currentPos ].documentInformation = oldArray[i].documentInformation;

                wordCount++;
            }
    }

    /**
     * checks whether the document is present at the given index of hash table
     * @param fileName
     * @param index
     * @return -1 if not present, count of the word if present
     */
    int documentPresent(string fileName, int index){
        if (index == -1)
            return index;
        for (int i = 0; i < array[ index ].documentInformation.size(); i++){
            if (array[ index ].documentInformation[i].documentName == fileName){ //if present
                return array[ index ].documentInformation[i].count; //return the word count
            }
        }
        return -1;
    }

    /**
     * it is designed for giving the correct output for the file and query words
     * it puts the start string into container vector if all query words are found
     * for the present file name.
     * if a word is not present in a document, it is skipped.
     * if any of the documents contain the given query words, No documents contain the given query is placed into container
     * @param fileNames
     * @param queryVector
     * @return container
     */
    vector<Key> getQueryResult(vector<string> fileNames, vector<Key> queryVector){
        vector <Key> container;
        int fileCount = 0;
        for (int i = 0; i < fileNames.size(); i++){
            string start = "in Document " + fileNames[i] + ", ";
            bool allFound = false;
            int match = 0;
            for (int j = 0; j < queryVector.size(); j++){
                int index = findPos( (queryVector[j]));
                int result = documentPresent(fileNames[i], index);
                if (result != -1) {
                    start += queryVector[j] + " found " + to_string(result) + " times, ";
                    match++;
                    if (match == queryVector.size()){
                        allFound = true;
                        fileCount++;
                    }

                } else {
                    break;
                }
            }
            if (allFound)
                container.push_back(start.substr(0, start.length() - 2) + ".");
        }
        if (fileCount == 0)
        {
            container.push_back("No document contains the given query.");
        }
        return container;
    }

    /**
     * for the load factor calculation this function is needed
     * @return unique word count
     */
    int getUniqueWordCount() { return wordCount; }

    /**
     * for the load factor calculation this function is needed
     * @return size of the hash table
     */
    int getArraySize() { return array.size(); }

private:
    //DOC STRUCTURE WITH DOC INFO
    struct DocumentItem{
        Key documentName;
        Value count;
        explicit DocumentItem(Key documentName = "", Value count = 0):
                count(count), documentName(documentName)
        {};
    };

    //WORD STRUCTURE WITH INFORMATION
    struct wordItem{
        vector<DocumentItem> documentInformation;
        Key word;
        LocationType status;
        explicit wordItem(Key word = "", LocationType status = EMPTY) : word(word), status(status)
        {};
    };

    vector<wordItem> array; // HASH ARRAY
    Key ITEM_NOT_FOUND; // IF NOT FOUND
    int wordCount; // UNIQUE WORD COUNT
};



#endif //CS300_HW4_HASHTABLE_H
