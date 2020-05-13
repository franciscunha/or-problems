#include <iostream>
//#include <ilcplex/ilocplex.h>
#include "../include/Data.h"
#include "../include/DataException.h"

using namespace std;

void solve(Data &data);

int main (int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "Invalid number of arguments" << endl
			 << "usage: ./kp [dataance filepath]" << endl;
	 	return 2;
	}

    try{
        Data data = Data(argv[1]);
        solve(data);
    }catch(DataException e){
        e.what();
        return 1;
    }

    return 0;
}

void solve(Data &data)
{

}