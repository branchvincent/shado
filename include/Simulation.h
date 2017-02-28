/****************************************************************************
*																			*
*	File:		Simulation.h												*
*																			*
*	Author:		Branch Vincent												*
*																			*
*	Purpose:	This file defines the Simulation class.						*
*																			*
****************************************************************************/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <string>
#include "Flags.h"
#include "Parameters.h"
#include "Task.h"
// #include "events/Event.h"
#include "Team.h"
// #include "Agent.h"
// #include "TaskType.h"
#include "../lib/EasyLogging.h"
#include "../lib/EZProgressBar.h"

using namespace std;

// Helper functions and definitions

// bool cmpTaskArrs(Task* t1, Task* t2) {return t1->arrivesBefore(*t2);}
bool cmpEvents(Event* t1, Event* t2) {return (*t1) < (*t2);}

/****************************************************************************
*																			*
*	Definition of Simulation class											*
*																			*
****************************************************************************/

//  Notes
//  - Add utilization count for tasks cut off at end of phase (processAllDepts)

class Simulation
{
//	Public member functions

	public:

	//	Constructors

        Simulation(string file, Flags flags);
		// ~Simulation() {del *;}

	//	Other member functions

        void run();
		void output(ostream& out) const;

//  Private member functions

    private:
		void run(int rep);
		void reset();

//	Data members

	public:
		Flags flags;				// command line flags
		ez::EZProgressBar bar;		// progress bar
		Parameters parameters;		// simulation parameters
		vector<Team> teams;			// trains
		list<Event*> events;		// event list
		// Stats stats;				// TODO: statistics
};

//	Operators

ostream& operator<<(ostream& out, const Simulation& s) {s.output(out); return out;}

/****************************************************************************
*																			*
*	Function:	Simulation													*
*																			*
*	Purpose:	To construct a new simulation								*
*																			*
****************************************************************************/

Simulation::Simulation(string file, Flags flags = Flags()) : flags(flags), parameters(file)
	// teams{Team(), Team(Shift("01-01 12:00", "01-02 3:00"))}
{
//	TODO: Perform assertions (i.e. duration of simulation and traffic)

	LOG(INFO) << "Initializing simulation";
	parameters.numReps = 3;
//  Initialize srand

	//TODO: change this
    if (!flags.isOn("rand"))
        srand((unsigned int) time(0));
    else
        srand(0);
	util::seed = rand();

	LOG(INFO) << "Flags = " << flags;

	// if (flags.isOn("rand"))
    //     util::seed = time(0);
    // else
    //     util::seed = 0;
	// LOG(INFO) << "Seed = " << util::seed;
	util::randNumGen = default_random_engine(util::seed);

	// if (flags.isOn("verbose"))
	// {
	// 	cout << parameters << endl;
	// 	cout << flags << endl;
	// }

	// if (flags.isOn("pbar"))
	// bar = ez::EZProgressBar(parameters.numReps);

//	Fill teams

	teams.push_back(Team());
	teams.push_back(Team(Shift("01-01 12:00", "01-02 3:00")));
	// ptree params;
	// read_xml(file, params);
	//
	// for (const auto& team : params.get_child("teams"))
	// {
	// 	if (team.first == "team")
	// 	{
	// 		teams.push_back(Team(team.second));
	// 	}
	// }
}

/****************************************************************************
*																			*
*	Function:	run                                                         *
*																			*
*	Purpose:	To run the simulation for the specified number of           *
*               replications                                                *
*																			*
****************************************************************************/

void Simulation::run()
{
	LOG(INFO) << "Simulation started";

//	Run each replication

	bar.start();
	for (int i = 0; i < parameters.numReps; i++, ++bar)
	{
		run(i);
		reset();
	}

	LOG(INFO) << "Simulation completed";
}

/****************************************************************************
*																			*
*	Function:	run                                                         *
*																			*
*	Purpose:	To run one replication of the simulation                    *
*																			*
****************************************************************************/

void Simulation::run(int rep)
{
	LOG(INFO) << "Rep " << rep + 1 << " of " << parameters.numReps << " started";

//	Get events

	for (Team& ti : teams)
	{
		events.merge(ti.getEvents());
	}
	events.sort(cmpEvents);

//	Process events

	// for (Event* e : events)
	// {
	// 	e->process();
	// }

	Event* e;

	while (!events.empty())
	{
		e = events.front();
		e->process(events);
		events.pop_front();
		// LOG(DEBUG) << "Event size = " << events.size();
	}

//	TODO: Might need to clear agents for next rep

	LOG(INFO) << "Rep " << rep + 1 << " of " << parameters.numReps << " completed";
}

void Simulation::reset()
{
	for (Team& team : teams)
	{
		team.reset();
	}
	events.clear();
}

/****************************************************************************
*																			*
*	Function:	output														*
*																			*
*	Purpose:	To output a simulation										*
*																			*
****************************************************************************/

void Simulation::output(ostream& out) const
{
	out << "Parameters: " << parameters << endl;
	out << "Flags: " << flags;
	//	TODO: Might want to output stats
}

#endif