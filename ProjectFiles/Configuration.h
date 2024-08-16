#ifndef TCT_CONFIG
#define TCT_CONFIG

#include <string>
#include <memory>

namespace tct
{
	namespace img
	{
		static std::string const NR1{ "1.png" };
		static std::string const NR2{ "2.png" };
		static std::string const NR3{ "3.png" };
		static std::string const NR4{ "4.png" };
		static std::string const NR5{ "5.png" };
	}

	namespace dir
	{

		static std::pair<int, int> const LEFT{ 1, 0 };
		static std::pair<int, int> const RIGHT{ -1, 0 };
		static std::pair<int, int> const UP{ 0, -1 };
		static std::pair<int, int> const DOWN{ 0, 1 };

	}

}

#endif //TCT_CONFIG