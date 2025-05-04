#include "blTime.h"

namespace BoulderLeaf::Metrics
{
	blTime::blTime() : blTime(
		0, 
		duration(),
		duration(),
		time(),
		std::chrono::high_resolution_clock::now())
	{
	}

	blTime::blTime(tick tick, duration total, duration delta, time current, time begin) :
		mTick(tick), 
		mTotal(total),
		mDelta(delta),
		mCurrent(current),
		mBegin(begin)
	{
		
	}

	blTime blTime::NewTick() const
	{
		const time current = std::chrono::high_resolution_clock::now();
		return blTime(mTick + 1,
			current - mBegin,
			current - mCurrent,
			current,
			mBegin
		);
	}
}