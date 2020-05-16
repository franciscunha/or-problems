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
			 << "usage: ./mkp [instance filepath]" << endl;
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
	char name[128];

	/// Variable
	IloArray<IloBoolVarArray> x(env, data.getNItems());
	for(int i = 0; i < data.getNItems(); i++)
	{
		IloBoolVarArray auxVec(env, data.getNKnapsacks());
		x[i] = auxVec;
	}

	for(int i = 0; i < data.getNItems(); i++)
	{
		for(int j = 0; j < data.getNKnapsacks(); j++)
		{
			sprintf(name, "X(%d,%d)", i, j);
			x[i][j].setName(name);
			model.add(x[i][j]);		
		}
	}

	/// Objective Function
	IloExpr sum_ij_VX(env);

	for(int i = 0; i < data.getNItems(); i++)
	{
		for(int j = 0; j < data.getNKnapsacks(); j++)
		{
			sum_ij_VX += data.getValue(i) * x[i][j];
		}
	}

	model.add(IloMaximize(env, sum_ij_VX));

	/// Restrictions

	// (1) - Each item can only go in 1 knapsack (or none)
	for(int i = 0; i < data.getNItems(); i++)
	{
		IloExpr sum_j_X(env);

		for(int j = 0; j < data.getNKnapsacks(); j++)
		{
			sum_j_X += x[i][j];
		}

		IloRange restriction = (sum_j_X - 1 <= 0);
		sprintf(name, "R_Item(%d)", i);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Weight of items cannot surpass knapsack capacity
	for(int j = 0; j < data.getNKnapsacks(); j++)
	{
		IloExpr sum_i_WX(env);

		for(int i = 0; i < data.getNItems(); i++)
		{
			sum_i_WX += data.getWeight(i) * x[i][j];
		}

		IloRange restriction = (sum_i_WX - data.getCapacity(j) <= 0);
		sprintf(name, "R_Capacity(%d)", j);
		restriction.setName(name);

		model.add(restriction);
	}

	/// Solve
	const double tol = 0.1;

	IloCplex mkp(model);
	mkp.setParam(IloCplex::TiLim, 1*60*60);
	mkp.setParam(IloCplex::Threads, 1);

	mkp.exportModel("model.lp");

	try{
		mkp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << mkp.getStatus() << endl;
	cout << "total value: " << mkp.getObjValue() << endl;
	
	cout << "took items:" << endl;
	for (int i = 0; i < data.getNItems(); i++)
	{
		for(int j = 0; j < data.getNKnapsacks(); j++)
		{
			if(mkp.getValue(x[i][j]) >= 1 - tol)
				cout << "item " << i << " in knapsack " << j << endl;
		}
	}
	cout << endl;
}