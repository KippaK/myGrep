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

struct Options {
    bool lineNumber = false;
    bool occurenceCount = false;
    bool reverseSearch = false;
    bool ignoreCase = false;
};

void printUsage(string fileName){
    cout << "Usage: " << fileName << " [-options] <pattern> <filename>\n"
        << "Options available:\n"
        << "\to = Print the number of occurences\n"
        << "\tl = Print the line numbers where the occurence is found\n"
        << "\tr = Inverse search\n"
        << "\ti = Case insensitie search";
}

void stringToLowerCase(string &str){
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

bool validOptions(string options){
    if (options[0] != '-' || options[1] != 'o'){ return false; }
    options.erase(0,2);
    if (options.find_first_not_of("ilor") != std::string::npos){ return false; }
    return true;
}

void printLines(vector<Line> lines, Options options, string search){
    if (lines.size() == 0){
        cout << "No occurences of lines containing \"" << search << "\"";
        return;
    }
    for (int i = 0; i < lines.size(); i++){
        if (options.lineNumber){
            cout << lines[i].num << ":\t";
        }
        cout << lines[i].text << '\n';
    }
    if (options.occurenceCount){
        cout << "Occurences of lines ";
        if (options.reverseSearch){
            cout << "not ";
        }
        cout << "containing \"" << search << "\"";
        if (options.ignoreCase){
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

void searchStrFromFile(string optionsStr, string search, string fileName, string runningFile){
    
    fstream file;
    file.open(fileName, ios::in);
    vector<Line> linesFound;
    string line;
    int lineCount = 1;
    Line currentLine;
    Options options;

    string searchOriginal = search;
    string lineOriginal;

    if (!filesystem::exists(fileName)){
        cout << "File \"" << fileName << "\" doesn't exist in working directory.";
        return;
    }
    if (!validOptions(optionsStr)){ 
        cout << "Options not recognized.\n";
        printUsage(runningFile);
        return;
    }
    optionsStr.erase(0, 2);

    if (optionsStr.length() > 0){
        if (optionsStr.find('l') != string::npos){
            options.lineNumber = true;
        }
        if (optionsStr.find('o') != string::npos){
            options.occurenceCount = true;     
        }
        if (optionsStr.find('r') != string::npos){
            options.reverseSearch = true;
        }
        if (optionsStr.find('i') != string::npos){
            options.ignoreCase = true;
        }
    }

    while (getline(file, line)){
        lineOriginal = line;

        if (options.ignoreCase){
            stringToLowerCase(search);
            stringToLowerCase(line);
        }

        if (line.find(search) != string::npos && options.reverseSearch == false){
            currentLine.text = lineOriginal;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        else if (line.find(search) == string::npos && options.reverseSearch == true){
            currentLine.text = lineOriginal;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        lineCount++;
    }
    printLines(linesFound, options, searchOriginal);
}


int main(int argc, char **argv){
    string runningFile = (filesystem::path(argv[0]).filename()).generic_string();
    switch (argc){
        case 1:
            noArgs();
            break;

        case 3:
            searchStrFromFile("-o", argv[1], argv[2], runningFile);
            break;

        case 4:
            searchStrFromFile(argv[1], argv[2], argv[3], runningFile);
            break;
        default:
            cerr << "Invalid amount of arguments!\n";
            printUsage(runningFile);
            break;
    }
    return 0;
}