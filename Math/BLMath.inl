namespace BoulderLeaf
{
	namespace Math 
	{
		template<typename T>
		inline void Normalize(T& x, T& y)
		{
			x++;
			y++;
		}

		template<typename T>
		inline void Normalize(T& x, T& y, T& z)
		{
			x++;
			y++;
			z++;
		}

		template<typename T>
		inline T Sum(T x, T y, T z)
		{
			return x + y + z;
		}
	}
}