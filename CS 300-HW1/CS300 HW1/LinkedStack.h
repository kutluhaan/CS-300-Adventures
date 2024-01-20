//
// Created by kutlu on 25.10.2023.
//
//name-surname: kutluhan aygüzel
//ID: 00029563

#ifndef CS300_HW1_LINKEDSTACK_H
#define CS300_HW1_LINKEDSTACK_H
#include <string>
#include <fstream>
#include "randgen.h"
using namespace std;
template<class Object>
struct cell{
    Object x, y;                    //x and y coordinates of a cell
    cell *up, *right, *down, *left; //initialize cell pointers for 2d double linked list maze. the neighbours of a cell.
    string Right, Left, Up, Down; //initialize strings for checking wall
    bool visited, ratCame;         //is this cell visited when creating the maze and did rat pass by here?

    //INITIALIZATION OF CELL OBJECTS
    cell(Object x, Object y, bool visited = false, bool ratCame = false,
         cell* up = NULL, cell* right = NULL, cell* down = NULL, cell* left = NULL,
         string Up = "wall", string Right = "wall", string Down = "wall", string Left = "wall")
            :
            x(x), y(y), visited(visited), ratCame(ratCame),
            right(right), left(left), up(up), down(down),
            Up(Up), Right(Right), Down(Down), Left(Left)
            {};
};

template <class Object>
struct path{
    cell<Object>* element; //a cell object will be hold by stack
    path* next; //a pointer for adding elements to stacks

    //INITIALIZATION
    path(cell<Object>* theElement, path* n = NULL )
            :next(n), element(theElement)
            {};
};


template <class Object>
class LinkedStack {
public:
    //CONSTRUCTOR AND DESTRUCTOR
    LinkedStack(int row, int col) {
        maxX = col;
        maxY = row;
        cellCount = 1;
        mazeHead = mazeBuilder(); //build maze
        topOfStack = new path<Object>(mazeHead, NULL); //assign the head of it
        rat = NULL; //do not start rat yet
    }
    ~LinkedStack() { //delete the maze and empyt the stack and delete the rat
        delete rat; //rat will be NULL in file writing method of path
        rat = NULL;
        deleteMaze();
        makeEmpty();
    }


    //FUNCTIONS FOR BUILDING MAZE
    void goUp(cell<Object>* & pointer){ //an object will be assigned to up neighbour
        pointer = pointer->up;
    }
    void goDown(cell<Object>* & pointer){ //an object will be assigned to down neighbour
        pointer = pointer->down;
    }
    void goRight(cell<Object>* & pointer){ //an object will be assigned to right neighbour
        pointer = pointer->right;
    }
    void goLeft(cell<Object>* & pointer){ //an object will be assigned to left neighbour
        pointer = pointer->left;
    }
    cell<Object>* buildRow(cell<Object>* pointer, int row, int col){ //it builds row and returns the first column of a row
        if (col == 0){ //checks the col number bc it is important for a row at the end we produce just one row
            for (int i = 0; i < row; i++){ //build a vertical row
                pointer->up = new cell<Object> (col, i + 1, false, NULL, NULL, NULL, NULL);
                pointer->up->down = pointer;
                goUp(pointer);
            }
            for (int i = 0; i < row; i++)
                goDown(pointer);

        }else{ //build a horizontal row
            for (int i = 0; i < col; i++){
                pointer->right = new cell<Object> (i + 1, row, false, NULL, NULL, NULL, NULL);
                pointer->right->left = pointer;
                goRight(pointer);
            }
            for (int i = 0; i < col; i++)
                goLeft(pointer);
        }
        return pointer;
    }
    cell<Object>* connectRows(cell<Object>* top, cell<Object>* below){  //it connects row for up and down pointers of the cell
                                                                        // and returns the top row's first column pointer
        for (int i = 0; i < maxX; i++){
            top->down = below;
            below->up = top;
            if (top->right != NULL && below->right != NULL){
                top = top->right;
                below = below->right;
            }
        }
        for (int i = 0; i < maxX - 1; i++)
            top = top->left;
        return top;
    }
    cell<Object>* mazeBuilder(){ //builds maze
        mazeHead = new cell<Object> (0, 0, true, NULL, NULL, NULL, NULL);
        if (maxY == 1){ //if just one row
            mazeHead = buildRow(mazeHead,  0, maxX- 1);
        }
        else if (maxX == 1){ //if just one column
            mazeHead = buildRow(mazeHead,  maxY - 1, 0);
        }
        else{ //other condition
            cell<Object>* temp = mazeHead;
            temp = buildRow(temp, 0, maxX - 1);
            for (int i = 0; i < maxY- 1; i++){
                cell<Object>* follower = temp;
                temp = new cell<Object> (0, i + 1);
                temp = buildRow(temp, i + 1, maxX- 1);
                temp = connectRows(temp, follower);
                follower = follower->up;
            }
        }
        return mazeHead; //it returns the mazeHead to mazeHead
    }

    //DESTRUCTORS
    void makeEmpty() { //empty the stack and delete topOfStack
        while (topOfStack != NULL) {
            path<Object>* temp = topOfStack;
            topOfStack = topOfStack->next;
            delete temp;
            temp = NULL;
        }
        delete topOfStack;
        topOfStack = NULL;
    }
    void deleteMaze(){ //deletes the maze and deletes the mazeHead
        if (mazeHead != NULL){
            cell <Object>* iter = mazeHead;
            while(iter->up != NULL)
                goUp(iter);

            for (int i = 0; i < maxY; i++){
                while(iter->right != NULL)
                    goRight(iter);
                for (int j = 0; j < maxX; j++){
                    if (iter->up != NULL)
                        iter->up = NULL;
                    if (iter->right != NULL)
                        iter->right = NULL;
                    if (iter->down != NULL){
                        if (iter->left == NULL){
                            iter = iter->down;
                            iter->up->down = NULL;
                            delete iter->up;
                            iter->up = NULL;
                        }else
                            iter->down = NULL;
                    }

                    if (iter->left != NULL){
                        iter = iter->left;
                        iter->right->left = NULL;
                        delete iter->right;
                        iter->right = NULL;
                    }
                }
            }
            delete mazeHead;
            mazeHead = NULL;
        }
    }
    void restart() { //restarts the maze for new maze creations
        makeEmpty();
        deleteMaze();
        mazeHead = mazeBuilder();
        topOfStack = new path<Object>(mazeHead, NULL);
        cellCount = 1;
    }


    //CONTROL OF STACK
    bool isDeadEndStack() { //checks the dead end situations for topOfStack
        cell<Object>* Cell = topOfStack->element;
        //BORDER CONDITIONS
        //right up corner
        if (Cell->up == NULL &&
            Cell->right == NULL &&
            (Cell->down != NULL && Cell->down->visited == true) &&
            (Cell->left != NULL && Cell->left->visited == true))
            return true;
        //left up corner
        else if (Cell->up == NULL &&
                Cell->left == NULL &&
                (Cell->down != NULL && Cell->down->visited == true) &&
                (Cell->right != NULL && Cell->right->visited == true))
            return true;
        //bottom left corner
        else if(Cell->down == NULL &&
                Cell->left == NULL &&
                (Cell->up != NULL && Cell->up->visited == true) &&
                (Cell->right != NULL && Cell->right->visited == true))
            return true;
        //bottom right corner
        else if(Cell->down == NULL &&
                Cell->right == NULL &&
                (Cell->up != NULL && Cell->up->visited == true) &&
                (Cell->left != NULL && Cell->left->visited == true))
            return true;

        //SIDE CONDITIONS
        //top side condition
        else if (Cell->up == NULL &&
                (Cell->right != NULL && Cell->right->visited == true) &&
                (Cell->down != NULL && Cell->down->visited == true) &&
                (Cell->left != NULL && Cell->left->visited == true))
            return true;
        //right side condition
        else if (Cell->right == NULL &&
                (Cell->up != NULL && Cell->up->visited == true)  &&
                (Cell->down != NULL && Cell->down->visited == true) &&
                (Cell->left != NULL && Cell->left->visited == true))
            return true;
        //bottom side condition
        else if (Cell->down == NULL &&
                (Cell->up != NULL && Cell->up->visited == true) &&
                (Cell->right != NULL && Cell->right->visited == true) &&
                (Cell->left != NULL && Cell->left->visited == true))
            return true;
        //left side condition
        else if (Cell->left == NULL &&
                (Cell->up != NULL && Cell->up->visited == true) &&
                (Cell->down != NULL && Cell->down->visited == true) &&
                (Cell->right != NULL && Cell->right->visited == true))
            return true;

        //CENTER CONDITION
        else if ((Cell->left != NULL && Cell->left->visited == true) &&
                (Cell->up != NULL && Cell->up->visited == true) &&
                (Cell->down != NULL && Cell->down->visited == true) &&
                (Cell->right != NULL && Cell->right->visited == true))
            return true;

        //NOT DEAD END
        else
            return false;
    }
    bool isEmpty(path<Object>* element){ //checks whether the givne object is NULL
        return element == NULL;
    }
    bool isFinished(){ //is all the maze created
        return cellCount == (maxX * maxY);
    }
    bool shouldStackGoBack() { //checks if the current cell is proper to move for a new cell
        return
                ((topOfStack->element->up == NULL || topOfStack->element->up->visited == true) &&
                (topOfStack->element->right == NULL || topOfStack->element->right->visited == true) &&
                (topOfStack->element->down == NULL || topOfStack->element->down->visited == true) &&
                (topOfStack->element->left == NULL || topOfStack->element->left->visited == true));
    }

    //ADD AND REMOVE CELL
    void popStack(){ //remove an element from stack
        if (!isEmpty(topOfStack)){
            path<Object>* temp = topOfStack->next;
            topOfStack = NULL;
            topOfStack = temp;
        }
    }
    cell<Object>* getCell(string way, path<Object>* subject){ //get an element in a given direction of the given cell
        if (way == "UP")
            return subject->element->up;
        else if (way == "RIGHT")
            return subject->element->right;
        else if(way == "DOWN")
            return subject->element->down;
        else
            return subject->element->left;
    }
    void visit(string way){ //based on the way, it visits the cell
        if (way == "UP")
            topOfStack->element->up->visited = true;
        else if (way == "RIGHT")
            topOfStack->element->right->visited = true;
        else if(way == "DOWN")
            topOfStack->element->down->visited = true;
        else
            topOfStack->element->left->visited = true;
    }
    void knockDown(string way){ //based on the way, it knock downs wall in the given direction
        if (way == "UP"){
            topOfStack->element->Up = "no wall";
            topOfStack->element->up->Down = "no wall";
        }

        else if (way == "RIGHT"){
            topOfStack->element->Right = "no wall";
            topOfStack->element->right->Left = "no wall";
        }

        else if(way == "DOWN"){
            topOfStack->element->Down = "no wall";
            topOfStack->element->down->Up = "no wall";
        }

        else{
            topOfStack->element->Left = "no wall";
            topOfStack->element->left->Right = "no wall";
        }
    }
    void pushStack(string way){ //puts an element to the stack
        visit(way);
        knockDown(way);
        cellCount++;
        path<Object>* temp = topOfStack;
        cell<Object>* newCell = getCell(way, topOfStack); //gets a cell in the given direction
        path<Object>* newPath = new path<Object> (newCell, temp);
        topOfStack = newPath;
    }

    //WHERE TO GO
    string choseWallStack(){ //choosing a way for the topOfStack
        string availableWays = "0123";

        if (topOfStack->element->up == NULL || topOfStack->element->up->visited == true)
            availableWays = availableWays.substr(1);
        if (topOfStack->element->right == NULL || topOfStack->element->right->visited == true)
            availableWays = availableWays.substr(0, availableWays.find('1')) + availableWays.substr(availableWays.find('1') + 1);
        if (topOfStack->element->down == NULL || topOfStack->element->down->visited == true)
            availableWays = availableWays.substr(0, availableWays.find('2')) + availableWays.substr(availableWays.find('2') + 1);
        if (topOfStack->element->left == NULL || topOfStack->element->left->visited == true)
            availableWays = availableWays.substr(0, availableWays.find('3')) + availableWays.substr(availableWays.find('3') + 1);

        RandGen random; //random generator
        int wall = random.RandInt(0, availableWays.length() - 1);  //chooses a random wall for current cell
                                                                            // and from availableWays
        if (availableWays[wall] == '0')
            return "UP";

        else if (availableWays[wall] == '1')
            return "RIGHT";

        else if (availableWays[wall] == '2')
            return "DOWN";

        else
            return "LEFT";
    }

    //FILE WRITING
    void mazeFileWriter(string fileName){
        ofstream outputFile;
        outputFile.open(fileName);
        if (outputFile.is_open()){
            cell<Object>* iterator = mazeHead;
            outputFile << to_string(maxY) + " " + to_string(maxX) + "\n"; //writes the size of maze
            for (int j = 0; j <= maxY - 1; j++){
                for (int i = 0; i <= maxX - 1; i++){
                    //takes the elements of the iterator
                    string x = to_string(iterator->x), y = to_string(iterator->y), up = iterator->Up, right = iterator->Right, down = iterator->Down, left = iterator->Left;

                    if (up == "wall")
                        up = '1';
                    else
                        up = '0';

                    if (right == "wall")
                        right = '1';
                    else
                        right = '0';

                    if (down == "wall")
                        down = '1';
                    else
                        down = '0';

                    if (left == "wall")
                        left = '1';
                    else
                        left = '0';

                    outputFile <<"x=" + x + " y=" + y + " l=" + left + " r=" + right + " u=" + up + " d=" + down + "\n"; //puts the elements into txt file
                    if (iterator->right != NULL)
                        goRight(iterator);
                }
                for (int i = 0; i <= maxX - 1; i++){ //go beginning of the row
                    if (iterator->left != NULL)
                        goLeft(iterator);
                }

                if (iterator->up != NULL) //and a step up of iterator
                    goUp(iterator);
            }

        }
    }

    //PATH FINDER FOR RAT
    //FILE WRITING
    void pathWriter(int mazeID, Object enterX, Object enterY, Object exitX, Object exitY){
        string fileName = "maze_" + to_string(mazeID) + "_path_" + to_string(enterX) + "_" + to_string(enterY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
        ofstream outputFile;
        outputFile.open(fileName);
        //REVERSING THE RAT STACK
        path<Object>* real = NULL;
        while (rat != NULL){
            if (real == NULL){
                real = new path<Object> (rat->element, NULL);
            }else{
                path<Object>* temp = new path<Object> (rat->element, real);
                real = temp;
            }
            popRatPath(); //it makes the rat NULL
        }
        //END OF REVERSING THE RAT STACK

        //WRITING THE REAL RAT STACK TO THE FILE WITH GIVEN FILE NAME
        if (outputFile.is_open()){
            while (real != NULL){
                outputFile << to_string(real->element->x) + " " + to_string(real->element->y) + "\n";
                real = real->next;
            }
        }
    }

    //PREPARE MAZE FOR RAT
    void popRatPath(){ //rat steps back
        //removes a rat path from rat stack
        if (!isEmpty(rat)){
            path<Object>* temp = rat->next;
            delete rat;
            rat = NULL;
            rat = temp;
        }
    }
    void cellModifier(string line, cell<Object>* & modify){ //reads the line and determines the important values for rat and modifies the given cell
        //takes the important values from line
        Object x = stoi(line.substr(line.find("x=") + 2, line.find(" ")));
        line = line.substr(line.find(" ") + 1);
        Object y = stoi(line.substr(line.find("y=") + 2, line.find(" ")));
        line = line.substr(line.find(" ") + 1);
        string left = line.substr(line.find("l="), line.find(" "));
        left = left.substr(2);
        line = line.substr(line.find(" ") + 1);
        string right = line.substr(line.find("r="), line.find(" "));
        right = right.substr(2);
        line = line.substr(line.find(" ") + 1);
        string up = line.substr(line.find("u="), line.find(" "));
        up = up.substr(2);
        line = line.substr(line.find(" ") + 1);
        string down = line.substr(line.find("d="), line.find(" "));
        down = down.substr(2);
        modify->x = x;
        modify->y = y;

        //modifying the given cell
        if (up == "1")
            modify->Up = "wall";
        else
            modify->Up = "no wall";

        if (right == "1")
            modify->Right = "wall";
        else
            modify->Right = "no wall";

        if (down == "1")
            modify->Down = "wall";
        else
            modify->Down = "no wall";

        if (left == "1")
            modify->Left = "wall";
        else
            modify->Left = "no wall";
    }
    void txtToMaze(string mazeName){ //reads the maze from a given input file
        ifstream file(mazeName);
        string line;
        int first = 0, row = 1, col = 1;
        mazeHead = mazeBuilder(); //calls it again reinitialize the maze
        cell<Object>* iter = mazeHead;
        while(getline(file, line)){ //takes the lines from txt file
            if (first == 0){ //first line contains the size of maze
                first++;
                continue;
            }else{
                cellModifier(line, iter); //modifies the cell
                if (iter->right != NULL) //step for next cell
                    goRight(iter);
                col++; //increment the number of cells processed in row
                if (col == maxX){ //checks the end of row by checking column number
                    getline(file, line);
                    cellModifier(line, iter);
                    while(iter->left != NULL) //go to beginning
                        goLeft(iter);
                    if (iter->up != NULL) //and go up for new row
                        goUp(iter);
                    col = 1;
                    row++; //increment row
                }
            }
        }
    }
    void putRateToMaze(string mazeName, Object enterX, Object enterY) {
        txtToMaze(mazeName); //initializes the maze by reading maze from file
        cell<Object>* initial = NULL; //temporary starting point iterator
        if (enterY == 0 && enterX == 0) {
            initial = mazeHead;
            initial->ratCame = true;

        } else if (enterY == 0 && enterX != 0) {
            cell<Object> *temp = mazeHead;
            for (int i = 0; i < enterX ; i++)
                goRight(temp);
            initial = temp;
            initial->ratCame = true;

        } else if (enterY != 0 && enterX == 0) {
            cell<Object> *temp = mazeHead;
            for (int i = 0; i < enterY ; i++)
                goUp(temp);
            initial = temp;
            initial->ratCame = true;

        } else if (enterX != 0 && enterY != 0){
            cell<Object> *temp = mazeHead;
            for(int i = 0; i < enterX; i++)
                goRight(temp);
            for (int i = 0; i < enterY; i++)
                goUp(temp);
            initial = temp;
            initial->ratCame = true;
        }
        rat = new path<Object>(initial, NULL); //assign the rat to starting point
    }

    //RAT MOVES
    bool isDeadEndForRat() {
        //checks if the rat's current position is dead end or not
        //checks the neighbours condition on whether rat been there or not and the walls of the position of rat
        int available = 0; //number of available ways
        if (rat->element->Up != "wall" && rat->element->up->ratCame != true)
            available++;
        if (rat->element->Right != "wall" && rat->element->right->ratCame != true)
            available++;
        if(rat->element->Down != "wall" && rat->element->down->ratCame != true)
            available++;
        if (rat->element->Left != "wall" && rat->element->left->ratCame != true)
            available++;

        return available == 0; //if not equal to 0, then NOT DEAD END

    }
    bool ratArrived(Object exitX,Object exitY){ //is rat on the exit point
        return rat->element->x == exitY && rat->element->y == exitX;
    }
    void ratSteps(string way){ //push method for the rat same functionality with topOfStack push method
        path<Object>* temp = rat;
        cell<Object>* newCell = getCell(way, rat);
        path<Object>* newPath = new path<Object> (newCell, temp);
        rat = newPath;
        rat->element->ratCame = true;
    }
    string choseWayRat(){//for the current position of the rat checks the available ways and returns it
                         //same functonality with choseWayStack()
        string availableWays = "0123";
        //checks the available ways for current poisiton of the rat
        if (rat->element->up == NULL  || rat->element->Up == "wall" || rat->element->up->ratCame == true)
            availableWays = availableWays.substr(1);
        if (rat->element->right == NULL  || rat->element->Right == "wall" || rat->element->right->ratCame == true)
            availableWays = availableWays.substr(0, availableWays.find('1')) + availableWays.substr(availableWays.find('1') + 1);
        if (rat->element->down == NULL  || rat->element->Down == "wall" || rat->element->down->ratCame == true)
            availableWays = availableWays.substr(0, availableWays.find('2')) + availableWays.substr(availableWays.find('2') + 1);
        if (rat->element->left == NULL  || rat->element->Left == "wall" || rat->element->left->ratCame == true)
            availableWays = availableWays.substr(0, availableWays.find('3')) + availableWays.substr(availableWays.find('3') + 1);

        RandGen random;
        int wall = random.RandInt(0, availableWays.length() - 1);

        if (availableWays[wall] == '0')
            return "UP";
        else if (availableWays[wall] == '1')
            return "RIGHT";
        else if (availableWays[wall] == '2')
            return "DOWN";
        else if (availableWays[wall] == '3')
            return "LEFT";
        else
            return "NO WAY";
    }

private:
    path<Object>* topOfStack; //last element in stack
    path<Object>* rat; //rat for finding the path for given entry and exit
    cell<Object>* mazeHead; //left bottom point of maze (0,0)
    int cellCount; //count the number of cells that is visited by random maze generator stack (topOfStack)
    //sizes of the maze
    int maxX;
    int maxY;
};
#endif //CS300_HW1_LINKEDSTACK_H