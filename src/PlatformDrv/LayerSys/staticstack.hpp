/********************************************************************
	Created:	2016-04-06  14:15:24
	Filename: 	AVRGameConsole2.3\PlatformDrv\LayerSys\staticstack.hpp

	Author:		SlavaGrasiuk
	
	purpose:	static-static stack template class
*********************************************************************/

#ifndef STATICSTACK_HPP
#define STATICSTACK_HPP

template<typename DataType>
class StaticStack {
	DataType *top;

public:
	StaticStack(): top(nullptr) {}
		
	void push(const DataType *item) {
		item->prev = top;
		top = const_cast<DataType*>(item);
	}
	
	DataType *pop() {
		DataType *res = top;
		top = top->prev;
		return res;
	}
	
	DataType *pick() const {
		return top;
	}
	
	bool IsEmpty() const {
		return top==nullptr ? true : false;
	}
};

#endif // STATICSTACK_HPP
