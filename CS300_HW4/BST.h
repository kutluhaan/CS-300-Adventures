/**
 * Kutluhan Aygüzel
 * username: kutluhan
 * su-id: 29563
 */

#ifndef CS300_HW4_BST_H
#define CS300_HW4_BST_H

#include <string>
#include <vector>
using namespace std;

//STRUCTURE FOR { {a.txt, 2},{b.txt, 1}, ... }
//DOCUMENT STRUCTURE
template <class Key, class Value>
struct DocumentItem{
    Key documentName;
    Value count;
    DocumentItem(Key documentName = "NO FILE", Value count = 0):
            count(count), documentName(documentName)
    {}

};


//WORD STRUCTURE WITH INFORMATION
template<class Key, class Value>
struct wordItem{
    vector<DocumentItem<Key, Value>> documentInformation;
    Key element;
    wordItem* right;
    wordItem* left;
    int height;
    wordItem(Key element = "", wordItem* right = NULL, wordItem* left = NULL, int height = 0):
            element(element), right(right), left(left), height(height)
    {};
};

template<class Key, class Value>
class BST{
public:

    BST(wordItem<Key, Value>* root = NULL): root(root){
        ITEM_NOT_FOUND = new wordItem<Key, Value> ("NO SUCH ITEM", NULL, NULL);
    }; //CONSTRUCTOR

    BST(const BST & rhs ){
        *this = rhs;
    }; //COPY CONSTRUCTOR

    ~BST(){
        makeEmpty(root);
    }; //DESTRUCTOR

    const wordItem<Key, Value> & findMin() const{
        return findMin(root); //FIND THE MIN NODE OF TREE
    };

    const wordItem<Key, Value> & findMax() const{
        return findMax(root);//FIND THE MAX NODE OF TREE
    };

    wordItem<Key, Value>* find(const Key & x) const{
        return elementAt(find(x, root)); //FINDS NODE USING ELEMENT AT AT FUNCTION
    };

    bool isEmpty() const{ //CHECKS IF TREE IS EMPTY
        return root == NULL;
    };

    wordItem<Key, Value>* & getRoot(){ return root; } //RETURNS THE ROOT

    wordItem<Key, Value>* elementAt(wordItem<Key, Value> *t ) const{
        //IF FOUND RETURN ELEMENT, IF NOT RETURN ITEM_NOT_FOUND
        if (t == NULL)
            return ITEM_NOT_FOUND;
        return t;
    };

    void update(wordItem<Key, Value>* & exists, Key file){
        vector<DocumentItem<Key, Value>> temp = exists->documentInformation;
        bool isIn = false;

        for (int i = 0; i < temp.size(); i++){
            if (exists->documentInformation[i].documentName == file){
                exists->documentInformation[i].count++; //UPDATE
                isIn = true;
            }
        }

        if (!isIn) // IF DOC NOT PRESENT ADD THE DOC
            exists->documentInformation.emplace_back(file, 1);
    } // UPDATE THE ITEM IF IT EXISTS

    void insert(const Key & x, wordItem<Key, Value>* & t, Key fileName, Value count){
        wordItem<Key, Value>* exists = find(x);
        if (exists->element != "NO SUCH ITEM")
            update(exists, fileName); //IF EXISTS
        else {
            if (t == NULL) { //IF NULL NEW NODE INITIALIZATION
                t = new wordItem<Key, Value>(x, NULL, NULL);
                vector<DocumentItem<Key, Value>> info = {DocumentItem<Key, Value>(fileName, count)};
                t->documentInformation = info;
            } else if (x < t->element) {
                // X should be inserted to the left tree!
                insert(x, t->left, fileName, count);

                /*// Check if the left tree is out of balance (left subtree grew in height!)
                if ( height( t->left ) - height( t->right ) == 2 )
                    if ( x < t->left->element ) // X was inserted to the left-left subtree!
                        rotateWithLeftChild(t);

                    else // X was inserted to the left-right subtree!
                        doubleWithLeftChild( t);*/
            } else if (t->element < x) {    // Otherwise X is inserted to the right subtree
                insert(x, t->right, fileName, count);
                /*if ( height( t->right ) - height( t->left ) == 2 )
                    // height of the right subtree increased
                    if ( t->right->element < x )
                        // X was inserted to right-right subtree
                        rotateWithRightChild( t );
                    else // X was inserted to right-left subtree
                        doubleWithRightChild( t );*/
            } else;  // Duplicate; do nothing

            // update the height the node
            t->height = max(height(t->left), height(t->right)) + 1;
        }
    };

    void remove(const Key & x, wordItem<Key, Value> * & t ) const{
        if (t == NULL) { //IF NULL DO NOTHING
            return;
        }

        if (x < t->element) { //GO LEFT
            remove(x, t->left);
        } else if (t->element < x) { //GO RIGHT
            remove(x, t->right);
        } else if (t->left != NULL && t->right != NULL) { //IT IS FOUND AND HAS TWO CHILDREN
            //NODE TO BE DELETED HAS TWO CHILDREN
            wordItem<Key, Value> *temp = findMin(t->right); //FIND MIN FROM RIGHT
            //RE-ASSIGN THE NODES
            t->element = temp->element;
            t->documentInformation = temp->documentInformation;
            remove(t->element, t->right); //REMOVE THE NODE
        } else {
            //NODE TO BE DELETED HAS ONE OR NO CHILDREN
            wordItem<Key, Value> *oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
        }

        if (t != NULL) {
            //UPDATE HEIGHT
            t->height = 1 + max(height(t->left), height(t->right));

            /*//CHECK BALANCE FACTOR
            int balance = height(t->left) - height(t->right);

            //PERFORM ROTATIONS IF NECESSARY BASED ON BALANCE
            //ROTATION CASES
            if (balance > 1 && height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);
            }
            else if (balance < -1 && height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);
            }
            else if (balance > 1 && height(t->left->left) < height(t->left->right)) {
                doubleWithLeftChild(t);
            }
            else if (balance < -1 && height(t->right->right) < height(t->right->left)) {
                doubleWithRightChild(t);
            }*/
        }
    };

    wordItem<Key, Value> * findMin(wordItem<Key, Value> *t ) const{
        if( t == NULL ) //GOES LEFT UNTIL NULL BECAUSE MIN IS AT THE MOST LEFT NODE
            return NULL;
        if( t->left == NULL )
            return t;
        return findMin( t->left );
    };

    wordItem<Key, Value> * findMax(wordItem<Key, Value> *t ) const{
        if( t != NULL ) //GOES RIGHT UNTIL NULL BECAUSE MIN IS AT THE MOST RIGHT NODE
            while( t->right != NULL )
                t = t->right;
        return t;
    };

    wordItem<Key, Value> * find( const Key & x, wordItem<Key, Value> *t ) const{
        //UNTIL FINDS IT GOES RIGHT AND LEFT BASED ON THE X
        while( t != NULL )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return t;    // Match

        return NULL;
    };

    void makeEmpty(wordItem<Key, Value> * & t ) const{
        if( t != NULL ) //UNTIL IT IS NULL, MAKE LEFT AND RIGHT NULL
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = NULL;
    };

    // BST manipulations
    int height(wordItem<Key, Value> *t ) const{
        //RETURNS THE HEIGHT OF THE NODES
        if (t == NULL)
            return -1;
        return t->height;
    };

    int max( int lhs, int rhs ) const{
        //RETURNS WHICH ONE IS BIGGER
        if (lhs > rhs)
            return lhs;
        return rhs;
    };


    bool allLetter(const string& str) { //CHECKS IF ALL LETTERS IN WORD ARE ALPHABETICAL
        for (char c : str) {
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
                return false; // If any character is not a letter, return false
            }
        }
        return true; // All characters are letters
    }

    string toLowercase(const string& word) { //RETURNS THE LOWERED WORD
        string lowercaseWord = word; // Create a copy of the word

        // Convert each alphabetical character to lowercase
        for (char &c : lowercaseWord) {
            // Check if the character is an uppercase letter
            if ((c >= 'A' && c <= 'Z')) {
                // Convert uppercase to lowercase by adding the ASCII offset
                c = c + ('a' - 'A');
            }
        }

        return lowercaseWord;
    }

    int findDocument(wordItem<Key, Value>* room, Key object){ //FINDS THE DOCUMENT AND RETURNS INDEX IF FOUND, -1 OTHERWISE.
        for (int i = 0; i < room->documentInformation.size(); i++)
        {
            if (room->documentInformation[i].documentName == object)
                return i;
        }
        return -1;
    }

    vector<string> queryResult(vector<string> fileNames, vector<Key> queryVector){
        int count = 0; //COUNT FOR FILE NAMES
        vector<string> container;
        for (int i = 0; i < fileNames.size(); i++){
            string filePush = "in Document " + fileNames[i] + ", "; //FIRST PART OF OUTPUT
            bool absent = false;
            for (int j = 0; j < queryVector.size(); j++){
                wordItem<string, int>* result = find(queryVector[j]); //FIND THE WORD
                int index = findDocument(result, fileNames[i]);
                if (index != -1){ //IF FOUND
                    filePush += queryVector[j] + " found " + to_string(result->documentInformation[index].count) + " times, "; //UPDATE THE OUTPUT STRING
                }
                else{ //IF NOT FOUND
                    absent = true;
                    count++; //COUNT FOR ABSENT WORDS
                    break;
                }
            }
            if (!absent) //IF FOUND ADD THE OUTPUT TO OUTPUT CONTAINER VECTOR
                container.push_back(filePush.substr(0, filePush.length() - 2) + ".");
        }
        if (count != fileNames.size()){ //IF SOME FILE QUERY IS PRESENT
            return container;
        }
        else{//IF ANY OF THE FILES DOES NOT CONTAIN THE GIVEN QUERY
            vector <string> oneTime;
            oneTime.push_back("No document contains the given query.");
            return oneTime;
        }
    }

private:
    wordItem<Key, Value>* root;
    wordItem<Key, Value>* ITEM_NOT_FOUND;
};



#endif //CS300_HW4_BST_H
