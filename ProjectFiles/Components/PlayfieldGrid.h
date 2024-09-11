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
		struct CellData
		{
			int RegionID{ error::IN };
			int RegionSize{ error::IN };
			int Value{ error::IN };
		};

		enum class Orientation
		{
			Horizontal,
			Vertical,
			Diagonal
		};

		TectonicGridComponent(amu::GameObject* ownerObjectPtr, amu::Scene* scenePtr, int rows, int cols, int highestNr);
		virtual ~TectonicGridComponent() override = default;

		TectonicGridComponent(TectonicGridComponent const&) = delete;
		TectonicGridComponent(TectonicGridComponent&&) = delete;
		TectonicGridComponent& operator=(TectonicGridComponent const&) = delete;
		TectonicGridComponent& operator=(TectonicGridComponent&&) = delete;

	private:
		int const m_Rows{};
		int const m_Cols{};
		int const m_HighestNumber{};
		std::vector<CellData> m_CellVec{};

		void CreateRegions();
		void AssignValues();
		void CreateEmptyCells(amu::Scene* scenePtr);
		int GetNeighbourIdxLeft(int arrIdx);
		int GetNeighbourIdxRight(int arrIdx);
		int GetNeighbourIdxUp(int arrIdx);
		int GetNeighbourIdxDown(int arrIdx);
		int GetNeighbourIdx(direction const& dir, int arrIdx);
		bool DoAdjecentCellsContainValue(int arrIdx, int cellValue);
		std::vector<direction> GetPossibleNeighbourDirections(int arrIdx);
		std::vector<direction> GetNeighbourDirectionsWithoutID(int arrIdx);
		void SpawnBar(Orientation const& orientationBar, amu::Scene* scenePtr, amu::GameObject* parentPtr, glm::vec2 const& offsetToParent);
	};

}

#endif //TCT_PLAYFIELD_GRID