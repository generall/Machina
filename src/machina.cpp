//============================================================================
// Name        : machina.cpp
// Author      : generall
// Version     :
// Copyright   : Your copyright notice
// Description : end-state machine Test program
//============================================================================

#include <iostream>
#include "TMureMachina.h"
using namespace std;

int main()
{
	//mach::TVertex<int,int> t(0);
	TMureMachina< int, int, int> m;
	m.addVertexOut(0,0);
	m.addVertexOut(1,0);
	m.addVertexOut(2,100);
	m.addEdge(0,1,3);
	m.addEdge(1,0,0);
	m.setCurrent(0);
	m.submitSignal(3);
	m.submitSignal(0);
	std::vector<std::pair< std::pair<int, int>, int> > list;
	m.getEdgeList(list);
	cout << list[0].second << endl; // prints
	return 0;
}
