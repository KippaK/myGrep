#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>

using namespace std;

struct Line {
    string line;
    int lineNum;
};

void printLines(vector<Line> lines, bool printLineNumber, bool printOccurenceCount, bool reverseSearch, bool ignoreCase, string search){
    if (lines.size() == 0){
        cout << "No occurences of lines containing \"" << search << "\"";
        return;
    }
    for (int i = 0; i < lines.size(); i++){
        if (printLineNumber){
            cout << lines[i].lineNum << ":\t";
        }
        cout << lines[i].line << '\n';
    }
    if (printOccurenceCount){
        cout << "\nOccurences of lines ";
        if (reverseSearch){
            cout << "not ";
        }
        cout << "containing \"" << search << "\"";
        if (ignoreCase){
            cout << " with case ignored";
        }
        cout << ": " << lines.size();
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

void searchStrFromFile(string options, string search, string fileName){
    if (!filesystem::exists(fileName)){
        cout << "File \"" << fileName << "\" doesn't exist in working directory.";
        return;
    }

    bool printLineNum = false;
    bool printOccurenceCount = false;
    bool reverseSearch = false;
    bool ignoreCase = false;

    if (options.length() > 2){
        options.erase(0,2);
        if (options.find('l') != string::npos){
            printLineNum = true;
        }
        if (options.find('o') != string::npos){
            printOccurenceCount = true;     
        }
        if (options.find('r') != string::npos){
            reverseSearch = true;
        }
        if (options.find('i') != string::npos){
            ignoreCase = true;
        }
    }

    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open()){
        vector<Line> linesFound;
        string line;
        int lineCount = 1;
        Line lineInfo;

        string searchOriginal = search;
        string lineOriginal;
        while (getline(file, line)){
            lineOriginal = line;

            if (ignoreCase){
                transform(search.begin(), search.end(), search.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                transform(line.begin(), line.end(), line.begin(),
                    [](unsigned char c){ return std::tolower(c); });
            }

            if (line.find(search) != string::npos && reverseSearch == false){
                lineInfo.line = lineOriginal;
                lineInfo.lineNum = lineCount;
                linesFound.push_back(lineInfo);
            }
            else if (line.find(search) == string::npos && reverseSearch == true){
                lineInfo.line = lineOriginal;
                lineInfo.lineNum = lineCount;
                linesFound.push_back(lineInfo);
            }
            lineCount++;
        }

        printLines(linesFound, printLineNum, printOccurenceCount, reverseSearch, ignoreCase, search);
    }
}

int main(int argc, char **argv){
    if (argc == 1){
        noArgs();
        return 0;
    }
    else if (argc == 3){
        searchStrFromFile("-o", argv[1], argv[2]);
        return 0;
    }
    else if (argc == 4){
        searchStrFromFile(argv[1], argv[2], argv[3]);
        return 0;
    }
}