#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct Line {
    string line;
    int lineNum;
};

void printLines(vector<Line> lines, bool printLineNumber, bool printOccurenceCount, string search){
    for (int i = 0; i < lines.size(); i++){
        if (printLineNumber){
            cout << lines[i].lineNum << ":\t";
        }
        cout << lines[i].line << '\n';
    }
    if (printOccurenceCount){
        cout << "\nOccurences of lines containing \"" << search << "\": " << lines.size();
    }
}

void noArgs(){
    string str, search;
    cout << "Give a string from which to search for: ";
    getline(cin, str);
    cout << "Give search string: ";
    getline(cin, search);
    
    size_t found = str.find(search);
    if (found!=string::npos){
        cout << "\"" << search << "\" found in \"" << str << "\" in postion " << found;
    }
    else {
        cout << "\"" << search << "\" NOT found in \"" << str << "\"";
    }
    return;
}

void searchStrFromFile(string search, string fileName){
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open()){
        vector<Line> linesFound;
        string line;
        int lineCount = 1;
        Line lineInfo;
        while (getline(file, line)){
            if (line.find(search) != string::npos){
                lineInfo.line = line;
                lineInfo.lineNum = lineCount;
                linesFound.push_back(lineInfo);
            }
            lineCount++;
        }
        printLines(linesFound, false, false, "");
    }
}

void searchStrFromFile(string options, string search, string fileName){
    int OptCount = options.length() - 2;
    options.erase(0,2);
    bool printLineNum = false;
    bool printOccurenceCount = false;
    if (options.find('l') != string::npos){
        printLineNum = true;
    }
    if (options.find('o') != string::npos){
        printOccurenceCount = true;     
    }

    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open()){
        vector<Line> linesFound;
        string line;
        int lineCount = 1;
        Line lineInfo;
        while (getline(file, line)){
            if (line.find(search) != string::npos){
                lineInfo.line = line;
                lineInfo.lineNum = lineCount;
                linesFound.push_back(lineInfo);
            }
            lineCount++;
        }
        printLines(linesFound, printLineNum, printOccurenceCount, search);
    }
}

int main(int argc, char **argv){
    if (argc < 2){
        noArgs();
        return 0;
    }
    else if (argc == 3){
        searchStrFromFile(argv[1], argv[2]);
        return 0;
    }
    else if (argc == 4){
        searchStrFromFile(argv[1], argv[2], argv[3]);
        return 0;
    }

}