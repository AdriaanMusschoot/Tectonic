#include "PlayfieldGrid.h"

#include <iostream>
#include <random>
#include <chrono>
#include <memory>
#include <array>

#include "RenderComponent.h"

#include "Configuration.h"

tct::TectonicGridComponent::TectonicGridComponent(amu::GameObject* ownerObjectPtr, amu::Scene* scenePtr, unsigned int rows, unsigned int cols, unsigned int highestNr)
	: Component(ownerObjectPtr)
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_HighestNumber{ highestNr }
	, m_GridVec(rows * cols, 0)
	, m_RegionIDVec(rows * cols, 0)
{
	FillRegions(scenePtr);
	FillGrid(scenePtr);
}

void tct::TectonicGridComponent::FillRegions(amu::Scene*)
{
	std::array<std::pair<int, int>, 4> const directions{ { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } } };

	std::random_device randDev{};

	std::uniform_int_distribution<unsigned int> dist{ 1, m_HighestNumber };
	

	for (unsigned int idx{}; idx < std::size(m_RegionIDVec); ++idx)
	{
		unsigned int regionSize{ dist(randDev) };
		std::cout << regionSize << "\n";
	}
}

void tct::TectonicGridComponent::FillGrid(amu::Scene* scenePtr)
{
	std::random_device randDev{};

	std::uniform_int_distribution<unsigned int> dist{ 1, m_HighestNumber };

	constexpr unsigned int border{ 50 };
	constexpr unsigned int sideLength{ 100 };

	for (unsigned int rowIdx{}; rowIdx < m_Rows; ++rowIdx)
	{
		for (unsigned int colIdx{}; colIdx < m_Cols; ++colIdx)
		{
			unsigned int const arrIdx{ rowIdx * m_Cols + colIdx };
			assert(arrIdx < std::size(m_GridVec));
			
			unsigned int cellValue{ dist(randDev) };
			while (DoAdjecentCellsContainValue(rowIdx, colIdx, arrIdx, cellValue))
			{
				cellValue = dist(randDev);
			} 
			m_GridVec[arrIdx] = cellValue;

			std::unique_ptr cellUPtr{ std::make_unique<amu::GameObject>() };
			cellUPtr->AddComponent<amu::TransformComponent>(cellUPtr.get(), glm::vec2{ border + colIdx * sideLength, border + rowIdx * sideLength });
			cellUPtr->SetParent(GetComponentOwner(), true);

			auto rdrCompPtr{ cellUPtr->AddComponent<amu::RenderComponent>(cellUPtr.get(), GetNrFilePath(cellValue)) };
			auto dim{ rdrCompPtr->GetSize() };
			rdrCompPtr->SetSourceRectangle(SDL_Rect{ 0, 0, dim.x, dim.y });

			scenePtr->Add(std::move(cellUPtr));
		}
	}
}

std::string_view tct::TectonicGridComponent::GetNrFilePath(unsigned int nr)
{
	switch (nr)
	{
	case 1: 
		return tct::img::NR1;
		break;
	case 2: 
		return tct::img::NR2;
		break;
	case 3: 
		return tct::img::NR3;
		break;
	case 4: 
		return tct::img::NR4;
		break;
	case 5: 
		return tct::img::NR5;
		break;
	default:
		return "";
	}
}

bool tct::TectonicGridComponent::IsNeighbourCellHorizontal(std::pair<int, int> const& direction, unsigned int colIdx)
{
	if (direction == tct::dir::LEFT)
	{
		return colIdx - 1 >= 0 and colIdx - 1 < m_Cols;
	}
	else if (direction == tct::dir::RIGHT)
	{
		return colIdx + 1 < m_Cols;
	}
	return false;
}

bool tct::TectonicGridComponent::IsNeighbourCellVertical(std::pair<int, int> const& direction, unsigned int rowIdx)
{
	if (direction == tct::dir::UP)
	{
		return rowIdx - 1 >= 0 and rowIdx - 1 < m_Rows;
	}
	else if (direction == tct::dir::DOWN)
	{
		return rowIdx + 1 < m_Rows;
	}
	return false;
}

bool tct::TectonicGridComponent::DoAdjecentCellsContainValue(unsigned int rowIdx, unsigned int colIdx, unsigned int arrIdx, unsigned int cellValue)
{
	std::unordered_set<unsigned int> tempSet{};

	bool const cellLeft{ IsNeighbourCellHorizontal(tct::dir::LEFT, colIdx) };
	bool const cellRight{ IsNeighbourCellHorizontal(tct::dir::RIGHT, colIdx) };
	bool const cellTop{ IsNeighbourCellVertical(tct::dir::UP, rowIdx) };
	bool const cellBottom{ IsNeighbourCellVertical(tct::dir::DOWN, rowIdx) };

	if (cellLeft)
	{
		tempSet.insert(m_GridVec[arrIdx - 1]);
	}

	if (cellRight)
	{
		tempSet.insert(m_GridVec[arrIdx + 1]);
	}

	if (cellTop)
	{
		tempSet.insert(m_GridVec[arrIdx - m_Cols]);
	}

	if (cellBottom)
	{
		tempSet.insert(m_GridVec[arrIdx + m_Cols]);
	}

	if (cellLeft and cellTop)
	{
		tempSet.insert(m_GridVec[arrIdx - 1 - m_Cols]);
	}

	if (cellLeft and cellBottom)
	{
		tempSet.insert(m_GridVec[arrIdx - 1 + m_Cols]);
	}

	if (cellRight and cellTop)
	{
		tempSet.insert(m_GridVec[arrIdx + 1 - m_Cols]);
	}

	if (cellRight and cellBottom)
	{
		tempSet.insert(m_GridVec[arrIdx + 1 + m_Cols]);
	}

	return tempSet.contains(cellValue);
}
