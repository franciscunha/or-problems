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
			 << "usage: ./ap [instance filepath]" << endl;
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
	IloArray<IloBoolVarArray> x(env, data.getNAgents());
	for(int i = 0; i < data.getNAgents(); i++)
	{
		IloBoolVarArray auxVec(env, data.getNJobs());
		x[i] = auxVec;
	}

	for(int i = 0; i < data.getNAgents(); i++)
	{
		for(int j = 0; j < data.getNJobs(); j++)
		{
			sprintf(name, "X(%d,%d)", i, j);
			x[i][j].setName(name);
			model.add(x[i][j]);		
		}
	}

	/// Objective Function
	IloExpr sum_ij_CX(env);

	for(int i = 0; i < data.getNAgents(); i++)
	{
		for(int j = 0; j < data.getNJobs(); j++)
		{
			sum_ij_CX += data.getCost(i, j) * x[i][j];
		}
	}

	model.add(IloMinimize(env, sum_ij_CX));

	/// Restrictions

	// (1) - Each agent must have exactly 1 job
	for(int i = 0; i < data.getNAgents(); i++)
	{
		IloExpr sum_j_X(env);

		for(int j = 0; j < data.getNJobs(); j++)
		{
			sum_j_X += x[i][j];
		}

		IloRange restriction = (sum_j_X == 1);
		sprintf(name, "R_Agent(%d)", i);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Each job must have max 1 agent
	for(int j = 0; j < data.getNJobs(); j++)
	{
		IloExpr sum_i_X(env);

		for(int i = 0; i < data.getNAgents(); i++)
		{
			sum_i_X += x[i][j];
		}

		IloRange restriction = (sum_i_X - 1 <= 0);
		sprintf(name, "R_Job(%d)", j);
		restriction.setName(name);

		model.add(restriction);
	}

	/// Solve
	const double tol = 0.1;

	IloCplex ap(model);
	ap.setParam(IloCplex::TiLim, 1*60*60);
	ap.setParam(IloCplex::Threads, 1);

	ap.exportModel("model.lp");

	try{
		ap.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << ap.getStatus() << endl;
	cout << "total cost: " << ap.getObjValue() << endl;
	
	cout << endl;
	for (int i = 0; i < data.getNAgents(); i++)
	{
		for(int j = 0; j < data.getNJobs(); j++)
		{
			if(ap.getValue(x[i][j]) >= 1 - tol)
				cout << "agent " << i << " assigned to job " << j << endl;
		}
	}
	cout << endl;
}