/*
 * TMureMachine.h
 *
 *  Created on: 10.05.2013
 *      Author: generall
 */

#ifndef TMUREMACHINE_H_
#define TMUREMACHINE_H_

#include "TMachina.h"

/*
 * Класс, реализующий автомат Мура.
 * Унаследован от TMachine следовательно все функции сохранены.
 * Присутствует новый шаблонный параметр, задающий класс для выходного сигнала OutSignal.
 * Выходные сигналы хранятся в виде ассоциативного массива, ставящего в соответвие идентификатор вершины и выходной сигнал.
 * Добавлена функция добавления вершины addVertexOut, задающая выходное значение для добавляемой вершины, в этом классе пользоваться
 * необходимо ей и только ей.
 *
 *
 */

template<class ID, class InSignal, class OutSignal>
class TMureMachina : public mach::TMachina<ID, InSignal>
{
private:
	std::map<ID, OutSignal> output;
public:
	TMureMachina();
	virtual ~TMureMachina();
	virtual void addVertexOut(ID id, OutSignal out);
	virtual OutSignal getOutput();
	virtual void getOutList(std::vector<std::pair<ID, OutSignal> >& list);
};

template<class ID, class InSignal, class OutSignal>
inline TMureMachina<ID, InSignal, OutSignal>::TMureMachina()
{
}

template<class ID, class InSignal, class OutSignal>
inline TMureMachina<ID, InSignal, OutSignal>::~TMureMachina()
{
}


template<class ID, class InSignal, class OutSignal>
inline void TMureMachina<ID, InSignal, OutSignal>::addVertexOut(ID id,
		OutSignal out)
{
	addVertex(id);
	output.insert(std::make_pair(id, out));
}

template<class ID, class InSignal, class OutSignal>
inline OutSignal TMureMachina<ID, InSignal, OutSignal>::getOutput()
{
	return output[this->getCurrentId()];
}

template<class ID, class InSignal, class OutSignal>
inline void TMureMachina<ID, InSignal, OutSignal>::getOutList(
		std::vector<std::pair<ID, OutSignal> >& list)
{
	typename std::map<ID, OutSignal>::iterator it;

	for (it = output.begin(); it != output.end(); it++)
	{
		list.push_back(std::make_pair(it->first,it->second));
	}
}

#endif /* TMUREMACHINE_H_ */
