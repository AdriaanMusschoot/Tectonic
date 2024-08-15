#ifndef TCT_PLAYFIELD_GRID
#define TCT_PLAYFIELD_GRID

#include <vector>

#include "Component.h"

namespace tct
{

	class TectonicGridComponent final : public amu::Component
	{
	public:
		TectonicGridComponent(amu::GameObject* ownerObjectPtr, unsigned int rows, unsigned int cols, unsigned int highestNr);
		virtual ~TectonicGridComponent() override = default;

		TectonicGridComponent(TectonicGridComponent const&) = delete;
		TectonicGridComponent(TectonicGridComponent&&) = delete;
		TectonicGridComponent& operator=(TectonicGridComponent const&) = delete;
		TectonicGridComponent& operator=(TectonicGridComponent&&) = delete;
	private:
		unsigned int const m_Rows{};
		unsigned int const m_Cols{};
		unsigned int const m_HighestNumber{};
		std::vector<unsigned int> m_GridVec{};

		void FillGrid();
	};

}

#endif //TCT_PLAYFIELD_GRID