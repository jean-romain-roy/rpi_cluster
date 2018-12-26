#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

    // 1 + nbr of arguments passed
    if(argc == 3){

        int arg1 = atoi(argv[1]);
        int arg2 = atoi(argv[2]);

        cout << (arg1 + arg2) << endl;

    }else{
        cout << "Wrong number of input arguments" << endl;
    }

    return 0;
}
