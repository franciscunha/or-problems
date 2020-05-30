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
			 << "usage: ./gcp [instance filepath]" << endl;
	 	return 2;
	}

    try{
        Data data = Data(argv[1]);
        //data.print();
        solve(data);

	}
	catch(IloAlgorithm::CannotExtractException &e)
	{
		IloExtractableArray &failed = e.getExtractables();
		std::cerr << "Failed to extract:" << std::endl;
		for (IloInt i = 0; i < failed.getSize(); ++i)
			std::cerr << "\t" << failed[i] << std::endl; 
    }
    catch(DataException e)
    {
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
	IloArray<IloBoolVarArray> x(env, data.getMaxColors());
	for(int i = 0; i < data.getMaxColors(); i++)
	{
		IloBoolVarArray auxVec(env, data.getNVertices());
		x[i] = auxVec;
	}

	for(int i = 0; i < data.getMaxColors(); i++)
	{
		for(int j = 0; j < data.getNVertices(); j++)
		{
			sprintf(name, "X(%d,%d)", i, j);
			x[i][j].setName(name);
			model.add(x[i][j]);		
		}
	}

	// Y_i
	IloBoolVarArray y(env, data.getMaxColors());
	for(int i = 0; i < data.getMaxColors(); i++)
	{
		sprintf(name, "Y(%d)", i);
		y[i].setName(name);
		model.add(y[i]);
	}

	/// Objective Function
	IloExpr sum_i_Y(env);

	for(int i = 0; i < data.getMaxColors(); i++)
	{
		sum_i_Y += y[i];
	}

	model.add(IloMinimize(env, sum_i_Y));

	/// Restrictions

	// (1) - Each vertice must have 1 color
	for(int j = 0; j < data.getNVertices(); j++)
	{
		IloExpr sum_i_X(env);

		for(int i = 0; i < data.getMaxColors(); i++)
		{
			sum_i_X += x[i][j];
		}

		IloRange restriction = (sum_i_X == 1);
		sprintf(name, "R_Cov(%d)", j);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Tells if color i is being used
	for(int i = 0; i < data.getMaxColors(); i++)
	{
		for(int j = 0; j < data.getNVertices(); j++)
		{
			IloRange restriction = (x[i][j] - y[i] <= 0);
			sprintf(name, "R_Rel(%d,%d)", i, j);
			restriction.setName(name);

			model.add(restriction);
		}
	}

	// (3) - Adjacent vertices can't have the same color	
	for(int k = 0; k < data.getNVertices(); k++)
	{
		for (int j = k+1; j < data.getNVertices(); j++)
		{
			for(int i = 0; i < data.getMaxColors(); i++)
			{
				IloRange restriction = ( (data.isAdjacent(k, j) * x[i][k]) + x[i][j] <= 1 );
				sprintf(name, "R_Adj(C:%d)(%d,%d)", i, k, j);
				restriction.setName(name);

				model.add(restriction);
			}
		}
	}

	/// Solve
	const double tol = 0.1;

	IloCplex gcp(model);
	gcp.setParam(IloCplex::TiLim, 1*60*60);
	gcp.setParam(IloCplex::Threads, 1);

	gcp.exportModel("model.lp");

	try{
		gcp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << gcp.getStatus() << endl;
	cout << "number of colors used: " << gcp.getObjValue() << endl;
	
	cout << endl;
	for (int i = 0; i < data.getMaxColors(); i++)
	{
		for(int j = 0; j < data.getNVertices(); j++)
		{
			if(gcp.getValue(x[i][j]) >= 1 - tol)
				cout << "vertice " << j << " with color " << i << endl;
		}
	}
	cout << endl;
}