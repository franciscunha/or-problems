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
			 << "usage: ./rp [instance filepath]" << endl;
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

	// X - If person i sits at table k
	IloArray<IloBoolVarArray> x(env, data.getNPeople());
	for(int i = 0; i < data.getNPeople(); i++)
	{
		IloBoolVarArray auxVec(env, data.getNTables());
		x[i] = auxVec;
	}

	for(int i = 0; i < data.getNPeople(); i++)
	{
		for(int k = 0; k < data.getNTables(); k++)
		{
			sprintf(name, "X(%d,%d)", i, k);
			x[i][k].setName(name);
			model.add(x[i][k]);		
		}
	}
	
	// Y - If persons i and j are seated at the same table k
	IloArray<IloArray<IloBoolVarArray>> y(env, data.getNPeople());
	for(int i = 0; i < data.getNPeople(); i++)
	{
		IloArray<IloBoolVarArray> aux1(env, data.getNPeople());
		y[i] = aux1;
		
		for(int j = 0; j < data.getNPeople(); j++)
		{
			IloBoolVarArray aux2(env, data.getNTables());
			y[i][j] = aux2;
		}
	}

	for(int i = 0; i < data.getNPeople(); i++)
	{
		for(int j = 0; j < data.getNPeople(); j++)
		{
			for(int k = 0; k < data.getNTables(); k++)
			{
				sprintf(name, "Y(%d,%d,%d)", i, j, k);
				y[i][j][k].setName(name);
				model.add(y[i][j][k]);
			}
		}
	}

	/// Objective Function
	IloExpr sum_ijk_BY(env);

	for(int i = 0; i < data.getNPeople(); i++)
	{
		for(int j = 0; j < data.getNPeople(); j++)
		{
			for(int k = 0; k < data.getNTables(); k++)
			{
				sum_ijk_BY += data.getBenefit(i, j) * y[i][j][k];
			}	
		}
	}

	model.add(IloMaximize(env, sum_ijk_BY));

	/// Restrictions

	// (1) - Each person must sit at 1 table
	for(int i = 0; i < data.getNPeople(); i++)
	{
		IloExpr sum_k_X(env);

		for(int k = 0; k < data.getNTables(); k++)
		{
			sum_k_X += x[i][k];
		}

		IloRange restriction = (sum_k_X == 1);
		sprintf(name, "R_Seat(%d)", i);
		restriction.setName(name);

		model.add(restriction);
	}

	// (2) - Number of people seated cannot surpass table capacity
	for(int k = 0; k < data.getNTables(); k++)
	{
		IloExpr sum_i_X(env);

		for(int i = 0; i < data.getNPeople(); i++)
		{
			sum_i_X += x[i][k];
		}

		IloRange restriction = (sum_i_X - data.getCapacity(k) <= 0);
		sprintf(name, "R_Capacity(%d)", k);
		restriction.setName(name);

		model.add(restriction);
	}

	// (3) - Activate Y if i and j are seated at the same table k
	for(int k = 0; k < data.getNTables(); k++)
	{
		for(int i = 0; i < data.getNPeople(); i++)
		{
			for (int j = 0; j < data.getNPeople(); j++)
			{
				IloRange restriction = ( (x[i][k] + x[j][k])/2 - y[i][j][k] >= 0 );
				sprintf(name, "R_SameTable(T%d)(P%d,%d)", k, i, j);
				restriction.setName(name);

				model.add(restriction);	
			}
		}
	}

	/// Solve
	const double tol = 0.1;

	IloCplex rp(model);
	rp.setParam(IloCplex::TiLim, 1*60*60);
	rp.setParam(IloCplex::Threads, 1);

	rp.exportModel("model.lp");

	try{
		rp.solve();
	}catch(IloException& e){
		cout << e;
	}

	cout << "status: " << rp.getStatus() << endl;
	cout << "total value: " << rp.getObjValue() << endl;
	
	for (int i = 0; i < data.getNPeople(); i++)
	{
		for(int k = 0; k < data.getNTables(); k++)
		{
			if(rp.getValue(x[i][k]) >= 1 - tol)
				cout << "person " << i << " seats at table " << k << endl;
		}
	}
	cout << endl;
}