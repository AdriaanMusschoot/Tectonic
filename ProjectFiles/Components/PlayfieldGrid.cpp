#include "PlayfieldGrid.h"

#include <random>
#include <chrono>

tct::TectonicGridComponent::TectonicGridComponent(amu::GameObject* ownerObjectPtr, unsigned int rows, unsigned int cols, unsigned int highestNr)
	: Component(ownerObjectPtr)
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_HighestNumber{ highestNr }
	, m_GridVec(rows * cols)
{
	FillGrid();
}

void tct::TectonicGridComponent::FillGrid()
{
	//auto seed{ std::chrono::steady_clock::now().time_since_epoch().count() };
	std::random_device randDev{};
	unsigned int seed{ randDev() };

	std::default_random_engine randEng{ seed };

	std::uniform_int_distribution<unsigned int> dist{ 1, m_HighestNumber };

	for (auto& cell : m_GridVec)
	{
		cell = dist(randDev);
	}

}
