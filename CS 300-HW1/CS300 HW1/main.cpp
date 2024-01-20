#include <iostream>
#include "LinkedStack.h"
//name-surname: kutluhan aygüzel
//ID: 00029563
using namespace std;

int main() {
    //GETTING INPUTS OF MAZE
    int numberOfMazeK, rowsOfMazeM, colsOfMazeN;
    cout << "Enter the number of mazes: ";
    cin >> numberOfMazeK; cout << endl;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rowsOfMazeM >> colsOfMazeN;
    cout << endl;

    LinkedStack<int> stack(rowsOfMazeM, colsOfMazeN); //START STACK AND MAZE
    int mazeNumber = 1; //CONTROL THE AMOUNT OF MAZES PRODUCED BY PROGRAM
    //PRODUCING RANDOM MAZE(S) USING STACK
    for (int i = 0; i < numberOfMazeK; i++){
        while (!stack.isFinished()){ //IF MAZE PRODUCING IS NOT FINISHED
            if (!stack.isDeadEndStack()){ // AND IF IT IS NOT A DEAD END FOR TOP OF STACK
                string way = stack.choseWallStack(); //CHOSE A WAY TO STEP FORWARD
                stack.pushStack(way); //AND PUT THE STEP CELL TO THE STACK
            }
            else{ //IF IT IS A DEAD END FOR TOP OF STACK
                while (stack.shouldStackGoBack()) //THEN DETERMINE WHETHER TOP OF STACK SHOULD POP
                    stack.popStack(); //AND POP THE CELL OUT OF THE STACK
            }
        }
        stack.mazeFileWriter("maze_"+ to_string(mazeNumber) + ".txt"); //IF MAZE PRODUCING IS FINISHED, THEN WRITE THE MAZE FILE
        mazeNumber++; //WHEN A MAZE FINISHED, WE WILL CREATE THE OTHER ONE AND THE AMOUNT OF MAZES PRODUCED MUST BE INCREMENTED
        stack.restart(); //EVERYTHING SHOULD START AGAIN
    }

    int mazeID, enterX, enterY, exitX, exitY; //ENTRY AND EXIT POINTS FOR RAT
    cout << "All mazes are generated." << endl << endl;
    cout << "Enter a maze ID between 1 to " + to_string(numberOfMazeK) + " inclusive to find a path: ";
    cin >> mazeID;
    cout << endl;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> enterX >> enterY;
    cout << endl;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;
    cout << endl;

    stack.putRateToMaze("maze_"+ to_string(mazeID) + ".txt", enterX, enterY); //PUT THE RAT TO THE STARTING POINT IN THE MAZE
    while (!stack.ratArrived(exitX, exitY)){ //IF RAT IS COULD NOT FIND THE EXIT POINT
        if (!stack.isDeadEndForRat()){ //AND IF THE CELL WHICH RAT IS IN IS NOT A DEAD END
            string way = stack.choseWayRat(); //THEN CHOSE A WAY TO STEP FORWARD
            if (way != "NO WAY") //CHECK THE WAY AGAIN
                stack.ratSteps(way); //THEN RAT STEPS FORWARD
        }
        else{ //IF IT IS A DEAD END FOR RAT
            while (stack.isDeadEndForRat()) //DETERMINE WHETHER IT IS STILL DEAD END FOR RAT
                stack.popRatPath(); //POP THE RAT PATH UNTIL THE CELL WHICH RAT IN IS NOT A DEAD END
        }
    }
    stack.pathWriter(mazeID, enterX, enterY,exitX, exitY); //IF RAT MADE TO ITS DESTINATION THEN WRITE THE PATH COORDINATES
    return 0;
}
