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

struct Query {
    string exeName;
    string searchString;
    string searchFile;
    Options options;
};

void printUsage(string fileName){
    cout << "Usage: .\\" << fileName << " [-options] <pattern> <filename>\n"
        << "Options available:\n"
        << "\to = Print the number of occurences\n"
        << "\tl = Print the line numbers where the occurence is found\n"
        << "\tr = Inverse search\n"
        << "\ti = Case insensitive search";
}

void stringToLowerCase(string &str){
    transform(
        str.begin(), str.end(), str.begin(), [](unsigned char c){
            return std::tolower(c); 
        }
    );
}

Query parseArguments(int argc, char **argv){
    Query query;
    query.exeName = (filesystem::path(argv[0]).stem()).generic_string();

    string optionsString = argv[1];
    if (optionsString.length() > 2 && argc == 4){
        optionsString.erase(0, 2);
        
        if (optionsString.find('l') != string::npos){
            query.options.lineNumber = true;
        }
        if (optionsString.find('o') != string::npos){
            query.options.occurenceCount = true;     
        }
        if (optionsString.find('r') != string::npos){
            query.options.reverseSearch = true;
        }
        if (optionsString.find('i') != string::npos){
            query.options.ignoreCase = true;
        }
    }
    if (argc == 3){
        query.searchString = argv[1];
        query.searchFile = argv[2];
    }
    else if (argc == 4){
        query.searchString = argv[2];
        query.searchFile = argv[3];
    }
    return query;
}

bool validOptions(string options){
    if (options[0] != '-' || options[1] != 'o'){
        return false;
    }

    options.erase(0,2);

    if (options.find_first_not_of("ilor") != std::string::npos){
        return false;
    }

    return true;
}

void printLines(vector<Line> lines, Query query){
    if (lines.size() == 0){
        cout << "No occurences of lines containing \"" << query.searchString << "\"";
        return;
    }
    for (int i = 0; i < lines.size(); i++){
        if (query.options.lineNumber){
            cout << lines[i].num << ":\t";
        }
        cout << lines[i].text << '\n';
    }

    if (query.options.occurenceCount){
        cout << "Occurences of lines ";

        if (query.options.reverseSearch) {
            cout << "not ";
        }

        cout << "containing \"" << query.searchString << "\"";

        if (query.options.ignoreCase) {
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

void searchStrFromFile(Query query){
    
    fstream file;
    file.open(query.searchFile, ios::in);
    vector<Line> linesFound;
    string line;
    int lineCount = 1;
    Line currentLine;

    string searchLowerCase = query.searchString;
    string lineLowerCase;

    if (!filesystem::exists(query.searchFile)){
        cout << "File \"" << query.searchFile << "\" doesn't exist in working directory.";
        return;
    }

    while (getline(file, line)){
        lineLowerCase = line;
        if (query.options.ignoreCase){
            stringToLowerCase(searchLowerCase);
            stringToLowerCase(lineLowerCase);
        }
        if (line.find(searchLowerCase) != string::npos && !query.options.reverseSearch){
            currentLine.text = line;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        else if (line.find(searchLowerCase) == string::npos && query.options.reverseSearch){
            currentLine.text = line;
            currentLine.num = lineCount;
            linesFound.push_back(currentLine);
        }
        lineCount++;
    }
    printLines(linesFound, query);
}

int main(int argc, char **argv){
    Query query;

    switch (argc){
        case 1:
            noArgs();
            break;
        case 3:
            query = parseArguments(argc, argv);
            searchStrFromFile(query);
            break;
        case 4:
            if (!validOptions(argv[1])){ 
                cout << "Options not recognized.\n";
                printUsage(query.exeName);
                return 0;
            }
            query = parseArguments(argc, argv);
            searchStrFromFile(query);
            break;
        default:
            cerr << "Invalid amount of arguments!\n";
            printUsage((filesystem::path(argv[0]).stem()).generic_string());
            break;
    }
    return 0;
}