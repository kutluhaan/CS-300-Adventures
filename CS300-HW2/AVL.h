//
// Created by kutlu on 12.11.2023.
//

//kutluhan ayguzel - 29563
//SU-username: kutluhan
//SU-ID: 29563

#ifndef CS300_HW2_AVL_H
#define CS300_HW2_AVL_H

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


//WORD STRUCTURE WITH INFORMATIONS
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
class AVL{
public:

    AVL(wordItem<Key, Value>* root = NULL): root(root){
        ITEM_NOT_FOUND = new wordItem<Key, Value> ("NO SUCH ITEM", NULL, NULL);
    }; //CONSTRUCTOR

    AVL(const AVL & rhs ){
        *this = rhs;
    }; //COPY CONSTRUCTOR

    ~AVL(){
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

    void insert(const Key & x, wordItem<Key, Value>* & t, Key fileName, Value count){
        if (t == NULL ){ //IF NULL NEW NODE INITIALIZATION
            t = new wordItem<Key, Value>(x, NULL, NULL);
            vector<DocumentItem<Key, Value>> info = {DocumentItem<Key, Value> (fileName, count)};
            t->documentInformation = info;
        }

        else if ( x < t->element ) {
            // X should be inserted to the left tree!
            insert( x, t->left, fileName, count);

            // Check if the left tree is out of balance (left subtree grew in height!)
            if ( height( t->left ) - height( t->right ) == 2 )
                if ( x < t->left->element ) // X was inserted to the left-left subtree!
                    rotateWithLeftChild(t);

                else // X was inserted to the left-right subtree!
                    doubleWithLeftChild( t);
        }
        else if( t->element < x )
        {    // Otherwise X is inserted to the right subtree
            insert( x, t->right, fileName, count);
            if ( height( t->right ) - height( t->left ) == 2 )
                // height of the right subtree increased
                if ( t->right->element < x )
                    // X was inserted to right-right subtree
                    rotateWithRightChild( t );
                else // X was inserted to right-left subtree
                    doubleWithRightChild( t );
        }
        else
            ;  // Duplicate; do nothing

        // update the height the node
        t->height = max( height( t->left ), height( t->right ) ) + 1;
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

            //CHECK BALANCE FACTOR
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
            }
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

    // AVL manipulations
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

    void rotateWithLeftChild(wordItem<Key, Value> * & k2 ) const{
        wordItem<Key, Value> *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;

    };

    void rotateWithRightChild(wordItem<Key, Value> * & k1 ) const{
        wordItem<Key, Value> *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;

    };

    void doubleWithLeftChild(wordItem<Key, Value> * & k3 ) const{
        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );
    };

    void doubleWithRightChild(wordItem<Key, Value> * & k1 ) const{
        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );
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

private:
    wordItem<Key, Value>* root;
    wordItem<Key, Value>* ITEM_NOT_FOUND;
};


#endif //CS300_HW2_AVL_H