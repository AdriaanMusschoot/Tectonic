#ifndef TCT_CONFIG
#define TCT_CONFIG

#include <string>
#include <memory>
#include <numeric>
#include <vector>

namespace tct
{
	using direction = std::pair<int, int>;

	using gridOffset = std::pair<int, int>;

	using shape = std::vector<gridOffset>;

	namespace img
	{

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

		static direction constexpr LEFT{ -1, 0 };
		static direction constexpr RIGHT{ 1, 0 };
		static direction constexpr UP{ 0, -1 };
		static direction constexpr DOWN{ 0, 1 };

	}

	namespace error
	{

		static int constexpr IN{ -1 };

	}

	namespace shapes
	{

		namespace one
		{

			shape const SHAPE{ { 0, 0 } };

		}

		namespace two
		{

			shape const RIGHT{ { 0, 0 }, { 1, 0 } };
			
		}

		namespace three
		{

			shape const STRAIGHT_RIGHT{ { 0, 0 } , { 1, 0 }, { 2, 0 } };

			shape const ANGLED_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 } };

		}

		namespace four
		{

			shape const STRAIGHT_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } };

			shape const T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 } };

			shape const L_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } };

			shape const LIGHTNING_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }};

			shape const SQUARE_RIGHT{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };

		}

		namespace five
		{

			shape const STRAIGHT_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 } };
							   
			shape const T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } };
							   
			shape const L_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 3, 1 } };
							   
			shape const LIGHTNING_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } };
							   
			shape const SQUARE_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } };
							   
			shape const W_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } };
							   
			shape const S_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } };
							   
			shape const R_RIGHT{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } };
							   
			shape const X_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, -1 }, { 1, 1 } };
							   
			shape const SMALL_T_RIGHT{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 2, 1 } };

		}

	}

}

#endif //TCT_CONFIG