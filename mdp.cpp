/*****************************************
**	Name: Kuan Wen Ng		**	
**	Student Number: 5078052		**
**	Filename: mdp.cpp (ass2)	**
**	Description: MDP Value 		**
**			Iteration	**
**					**
*****************************************/

#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <utility>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

//Constant for epsilon and discount factor
const double EPSILON = 0.0001, DISCOUNT_FACTOR = 0.9;

typedef map<string, double> ProbMap;	//Type for state transition probabilities
typedef pair<char, double> BestAction;	//Type for action and future discounted reward

//Structured data for actions
struct Action
{
	char name;
	ProbMap prob;
};

//Structured data for states
struct State
{
	string name;	//State name
	double reward;	//Reward for state
	double optimalValue[2];	//Future discounted reward
	char optimalAction;	//Optimal action
	vector<Action> actions;	//Probabilities for state transition for all possible action of the state
};

//Compare function to sort BestAction
struct greaterAction
{
    inline bool operator() (const BestAction &action1, const BestAction &action2)
    {
        return (action1.second > action2.second);
    }
};

//Function prototype
double getFutureReward(string , vector<State> );

int main()
{
	int iteration = 1;	//Number of iterations
	Action tempAction;	//Temporary action var to set values for all actions of each states
	vector<State> states(5);	//Vector for states
	map<string, double>::iterator probIt;	//Iterator for all state transition probabilities for each state
	vector<BestAction> bestAction;	//Vector for future discounted rewards for each action, the best action will be selected here
	BestAction tempBestAction;	//Future discounted reward for an action
	bool converge = false;	//Convergence indicator

	//Initialize states
	//State1
	states[0].name = "State1";
	states[0].reward = 1;
	tempAction.name = 'A';
	tempAction.prob["State1"] = 0.2;
	tempAction.prob["State2"] = 0.8;
	states[0].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'B';
	tempAction.prob["State3"] = 1;
	states[0].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'C';
	tempAction.prob["State1"] = 1;
	states[0].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'D';
	tempAction.prob["State1"] = 0.8;
	tempAction.prob["State3"] = 0.2;
	states[0].actions.push_back(tempAction);

	//State2
	states[1].name = "State2";
	states[1].reward = -1;
	tempAction.prob.clear();
	tempAction.name = 'A';
	tempAction.prob["State1"] = 0.7;
	tempAction.prob["State4"] = 0.3;
	states[1].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'B';
	tempAction.prob["State2"] = 0.1;
	tempAction.prob["State4"] = 0.9;
	states[1].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'C';
	tempAction.prob["State3"] = 0.8;
	tempAction.prob["State4"] = 0.2;
	states[1].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'D';
	tempAction.prob["State1"] = 0.8;
	tempAction.prob["State2"] = 0.2;
	states[1].actions.push_back(tempAction);

	//State3
	states[2].name = "State3";
	states[2].reward = 0;
	tempAction.prob.clear();
	tempAction.name = 'A';
	tempAction.prob["State2"] = 0.5;
	tempAction.prob["State3"] = 0.5;
	states[2].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'B';
	tempAction.prob["State2"] = 0.5;
	tempAction.prob["State3"] = 0.5;
	states[2].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'C';
	tempAction.prob["State3"] = 1;
	states[2].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'D';
	tempAction.prob["State1"] = 0.5;
	tempAction.prob["State3"] = 0.5;
	states[2].actions.push_back(tempAction);

	//State4
	states[3].name = "State4";
	states[3].reward = 1;
	tempAction.prob.clear();
	tempAction.name = 'A';
	tempAction.prob["State4"] = 1;
	states[3].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'B';
	tempAction.prob["State2"] = 0.3;
	tempAction.prob["State4"] = 0.7;
	states[3].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'C';
	tempAction.prob["State5"] = 1;
	states[3].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'D';
	tempAction.prob["State4"] = 0.5;
	tempAction.prob["State5"] = 0.5;
	states[3].actions.push_back(tempAction);

	//State5
	states[4].name = "State5";
	states[4].reward = 3;
	tempAction.prob.clear();
	tempAction.name = 'A';
	tempAction.prob["State3"] = 0.3;
	tempAction.prob["State5"] = 0.7;
	states[4].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'B';
	tempAction.prob["State4"] = 0.6;
	tempAction.prob["State5"] = 0.4;
	states[4].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'C';
	tempAction.prob["State4"] = 0.9;
	tempAction.prob["State5"] = 0.1;
	states[4].actions.push_back(tempAction);
	tempAction.prob.clear();
	tempAction.name = 'D';
	tempAction.prob["State1"] = 0.8;
	tempAction.prob["State5"] = 0.2;
	states[4].actions.push_back(tempAction);

	for (int index = 0; index < states.size() + 1; index++)
	{
		if (index == 0)
			cout << "Iteration\t";

		else
			cout << states[index - 1].name << "\t\t";

		if (index == states.size())
			cout << endl;
	}

	do
	{
		//Store future discounted reward for previous iteration
		for (int index = 0; index < states.size(); index++)
		{
			states[index].optimalValue[0] = states[index].optimalValue[1];
		}

		//Iterate through all states
		for (int index1 = 0; index1 < states.size(); index1++)
		{
			//Iterate through all actions
			for (int index2 = 0; index2 < states[index1].actions.size(); index2++)
			{
				//Get action name and reset value
				tempBestAction.first = states[index1].actions[index2].name;
				tempBestAction.second = 0;

				//Iterate through all state transition probabilities for each action
				for (probIt = states[index1].actions[index2].prob.begin(); probIt != states[index1].actions[index2].prob.end(); probIt++)
				{
					tempBestAction.second += (getFutureReward(probIt->first, states) * probIt->second * DISCOUNT_FACTOR);
				}

				//Insert the future discounted reward into vector
				bestAction.push_back(tempBestAction);
			}

			//Get optimal action and future discounted reward by sorting in descending order and selecting the first element
			sort(bestAction.begin(), bestAction.end(), greaterAction());
			states[index1].optimalAction = bestAction[0].first;
			states[index1].optimalValue[1] = bestAction[0].second + states[index1].reward;
			bestAction.clear();
		}

		//Output iterations
		cout << iteration << "\t\t";
		for (int index = 0; index < states.size(); index++)
		{
			cout << states[index].optimalValue[1] << "\t\t";
		}

		//Output optimal policy
		if (iteration > 1)
		{
			cout << "Policy: ";
			for (int index = 0; index < states.size(); index++)
			{
				cout << states[index].optimalAction;

				if (index != states.size() - 1)
					cout << ",";

				else
					cout << endl;
			}
		}

		else
			cout << endl;

		iteration++;

		//Validate convergence
		converge = true;
		for (int index = 0; index < states.size() && converge; index++) //Stop search when any state do not converge
		{
			if (fabs(states[index].optimalValue[1] - states[index].optimalValue[0]) >= EPSILON)
				converge = false;

			else
				converge = true;
		}

	//Reiterate while not converged
	} while (!converge);

	return 0;
}

//Function to get future discounted reward of previous iteration
double getFutureReward(string name, vector<State> states)
{
	for (int index = 0; index < states.size(); index++)
	{
		if (name == states[index].name)
			return states[index].optimalValue[0];
	}
}
