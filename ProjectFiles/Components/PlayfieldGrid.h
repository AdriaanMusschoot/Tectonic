#ifndef TCT_PLAYFIELD_GRID
#define TCT_PLAYFIELD_GRID

#include <vector>

#include "Component.h"

namespace tct
{

	class PlayfieldGridComponent final : public amu::Component
	{
	public:
		PlayfieldGridComponent(amu::GameObject* ownerObjectPtr, int rows, int cols);
		virtual ~PlayfieldGridComponent() override = default;

		PlayfieldGridComponent(PlayfieldGridComponent const&) = delete;
		PlayfieldGridComponent(PlayfieldGridComponent&&) = delete;
		PlayfieldGridComponent& operator=(PlayfieldGridComponent const&) = delete;
		PlayfieldGridComponent& operator=(PlayfieldGridComponent&&) = delete;
	private:
		std::vector<int> m_GridVec{};
	};

}

#endif //TCT_PLAYFIELD_GRID