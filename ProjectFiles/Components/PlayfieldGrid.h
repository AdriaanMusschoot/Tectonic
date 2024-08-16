#ifndef TCT_PLAYFIELD_GRID
#define TCT_PLAYFIELD_GRID

#include <vector>
#include <unordered_set>

#include "Component.h"
#include "Scene.h"

namespace tct
{

	class TectonicGridComponent final : public amu::Component
	{
	public:
		TectonicGridComponent(amu::GameObject* ownerObjectPtr, amu::Scene* scenePtr, unsigned int rows, unsigned int cols, unsigned int highestNr);
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
		std::vector<unsigned int> m_RegionIDVec{};

		void FillRegions(amu::Scene* scenePtr);
		void FillGrid(amu::Scene* scenePtr);
		std::string_view GetNrFilePath(unsigned int nr);
		bool IsNeighbourCellHorizontal(std::pair<int, int> const& direction, unsigned int colIdx);
		bool IsNeighbourCellVertical(std::pair<int, int> const& direction, unsigned int rowIdx);
		bool DoAdjecentCellsContainValue(unsigned int rowIdx, unsigned int colIdx, unsigned int arrIdx, unsigned int cellValue);
	};

}

#endif //TCT_PLAYFIELD_GRID