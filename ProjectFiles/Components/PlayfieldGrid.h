#ifndef TCT_PLAYFIELD_GRID
#define TCT_PLAYFIELD_GRID

#include <vector>
#include <unordered_set>

#include "Component.h"
#include "Scene.h"
#include "Configuration.h"

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
		unsigned int GetNeighbourIdxLeft(unsigned int arrIdx);
		unsigned int GetNeighbourIdxRight(unsigned int arrIdx);
		unsigned int GetNeighbourIdxUp(unsigned int arrIdx);
		unsigned int GetNeighbourIdxDown(unsigned int arrIdx);
		unsigned int GetNeighbourIdx(direction const& dir, unsigned int arrIdx);
		bool DoAdjecentCellsContainValue(unsigned int arrIdx, unsigned int cellValue);
		std::vector<direction> GetPossibleNeighbourDirections(unsigned int arrIdx);
		std::vector<direction> GetNeighbourDirectionsWithoutID(unsigned int arrIdx);
	};

}

#endif //TCT_PLAYFIELD_GRID