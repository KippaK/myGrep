#include <iostream>
#include <string>

using namespace std;

int main(){
    string str, search;
    cout << "Give a string from which to search for: ";
    cin >> str;
    cout << "Give search string: ";
    cin >> search;

    size_t found = str.find(search);
    if (found!=string::npos){
        cout << "\"" << search << "\" found in \"" << str << "\" in postion " << found;
    }
    else {
        cout << "\"" << search << "\" NOT found in \"" << str << "\"";
    }
    return 0;
}