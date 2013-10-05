/*
 * TMachina.h
 *
 *  Created on: 09.05.2013
 *      Author: generall
 */

#ifndef TMACHINA_H_
#define TMACHINA_H_

#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <stdexcept>
typedef unsigned int uint;

namespace mach
{
template<class ID, class InSignal> class TVertex;
template<class ID, class InSignal> class TEdge;

/*
 * Базовый класс для вершин(состояний автомата)
 * содержит сектор из базового класса для ребер автомата.
 * Все вершины имеют уникальный идентификатор шаблонного параметра ID, который может быть любым классом.
 */
template<class ID, class InSignal>
class TVertex
{
private:
	ID identifier;
	std::vector<TEdge<ID, InSignal> > edges;
public:
	virtual void connect(TVertex<ID, InSignal> *to, InSignal s); //
	virtual void getConnection(std::vector<TVertex<ID, InSignal> *> &v);
	virtual bool isConnected(ID id);
	virtual bool isSiglan(InSignal s);
	virtual TEdge<ID, InSignal>* getEgde(ID id);
	virtual void deleteConnection(ID id);
	virtual void deleteConnectionBySignal(InSignal s);
	virtual TVertex<ID, InSignal>* getVertex(InSignal s);

	TVertex(ID id);
	TVertex();
	virtual ~TVertex();

	ID getIdentifier() const
	{
		return identifier;
	}

	void setIdentifier(ID identifier)
	{
		this->identifier = identifier;
	}
};

/*
 * Базовый класс для ребер(дуг)
 * Содержит указатели на начальное и конечное ребро.
 * А так же содержит входной сигнал типа шаблонного InSignal.
 * Переход автомата в каждое следующее состояние определяется наличием ребра с членом signal равным входному сигналу.
 *
 */
template<class ID, class InSignal>
class TEdge
{
private:

	TVertex<ID, InSignal> *from;
	TVertex<ID, InSignal> *to;
	InSignal signal;
public:
	TEdge(InSignal s);
	virtual ~TEdge();

	InSignal getSignal()
	{
		return signal;
	}

	void setSignal(InSignal signal)
	{
		this->signal = signal;
	}

	TVertex<ID, InSignal>* getFrom() const
	{
		return from;
	}

	void setFrom(TVertex<ID, InSignal>* from)
	{
		this->from = from;
	}

	TVertex<ID, InSignal>* getTo() const
	{
		return to;
	}

	void setTo(TVertex<ID, InSignal>* to)
	{
		this->to = to;
	}
};

/*
 * Базовый класс для автомата.
 * Содержит ассоциативный массив из вершин, индексируемый типом ID.
 * Содержит разнообразные функции для получения\удаления\добавления вершин\ребер.
 * Содержит начальное состоние.
 * Все обращения к вершинам происходят по объекту типа ID.
 * Обращения к ребру задаются парой (ID,ID).
 * Автомат может переходить в некоторое другое состояние под воздействием сходного сигнала типа InSignal
 * при условии, что существует ребро с равным входному сигналу значением. Если такого ребра нет, генерируется исключение.
 *
 *
 *
 *
 *
 */

template<class ID, class InSignal>
class TMachina
{
private:
	std::map<ID, TVertex<ID, InSignal> > vertexes;
	TVertex<ID, InSignal> *current_state;
public:
	virtual void addVertex(ID id);
	virtual void deleteVertex(ID id);
	virtual void addEdge(ID id1, ID id2, InSignal s);
	virtual void deleteEdge(ID id1, ID id2);
	virtual void setCurrent(ID id);
	virtual void submitSignal(InSignal s);
	virtual ID getCurrentId();
	virtual void getVertexList(std::vector<ID>& list);
	virtual void getEdgeList(
			std::vector<std::pair<std::pair<ID, ID>, InSignal> >& list);

	TMachina();
	virtual ~TMachina();
};

}

template<class ID, class InSignal>
inline mach::TMachina<ID, InSignal>::TMachina()
{
	current_state = 0;
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::addVertex(ID id)
{
	vertexes.insert(std::make_pair(id, TVertex<ID, InSignal>(id)));
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::deleteVertex(ID id)
{
	if (vertexes.count(id) != 0)
	{
		typename std::map<ID, TVertex<ID, InSignal> >::iterator it;
		for (it = vertexes.begin(); it != vertexes.end(); it++)
		{
			if (it->second.isConnected(id))
			{
				it->second.deleteConnection(id);
			}
		}
		vertexes.erase(id);
	}
	else
	{
		throw std::logic_error("no vertex to delete");
	}
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::addEdge(ID id1, ID id2, InSignal s)
{
	vertexes[id1].connect(&vertexes[id2], s);
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::deleteEdge(ID id1, ID id2)
{
	vertexes[id1].deleteConnection(id2);
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::setCurrent(ID id)
{
	current_state = &vertexes[id];
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::submitSignal(InSignal s)
{
	current_state = current_state->getVertex(s);
}

template<class ID, class InSignal>
inline ID mach::TMachina<ID, InSignal>::getCurrentId()
{
	if (current_state != 0)
	{
		return current_state->getIdentifier();
	}
	else
	{
		return 0;
	}
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::getVertexList(std::vector<ID>& list)
{
	typename std::map<ID, TVertex<ID, InSignal> >::iterator it;

	for (it = vertexes.begin(); it != vertexes.end(); it++)
	{
		list.push_back(it->first);
	}
}

template<class ID, class InSignal>
inline void mach::TMachina<ID, InSignal>::getEdgeList(
		std::vector<std::pair<std::pair<ID, ID>, InSignal> >& list)
{
	typename std::map<ID, TVertex<ID, InSignal> >::iterator it;
	for (it = vertexes.begin(); it != vertexes.end(); it++)
	{
		std::vector<TVertex<ID, InSignal> *> v;
		it->second.getConnection(v);
		for (uint j = 0; j < v.size(); j++)
		{
			InSignal s = vertexes[it->first].getEgde(v[j]->getIdentifier())->getSignal() ;
			list.push_back(std::make_pair(std::make_pair(it->first, v[j]->getIdentifier()),s));
		}

	}
}

template<class ID, class InSignal>
inline mach::TMachina<ID, InSignal>::~TMachina()
{
}

/* TEdge Implementation here */

template<class ID, class InSignal>
inline mach::TEdge<ID, InSignal>::TEdge(InSignal s)
{
	signal = s;
	from = 0;
	to = 0;
}

template<class ID, class InSignal>
inline mach::TEdge<ID, InSignal>::~TEdge()
{
}

template<class ID, class InSignal>
inline mach::TEdge<ID, InSignal>* mach::TVertex<ID, InSignal>::getEgde(ID id)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getTo()->getIdentifier() == id)
		{
			return &edges[i];
		}
	}
	throw std::logic_error("Vertex not connected");
}

/* TVertex Implementation here */

template<class ID, class InSignal>
inline void mach::TVertex<ID, InSignal>::connect(TVertex<ID, InSignal>* to,
		InSignal s)
{
	if (!this->isSiglan(s))
	{
		TEdge<ID, InSignal> temp(s);
		temp.setFrom(this);
		temp.setTo(to);
		edges.push_back(temp);
	}
	else
	{
		throw std::logic_error("Signal already exists");
	}

}

template<class ID, class InSignal>
inline mach::TVertex<ID, InSignal>::TVertex(ID id)
{
	identifier = id;
}

template<class ID, class InSignal>
inline bool mach::TVertex<ID, InSignal>::isSiglan(InSignal s)
{ //TODO заменить на map и делать поиск по map
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getSignal() == s)
		{
			return true;
		}
	}
	return false;
}

template<class ID, class InSignal>
inline bool mach::TVertex<ID, InSignal>::isConnected(ID id)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getTo()->getIdentifier() == id)
		{
			return true;
		}
	}
	return false;
}

template<class ID, class InSignal>
inline void mach::TVertex<ID, InSignal>::getConnection(
		std::vector<mach::TVertex<ID, InSignal> *>& v)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		v.push_back(edges[i].getTo());
	}
}

template<class ID, class InSignal>
inline void mach::TVertex<ID, InSignal>::deleteConnection(ID id)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getTo()->getIdentifier() == id)
		{
			edges.erase(edges.begin() + i);
			return;
		}
	}
	throw std::logic_error("Vertex not connected");
}

template<class ID, class InSignal>
inline void mach::TVertex<ID, InSignal>::deleteConnectionBySignal(InSignal s)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getSignal() == s)
		{
			edges.erase(edges.begin() + i);
			return;
		}
	}
	throw std::logic_error("Egde not connected");
}

template<class ID, class InSignal>
inline mach::TVertex<ID, InSignal>* mach::TVertex<ID, InSignal>::getVertex(
		InSignal s)
{
	for (uint i = 0; i < edges.size(); i++)
	{
		if (edges[i].getSignal() == s)
		{
			return edges[i].getTo();
		}
	}
	throw std::logic_error("Vertex not connected");
}

template<class ID, class InSignal>
inline mach::TVertex<ID, InSignal>::TVertex()
{
}

template<class ID, class InSignal>
inline mach::TVertex<ID, InSignal>::~TVertex()
{
}

#endif /* TMACHINA_H_ */
