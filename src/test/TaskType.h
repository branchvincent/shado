/****************************************************************************
*																			*
*	File:		TaskType.h													*
*																			*
*	Author:		Branch Vincent												*
*																			*
*	Purpose:	This file defines the TaskType class.						*
*																			*
****************************************************************************/

#ifndef TASKTYPE_H
#define TASKTYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "Distribution.h"

using namespace std;
using boost::property_tree::ptree;

// class Distribution;
class Task;

/****************************************************************************
*																			*
*	Definition of TaskType class											*
*																			*
****************************************************************************/

class TaskType
{
//	Public member functions

	public:

	//	Constructors

		TaskType();
		TaskType(const ptree& xmlData);
		TaskType(string name, vector<int> priority, vector<bool> isAffectedByTraffic,
			vector<Distribution> interarrival, vector<Distribution> service,
			vector<Distribution> expiration);

	//	Inspectors

		const string& getName() const;
		vector<int> getPriority() const;
		vector<bool> getIsAffectedByTraffic() const;
		vector<Distribution> getInterarrival() const;
		vector<Distribution> getService() const;
		vector<Distribution> getExpiration() const;

		int getPriority(int phase) const;
		bool getIsAffectedByTraffic(int phase) const;
		float randInterarrival(int phase);
		float randService(int phase);
		float randExpiration(int phase);

	//	Mutators

	//	Other member functions

		Task genTask(int phase);
        void output(ostream& out) const;

//	Private member functions

	private:
		void readDistributionFromXML(vector<Distribution>& dists, const ptree& xmlData);
		float genArrivalTime(int phase);
		float genServiceTime(int phase);
		float genExpirationTime(int phase, float arrivalTime, float serviceTime);

//	Data members

	private:
//		Team& team;
		string name;
		vector<int> priority;
		vector<bool> isAffectedByTraffic;
		vector<Distribution> interarrival;
		vector<Distribution> service;
		vector<Distribution> expiration;
		float lastArrival;
};

//	Operators

ostream& operator<<(ostream& out, const TaskType& t);

#endif
