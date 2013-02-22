#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    char program[256];
    char method[256];
    strcpy(program, argv[0]);
    cout << program << endl;
    if(argc < 2)
    {
        cout << "Usage: " << program << " [method] " << endl;
        exit(1);
    }
    else
    {
        strcpy(method, argv[1]); // メソッド名の格納
        //if(strcmp(method, ""))
        
    }
}
