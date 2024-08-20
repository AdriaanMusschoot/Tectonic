#include "PlayfieldGrid.h"

#include <iostream>
#include <random>
#include <chrono>
#include <memory>
#include <array>

#include "RenderComponent.h"
#include "TextComponent.h"

#include "Configuration.h"

tct::TectonicGridComponent::TectonicGridComponent(amu::GameObject* ownerObjectPtr, amu::Scene* scenePtr, unsigned int rows, unsigned int cols, unsigned int highestNr)
	: Component(ownerObjectPtr)
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_HighestNumber{ highestNr }
	, m_GridVec(rows * cols, 0)
	, m_RegionIDVec(rows * cols, std::make_pair(max::UIN, max::UIN))
{
	FillRegions(scenePtr);
	FillGrid(scenePtr);
}

void tct::TectonicGridComponent::FillRegions(amu::Scene*)
{
	std::random_device randDev{};
	std::mt19937 rng(randDev());
	std::vector<unsigned int> nrToGenerateVec{};
	nrToGenerateVec.reserve(m_HighestNumber - 1);
	for (unsigned int idx{}; idx < m_HighestNumber; ++idx)
	{
		nrToGenerateVec.emplace_back(idx);
	}
	std::discrete_distribution<unsigned int> biasedDist{ nrToGenerateVec.begin(), nrToGenerateVec.end() };

	unsigned int ID{};

	for (unsigned int arrIdx{}; arrIdx < std::size(m_RegionIDVec); ++arrIdx)
	{
		if (m_RegionIDVec[arrIdx].first == max::UIN)
		{
			unsigned int const regionSize{ biasedDist(rng) + 1 };
			std::cout << regionSize << "\n";
			m_RegionIDVec[arrIdx].first = ID;
			m_RegionIDVec[arrIdx].second = regionSize;

			unsigned int currentArrIdx{ arrIdx };

			for (unsigned int regionIdx{ 1 }; regionIdx < regionSize; ++regionIdx)
			{
				std::vector<direction> const possibleNeighbourVec{ GetNeighbourDirectionsWithoutID(currentArrIdx) };
				unsigned int const nrNeighbours{ static_cast<unsigned int>(std::size(possibleNeighbourVec)) };

				if (nrNeighbours > 0)
				{
					std::uniform_int_distribution<unsigned int> distDirs{ 0, nrNeighbours - 1 };
					direction const& dir{ possibleNeighbourVec[distDirs(rng)] };
					currentArrIdx = GetNeighbourIdx(dir, currentArrIdx);
					m_RegionIDVec[currentArrIdx].first = ID;
					m_RegionIDVec[currentArrIdx].second = regionSize;
				}
				else
				{
					break;
				}
			}
			++ID;
		}
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
			while (DoAdjecentCellsContainValue(arrIdx, cellValue))
			{
				cellValue = dist(randDev);
			} 
			m_GridVec[arrIdx] = cellValue;

			std::unique_ptr cellUPtr{ std::make_unique<amu::GameObject>() };
			cellUPtr->AddComponent<amu::TransformComponent>(cellUPtr.get(), glm::vec2{ border + colIdx * sideLength, border + rowIdx * sideLength });
			cellUPtr->SetParent(GetComponentOwner(), true);

			cellUPtr->AddComponent<amu::TextComponent>(cellUPtr.get(), std::to_string(m_RegionIDVec[arrIdx].second), fnt::LINGUA, 20);

			scenePtr->Add(std::move(cellUPtr));
		}
	}
}

unsigned int tct::TectonicGridComponent::GetNeighbourIdxLeft(unsigned int arrIdx)
{
	unsigned int const colIdxLeft{ arrIdx % m_Cols - 1 };
	if (colIdxLeft >= 0 and colIdxLeft < m_Cols)
	{
		return arrIdx - 1;
	}
	return max::UIN;
}

unsigned int tct::TectonicGridComponent::GetNeighbourIdxRight(unsigned int arrIdx)
{
	unsigned int const colIdxRight{ arrIdx % m_Cols + 1 };
	if (colIdxRight < m_Cols)
	{
		return arrIdx + 1;
	}
	return max::UIN;
}

unsigned int tct::TectonicGridComponent::GetNeighbourIdxUp(unsigned int arrIdx)
{
	unsigned int const rowIdxUp{ arrIdx / m_Cols - 1 };
	if (rowIdxUp >= 0 and rowIdxUp < m_Rows)
	{
		return arrIdx - m_Cols;
	}
	return max::UIN;
}

unsigned int tct::TectonicGridComponent::GetNeighbourIdxDown(unsigned int arrIdx)
{
	unsigned int const rowIdxDown{ arrIdx / m_Cols + 1 };
	if (rowIdxDown < m_Rows)
	{
		return arrIdx + m_Cols;
	}
	return max::UIN;
}

unsigned int tct::TectonicGridComponent::GetNeighbourIdx(direction const& dir, unsigned int arrIdx)
{
	if (dir == tct::dir::LEFT)
	{
		return GetNeighbourIdxLeft(arrIdx);
	}
	else if (dir == tct::dir::RIGHT)
	{
		return GetNeighbourIdxRight(arrIdx);
	}
	else if (dir == tct::dir::UP)
	{
		return GetNeighbourIdxUp(arrIdx);
	}
	else if (dir == tct::dir::DOWN)
	{
		return GetNeighbourIdxDown(arrIdx);
	}
	return max::UIN;
}

bool tct::TectonicGridComponent::DoAdjecentCellsContainValue(unsigned int arrIdx, unsigned int cellValue)
{
	std::unordered_set<unsigned int> tempSet{};

	unsigned int const cellLeftIdx{ GetNeighbourIdxLeft(arrIdx) };
	unsigned int const cellRightIdx{ GetNeighbourIdxRight(arrIdx) };
	unsigned int const cellTopIdx{ GetNeighbourIdxUp(arrIdx) };
	unsigned int const cellBottomIdx{ GetNeighbourIdxDown(arrIdx) };

	if (cellLeftIdx != max::UIN) 
	{
		tempSet.insert(m_GridVec[cellLeftIdx]);
	}

	if (cellRightIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[cellRightIdx]);
	}

	if (cellTopIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[cellTopIdx]);
	}

	if (cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[cellBottomIdx]);
	}

	if (cellLeftIdx != max::UIN and cellTopIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[arrIdx - 1 - m_Cols]);
	}

	if (cellLeftIdx != max::UIN and cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[arrIdx - 1 + m_Cols]);
	}

	if (cellRightIdx != max::UIN and cellTopIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[arrIdx + 1 - m_Cols]);
	}

	if (cellRightIdx != max::UIN and cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_GridVec[arrIdx + 1 + m_Cols]);
	}

	return tempSet.contains(cellValue);
}

std::vector<tct::direction> tct::TectonicGridComponent::GetPossibleNeighbourDirections(unsigned int arrIdx)
{
	std::vector<direction> possibleDirsVec{};
	possibleDirsVec.reserve(tct::dir::NR_OF_DIRECTIONS);
	if (GetNeighbourIdxLeft(arrIdx) != max::UIN)
	{
		possibleDirsVec.emplace_back(tct::dir::LEFT);
	}
	if (GetNeighbourIdxRight(arrIdx) != max::UIN)
	{
		possibleDirsVec.emplace_back(tct::dir::RIGHT);
	}
	if (GetNeighbourIdxUp(arrIdx) != max::UIN)
	{
		possibleDirsVec.emplace_back(tct::dir::UP);
	}
	if (GetNeighbourIdxDown(arrIdx) != max::UIN)
	{
		possibleDirsVec.emplace_back(tct::dir::DOWN);
	}
	return possibleDirsVec;
}

std::vector<tct::direction> tct::TectonicGridComponent::GetNeighbourDirectionsWithoutID(unsigned int arrIdx)
{
	std::vector<direction> dirWithoutIDVec{};
	for (direction const& dir : GetPossibleNeighbourDirections(arrIdx))
	{
		if (m_RegionIDVec[GetNeighbourIdx(dir, arrIdx)].first == max::UIN)
		{
			dirWithoutIDVec.emplace_back(dir);
		}
	}
	return dirWithoutIDVec;
}
