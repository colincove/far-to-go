#pragma once

#include <ctime>
#include <chrono>

namespace BoulderLeaf::Metrics
{
	class blTime
	{
	public:
		using tick = long int unsigned;
		using time = std::chrono::time_point<std::chrono::high_resolution_clock>;
		using duration = std::chrono::duration<long long, std::nano>;
	private:
		tick mTick;
		duration mTotal;
		duration mDelta;
		time mCurrent;
		time mBegin;
	public:
		blTime();
		blTime(tick tick,
			duration total,
			duration delta,
			time current,
			time begin);

		blTime NewTick() const;

		tick Tick() const
		{
			return mTick;
		}

		duration Delta() const
		{
			return mDelta;
		}

		float DeltaSeconds() const
		{
			return std::chrono::duration_cast<std::chrono::duration<float>>(mDelta).count();
		}

		duration Total() const
		{
			return mTotal;
		}

		float TotalSeconds() const
		{
			return std::chrono::duration_cast<std::chrono::duration<float>>(mTotal).count();
		}

		time Current() const
		{
			return mCurrent;
		}

		time Begin() const
		{
			return mBegin;
		}
	};
}