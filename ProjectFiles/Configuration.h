#ifndef TCT_CONFIG
#define TCT_CONFIG

#include <string>
#include <memory>
#include <numeric>
#include <vector>

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
		static std::string const HORIZONTAL_BAR{ "TouchingTileHorizontal.png" };
		static std::string const VERTICAL_BAR{ "TouchingTileVertical.png" };

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

	namespace shapes
	{

		//first int signifies rows second signifies columns
		namespace one
		{

			static std::vector<std::pair<int, int>> const SHAPE{ { 0, 0 } };

		}

		namespace two
		{

			static std::vector<std::pair<int, int>> const RIGHT{ { 0, 0 }, { 1, 0 } };
			
		}

		namespace three
		{

			static std::vector<std::pair<int, int>> const STRAIGHT_RIGHT{ { 0, 0 } , { 1, 0 }, { 2, 0 } };

			static std::vector<std::pair<int, int>> const ANGLED_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 } };

		}

		namespace four
		{

			static std::vector<std::pair<int, int>> const STRAIGHT_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } };

			static std::vector<std::pair<int, int>> const T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 } };

			static std::vector<std::pair<int, int>> const L_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } };

			static std::vector<std::pair<int, int>> const LIGHTNING_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }};

			static std::vector<std::pair<int, int>> const SQUARE_RIGHT{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };

		}

		namespace five
		{

			static std::vector<std::pair<int, int>> const STRAIGHT_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 } };

			static std::vector<std::pair<int, int>> const T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } };

			static std::vector<std::pair<int, int>> const L_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 3, 1 } };

			static std::vector<std::pair<int, int>> const LIGHTNING_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } };

			static std::vector<std::pair<int, int>> const SQUARE_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } };

			static std::vector<std::pair<int, int>> const W_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } };

			static std::vector<std::pair<int, int>> const S_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } };

			static std::vector<std::pair<int, int>> const R_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } };

			static std::vector<std::pair<int, int>> const X_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, -1 }, { 1, 1 } };

			static std::vector<std::pair<int, int>> const SMALL_T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 2, 1 } };

		}

	}

}

#endif //TCT_CONFIG