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
			 << "usage: ./bpp [instance filepath]" << endl;
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

	/// Variables
	
	// X_i_j
	IloArray<IloBoolVarArray> x(env, data.getNItems());
	for(int i = 0; i < data.getNItems(); i++)
	{
		IloBoolVarArray auxVec(env, data.getNItems());
		x[i] = auxVec;
	}

	for(int i = 0; i < data.getNItems(); i++)
	{
		for(int j = 0; j < data.getNItems(); j++)
		{
			sprintf(name, "X(%d,%d)", i, j);
			x[i][j].setName(name);
			model.add(x[i][j]);		
		}
	}

	// Y_j
	IloBoolVarArray y(env, data.getNItems());
	for(int j = 0; j < data.getNItems(); j++)
	{
		sprintf(name, "Y(%d)", j);
		y[j].setName(name);
		model.add(y[j]);
	}

	/// Objective Function
	IloExpr sum_j_Y(env);

	for(int j = 0; j < data.getNItems(); j++)
	{
		sum_j_Y += y[j];
	}

	model.add(IloMinimize(env, sum_j_Y));

	/// Restrictions

	// (1) - Each item must go in 1 bin
	for(int i = 0; i < data.getNItems(); i++)
	{
		IloExpr sum_j_X(env);

		for(int j = 0; j < data.getNItems(); j++)
		{
			sum_j_X += x[i][j];
		}

		IloRange restriction = (sum_j_X == 1);
		sprintf(name, "R_Item(%d)", i);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Weight of items cannot surpass bin capacity
	for(int j = 0; j < data.getNItems(); j++)
	{
		IloExpr sum_i_WX(env);

		for(int i = 0; i < data.getNItems(); i++)
		{
			sum_i_WX += data.getWeight(i) * x[i][j];
		}

		IloRange restriction = (sum_i_WX - data.getCapacity()*y[j] <= 0);
		sprintf(name, "R_Capacity(%d)", j);
		restriction.setName(name);

		model.add(restriction);
	}

	/// Solve
	const double tol = 0.1;

	IloCplex bpp(model);
	bpp.setParam(IloCplex::TiLim, 1*60*60);
	bpp.setParam(IloCplex::Threads, 1);

	bpp.exportModel("model.lp");

	try{
		bpp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << bpp.getStatus() << endl;
	cout << "number of bins used: " << bpp.getObjValue() << endl;
	
	cout << endl;
	for (int i = 0; i < data.getNItems(); i++)
	{
		for(int j = 0; j < data.getNItems(); j++)
		{
			if(bpp.getValue(x[i][j]) >= 1 - tol)
				cout << "item " << i << " in bin " << j << endl;
		}
	}
	cout << endl;
}