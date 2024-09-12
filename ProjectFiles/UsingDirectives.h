#ifndef TCT_USING_DIR
#define TCT_USING_DIR

#include <memory>
#include <vector>

namespace tct
{

	using direction = std::pair<int, int>;

	using gridOffset = std::pair<int, int>;

	using shape = std::vector<gridOffset>;

}

#endif //TCT_USING_DIR