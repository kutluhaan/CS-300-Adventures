#include <iostream>
#include "AVL.h"
#include <vector>
#include <fstream>
#include <sstream>
//kutluhan ayguzel - 29563
//SU-username: kutluhan
//SU-ID: 29563

int main() {
    //GETTING FILE INPUTS
    //GET NUMBER OF FILES
    int numOfFiles;
    vector<string> fileNames;
    ifstream inputFile;
    cout << "Enter the number of input files: ";
    cin >> numOfFiles;

    //GET FILE NAMES
    for (int i = 0; i < numOfFiles; i++){
        string fileName;
        cout << "Enter " << to_string(i + 1) << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }
    cout <<endl;

    //START INSERTING WORDS FROM FILES
    AVL<string, int> Google;
    for (int i = 0; i < numOfFiles; i++){
        inputFile.open(fileNames[i]); //OPEN FILE
        string line;
        while (getline(inputFile, line)){ //GET A LINE FROM FILE
            istringstream iss(line);
            string word;
            while (iss >> word) { //GET WORDS ONE BY ONE
                if (Google.allLetter(word)){
                    word = Google.toLowercase(word); //LOWER THE WORD
                    wordItem<string, int>* check = Google.find(word); //FIND THE WORD
                    if (check->element == "NO SUCH ITEM") //IF WORD IS NOT IN TREE
                        Google.insert(word, Google.getRoot(), fileNames[i], 1); //INSERT IT
                    else{
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
                            check->documentInformation.push_back(DocumentItem<string, int> (fileNames[i],1));
                    }
                }
            }
        }
        inputFile.close(); //CLOSE FILE
    }
    cin.ignore();

    //START TO QUERY
    while(true){
        //GET THE QUERY
        string queryWord = "";
        cout << "Enter queried words in one line: ";

        getline(cin, queryWord);

        vector<string> queryVector;
        stringstream ss(queryWord);
        string word;
        while (ss >> word) //GET WORDS ONE BY ONE AND LOWER THEM
            queryVector.push_back(Google.toLowercase(word));

        if (queryWord == "ENDOFINPUT"){ break; } //IF ENDOFINPUT BREAK THE LOOP

        else if (queryWord.find("REMOVE") != string::npos){ //IF REMOVE THEN REMOVE THE NODE FROM TREE
            for (int i = 1; i < queryVector.size(); i++){
                wordItem<string, int>* result = Google.find(queryVector[i]);
                if (result->element != "NO SUCH ELEMENT")
                    Google.remove(queryVector[i], Google.getRoot());
            }
            for (int i = 1; i < queryVector.size(); i++)
                cout << queryVector[i] << " ";
            cout << "has been REMOVED" << endl;
        }

        else{ //FIND THE WORDS AND OUTPUT THEIR COUNT WITH FILE NAMES
            int count = 0; //COUNT FOR FILE NAMES
            vector<string> container;
            for (int i = 0; i < fileNames.size(); i++){
                string filePush = "in Document " + fileNames[i] + ", "; //FIRST PART OF OUTPUT
                bool absent = false;
                for (int j = 0; j < queryVector.size(); j++){
                    wordItem<string, int>* result = Google.find(queryVector[j]); //FIND THE WORD
                    int index = Google.findDocument(result, fileNames[i]);
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
                for (int i = 0; i < container.size(); i++)
                    cout << container[i] << endl;
            }
            else //IF ANY OF THE FILES DOES NOT CONTAIN THE GIVNE QUERY
                cout << "No document contains the given query." << endl;
        }
        cout << endl;
    }

    return 0;
}
