#ifndef _FUNCDELEGATE_H_
#define _FUNCDELEGATE_H_

#include "TGE.h"
#include <vector>

typedef void	(CALLBACK *pProcFunc)(void *pParam),
				(CALLBACK *pMsgProcFunc)(const TGE::TWindowMessage& msg, void *pParam);

template<class T1>
class CFuncDelegate;

template<>
class CFuncDelegate<pProcFunc>
{
	struct _TFunc
	{
		pProcFunc pFunc;
		void * pParam;
	};

	std::vector<_TFunc> _funcList;

	CFuncDelegate(const CFuncDelegate&);
	CFuncDelegate& operator=(const CFuncDelegate&);

public:

	CFuncDelegate(){}

	bool IsEmpty()
	{
		return _funcList.empty();
	}

	void Add(pProcFunc pFunc, void *pParam)
	{
		_TFunc temp;
		temp.pFunc = pFunc;
		temp.pParam = pParam;
		_funcList.push_back(temp);
	}

	void Remove(pProcFunc pFunc, void *pParam)
	{
		for (auto it = _funcList.begin(); it < _funcList.end(); ++it)
			if (it->pFunc == pFunc && it->pParam == pParam)
			{
				_funcList.erase(it);
				break;
			}
	}

	void Invoke()
	{
		for (auto it : _funcList)
		{
			(*it.pFunc)(it.pParam);
		}
	}
};

template<>
class CFuncDelegate<pMsgProcFunc>
{
	struct _TFunc
	{
		pMsgProcFunc pFunc;
		void * pParam;
	};

	std::vector<_TFunc> _funcList;

	CFuncDelegate(const CFuncDelegate&);
	CFuncDelegate& operator=(const CFuncDelegate&);

public:

	CFuncDelegate(){}

	bool IsEmpty()
	{
		return _funcList.empty();
	}

	void Add(pMsgProcFunc pFunc, void *pParam)
	{
		_TFunc temp;
		temp.pFunc = pFunc;
		temp.pParam = pParam;
		_funcList.push_back(temp);
	}

	void Remove(pMsgProcFunc pFunc, void *pParam)
	{
		for (auto it = _funcList.begin(); it < _funcList.end(); ++it)
			if (it->pFunc == pFunc && it->pParam == pParam)
			{
				_funcList.erase(it);
				break;
			}	
	}

	void Invoke(const TGE::TWindowMessage& msg)
	{
		for (auto it : _funcList)
		{
			(*it.pFunc)(msg, it.pParam);
		}
	}
};

typedef CFuncDelegate<pProcFunc> TProcDelegate;
typedef CFuncDelegate<pMsgProcFunc> TMsgProcDelegate;

#endif