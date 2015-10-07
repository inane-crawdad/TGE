#ifndef _TGE_COMMONTYPES_H_
#define _TGE_COMMONTYPES_H_

namespace TGE
{
	// base class for classes than should not be copied
	class CUncopyable
	{
		CUncopyable(const CUncopyable&);
		CUncopyable& operator=(const CUncopyable&);
	public:
		CUncopyable(){}
		~CUncopyable(){}
	};
}

#endif // _TGE_COMMONTYPES_H_