#include <iostream>
#include <string>
#include <ilcplex/ilocplex.h>
#include "../include/Data.h"

using namespace std;

void solve(Data &data);

int main (int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "Invalid number of arguments" << endl
			 << "usage: ./kp [instance filepath]" << endl;
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
	IloEnv env;
	IloModel model(env);
	char name[10];

	/// Variable
	IloBoolVarArray x(env, data.getNItems());
	for(int i = 0; i < data.getNItems(); i++)
	{
		sprintf(name, "X(%d)", i);
		x[i].setName(name);
		model.add(x[i]);
	}

	/// Objective Function
	IloExpr sumVX(env);

	for(int i = 0; i < data.getNItems(); i++)
	{
		sumVX += data.getValue(i) * x[i];
	}

	model.add(IloMaximize(env, sumVX));

	/// Restrictions

	// (1) - Weight of items cannot surpass capacity
	IloExpr sumWX(env);

	for (int i = 0; i < data.getNItems(); i++)
	{
		sumWX += data.getWeight(i) * x[i];
	}

	IloRange restriction = (sumWX - data.getCapacity() <= 0);
	sprintf(name, "Capacity");
	restriction.setName(name);

	model.add(restriction);

	/// Solve
	const double tol = 0.1;

	IloCplex kp(model);
	kp.setParam(IloCplex::TiLim, 1*60*60);
	kp.setParam(IloCplex::Threads, 1);

	kp.exportModel("model.lp");

	try{
		kp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << kp.getStatus() << endl;
	cout << "total value: " << kp.getObjValue() << endl;
	
	cout << "took items: ";
	for (int i = 0; i < data.getNItems(); i++)
	{
		if(kp.getValue(x[i]) >= 1 - tol)
			cout << i << " ";
	}
	cout << endl;
}