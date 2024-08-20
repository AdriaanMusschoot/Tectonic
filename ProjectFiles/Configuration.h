#ifndef TCT_CONFIG
#define TCT_CONFIG

#include <string>
#include <memory>
#include <numeric>

namespace tct
{
	using direction = std::pair<int, int>;

	namespace img
	{

		static std::string const NR1{ "1.png" };
		static std::string const NR2{ "2.png" };
		static std::string const NR3{ "3.png" };
		static std::string const NR4{ "4.png" };
		static std::string const NR5{ "5.png" };
		static std::string const EMPTY_TILE{ "TileEmpty.png" };

	}

	namespace fnt
	{

		static std::string const LINGUA{ "Lingua.otf" };

	}

	namespace dir
	{
		static int constexpr NR_OF_DIRECTIONS{ 4 };

		static std::pair<int, int> constexpr LEFT{ -1, 0 };
		static std::pair<int, int> constexpr RIGHT{ 1, 0 };
		static std::pair<int, int> constexpr UP{ 0, -1 };
		static std::pair<int, int> constexpr DOWN{ 0, 1 };

	}

	namespace max
	{

		static unsigned int constexpr UIN{ std::numeric_limits<unsigned int>::max() };

	}

}

#endif //TCT_CONFIG