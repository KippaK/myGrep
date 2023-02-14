#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>

using namespace std;

struct Line {
    string text;
    int num;
};

void stringToLowerCase(string &str){
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

bool validOptions(string options){
    if (options[0] != '-' || options[1] != 'o'){
        cerr << "Options not recognized.";
        return false;
    }
    options.erase(0,2);
    if (options.find_first_not_of("ilor") != std::string::npos){
        cerr << "Options not recognized.";
        return false;
    }
    return true;
}

void printLines(vector<Line> lines, bool printLineNumber, bool printOccurenceCount, bool reverseSearch, bool ignoreCase, string search){
    if (lines.size() == 0){
        cout << "No occurences of lines containing \"" << search << "\"";
        return;
    }
    for (int i = 0; i < lines.size(); i++){
        if (printLineNumber){
            cout << lines[i].num << ":\t";
        }
        cout << lines[i].text << '\n';
    }
    if (printOccurenceCount){
        cout << "Occurences of lines ";
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

    if (str.find(search) != string::npos){
        cout << "\"" << search << "\" found in \"" << str << "\" in postion " << str.find(search);
    }
    else {
        cout << "\"" << search << "\" NOT found in \"" << str << "\"";
    }
    return;
}

void searchStrFromFile(string options, string search, string fileName){
    
    fstream file;
    file.open(fileName, ios::in);
    vector<Line> linesFound;
    string line;
    int lineCount = 1;
    Line currentLine;

    string searchOriginal = search;
    string lineOriginal;

    bool printLineNumber = false;
    bool printOccurenceCount = false;
    bool reverseSearch = false;
    bool ignoreCase = false;

    if (!filesystem::exists(fileName)){
        cout << "File \"" << fileName << "\" doesn't exist in working directory.";
        return;
    }
    if (!validOptions(options)){ return; }
    options.erase(0, 2);

    if (options.length() > 0){
        if (options.find('l') != string::npos){
            printLineNumber = true;
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

    while (getline(file, line)){
        lineOriginal = line;

        if (ignoreCase){
            stringToLowerCase(search);
            stringToLowerCase(line);
        }

        if (line.find(search) != string::npos && reverseSearch == false){
            currentLine.text = lineOriginal;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        else if (line.find(search) == string::npos && reverseSearch == true){
            currentLine.text = lineOriginal;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        lineCount++;
    }

        printLines(linesFound, printLineNumber, printOccurenceCount, reverseSearch, ignoreCase, searchOriginal);
}


int main(int argc, char **argv){
    switch (argc){
        case 1:
            noArgs();
            break;

        case 3:
            searchStrFromFile("-o", argv[1], argv[2]);
            break;

        case 4:
            searchStrFromFile(argv[1], argv[2], argv[3]);
            break;
        default:
            cerr << "Invalid amount of arguments!\n";
            cout << "Usage: " << filesystem::path(argv[0]).filename() << " [-options] <pattern> <filename>\n"
                << "Options available:\n"
                << "\to = Print the number of occurences\n"
                << "\tl = Print the line numbers where the occurence is found\n"
                << "\tr = Inverse search\n"
                << "\ti = Case insensitie search";
            break;
    }
    return 0;
}