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
			 << "usage: ./tp [instance filepath]" << endl;
	 	return 2;
	}

    try{
        Data data = Data(argv[1]);
		try{
			solve(data);
		}catch(IloException& e) {
			cout << e.getMessage();
			e.end();
			return 1;
		} 
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
	IloArray<IloIntVarArray> x(env, data.getNOrigins());
	for(int i = 0; i < data.getNOrigins(); i++)
	{
		IloIntVarArray auxVec(env, data.getNDestinations(), (IloInt) 0, (IloInt)std::numeric_limits<int>::max() );
		x[i] = auxVec;
	}
	

	for(int i = 0; i < data.getNOrigins(); i++)
	{
		for(int j = 0; j < data.getNDestinations(); j++)
		{
			sprintf(name, "X(%d,%d)", i, j);

			x[i][j].setName(name);
			model.add(x[i][j]);		
		}
	}
	
	/// Objective Function
	IloExpr sum_ij_CX(env);

	for(int i = 0; i < data.getNOrigins(); i++)
	{
		for(int j = 0; j < data.getNDestinations(); j++)
		{
			sum_ij_CX += data.getCost(i, j) * x[i][j];
		}
	}

	model.add(IloMinimize(env, sum_ij_CX));

	/// Restrictions

	// (1) - Each origin's offer must be met
	for(int i = 0; i < data.getNOrigins(); i++)
	{
		IloExpr sum_j_X(env);

		for(int j = 0; j < data.getNDestinations(); j++)
		{
			sum_j_X += x[i][j];
		}

		IloRange restriction = (sum_j_X - data.getOffer(i) == 0);
		sprintf(name, "R_Agent(%d)", i);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Each destination's demand must be met
	for(int j = 0; j < data.getNDestinations(); j++)
	{
		IloExpr sum_i_X(env);

		for(int i = 0; i < data.getNOrigins(); i++)
		{
			sum_i_X += x[i][j];
		}

		IloRange restriction = (sum_i_X - data.getDemand(j) == 0);
		sprintf(name, "R_Job(%d)", j);
		restriction.setName(name);

		model.add(restriction);
	}

	/// Solve
	IloCplex tp(model);
	tp.setParam(IloCplex::TiLim, 1*60*60);
	tp.setParam(IloCplex::Threads, 1);

	tp.exportModel("model.lp");

	try{
		tp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << tp.getStatus() << endl;
	cout << "total cost: " << tp.getObjValue() << endl;
	
	cout << endl;
	for (int i = 0; i < data.getNOrigins(); i++)
	{
		for(int j = 0; j < data.getNDestinations(); j++)
		{
			if(tp.getValue(x[i][j]) > 0)
				cout << "origin " << i 
					 << " sends " << tp.getValue(x[i][j]) 
					 << " to destination " << j 
					 << endl;
		}
	}
	cout << endl;
}