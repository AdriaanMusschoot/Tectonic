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
	, m_CellVec(rows * cols)
{
	CreateRegions();
	CreateEmptyCells(scenePtr);
}

void tct::TectonicGridComponent::CreateRegions()
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

	for (unsigned int arrIdx{}; arrIdx < std::size(m_CellVec); ++arrIdx)
	{
		if (m_CellVec[arrIdx].RegionID == max::UIN)
		{
			unsigned int const regionSize{ biasedDist(rng) + 1 };
			m_CellVec[arrIdx].RegionID = ID;
			m_CellVec[arrIdx].RegionSize = regionSize;

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
					m_CellVec[currentArrIdx].RegionID = ID;
					m_CellVec[currentArrIdx].RegionSize = regionSize;
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

void tct::TectonicGridComponent::AssignValues()
{

}

void tct::TectonicGridComponent::CreateEmptyCells(amu::Scene* scenePtr)
{
	std::random_device randDev{};

	std::uniform_int_distribution<unsigned int> dist{ 1, m_HighestNumber };

	int constexpr border{ 50 };
	int constexpr cellLength{ 100 };
	int constexpr regionBorderThickness{ 3 };

	for (unsigned int rowIdx{}; rowIdx < m_Rows; ++rowIdx)
	{
		for (unsigned int colIdx{}; colIdx < m_Cols; ++colIdx)
		{
			unsigned int const arrIdx{ rowIdx * m_Cols + colIdx };
			
			std::unique_ptr cellUPtr{ std::make_unique<amu::GameObject>() };
			cellUPtr->AddComponent<amu::TransformComponent>(cellUPtr.get(), glm::vec2{ border + colIdx * cellLength, border + rowIdx * cellLength });
			cellUPtr->SetParent(GetComponentOwner(), true);

			cellUPtr->AddComponent<amu::TextComponent>(cellUPtr.get(), std::to_string(m_CellVec[arrIdx].RegionID), fnt::LINGUA, 20);
			auto* cellPtr{ scenePtr->Add(std::move(cellUPtr)) };

			std::unique_ptr emptyCellUPtr{ std::make_unique<amu::GameObject>() };
			emptyCellUPtr->AddComponent<amu::TransformComponent>(emptyCellUPtr.get(), glm::vec2{ border + colIdx * cellLength, border + rowIdx * cellLength });
			emptyCellUPtr->SetParent(cellPtr, true);

			auto* rdrCompPtr{ emptyCellUPtr->AddComponent<amu::RenderComponent>(emptyCellUPtr.get(), tct::img::EMPTY_TILE) };
			auto dim{ rdrCompPtr->GetSize() };
			rdrCompPtr->SetSourceRectangle(SDL_Rect{ 0, 0, dim.x, dim.y });

			scenePtr->Add(std::move(emptyCellUPtr));
		}
	}

	auto* ownerPtr{ GetComponentOwner() };
	for (unsigned int arrIdx{}; arrIdx < std::size(m_CellVec); ++arrIdx)
	{
		assert(arrIdx < ownerPtr->GetChildCount());

		auto* childUPtr{ ownerPtr->GetChildAt(arrIdx) };

		if (unsigned const int leftIdx{ GetNeighbourIdxLeft(arrIdx) }; leftIdx != max::UIN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[leftIdx].RegionID)
			{
				SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ -cellLength / 2 + regionBorderThickness, 0 });
			}
		}
		else
		{
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ -cellLength / 2 + regionBorderThickness, 0 });
		}
		if (unsigned const int rightIdx{ GetNeighbourIdxRight(arrIdx) }; rightIdx != max::UIN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[rightIdx].RegionID)
			{
				SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ cellLength / 2 - regionBorderThickness, 0 });
			}
		}
		else
		{
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ cellLength / 2 - regionBorderThickness, 0 });
		}
		if (unsigned const int topIdx{ GetNeighbourIdxUp(arrIdx) }; topIdx != max::UIN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[topIdx].RegionID)
			{
				SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, -cellLength / 2 + regionBorderThickness });
			}
		}
		else
		{
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, -cellLength / 2 + regionBorderThickness });
		}
		if (unsigned const int bottomIdx{ GetNeighbourIdxDown(arrIdx) }; bottomIdx != max::UIN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[bottomIdx].RegionID)
			{ 
				SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, cellLength / 2 - regionBorderThickness});
			}
		}
		else
		{
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, cellLength / 2 - regionBorderThickness });
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
		tempSet.insert(m_CellVec[cellLeftIdx].Value);
	}

	if (cellRightIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[cellRightIdx].Value);
	}

	if (cellTopIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[cellTopIdx].Value);
	}

	if (cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[cellBottomIdx].Value);
	}

	if (cellLeftIdx != max::UIN and cellTopIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[arrIdx - 1 - m_Cols].Value);
	}

	if (cellLeftIdx != max::UIN and cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[arrIdx - 1 + m_Cols].Value);
	}

	if (cellRightIdx != max::UIN and cellTopIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[arrIdx + 1 - m_Cols].Value);
	}

	if (cellRightIdx != max::UIN and cellBottomIdx != max::UIN)
	{
		tempSet.insert(m_CellVec[arrIdx + 1 + m_Cols].Value);
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
		if (m_CellVec[GetNeighbourIdx(dir, arrIdx)].RegionID == max::UIN)
		{
			dirWithoutIDVec.emplace_back(dir);
		}
	}
	return dirWithoutIDVec;
}

void tct::TectonicGridComponent::SpawnBar(Orientation const& orientationBar, amu::Scene* scenePtr, amu::GameObject* parentPtr, glm::vec2 const& offsetToParent)
{
	glm::vec2 const& parentWorldTransform{ parentPtr->GetComponent<amu::TransformComponent>()->GetWorldPosition() };

	std::unique_ptr beamUPtr{ std::make_unique<amu::GameObject>() };
	beamUPtr->AddComponent<amu::TransformComponent>(beamUPtr.get(), glm::vec2{ parentWorldTransform.x + offsetToParent.x, parentWorldTransform.y + offsetToParent.y });
	beamUPtr->SetParent(parentPtr, true);
	std::cout << beamUPtr->GetComponent<amu::TransformComponent>()->GetWorldPosition().y << "\n";
	if (orientationBar == Orientation::Horizontal)
	{
		beamUPtr->AddComponent<amu::RenderComponent>(beamUPtr.get(), tct::img::HORIZONTAL_BAR);
	}
	else if (orientationBar == Orientation::Vertical)
	{
		beamUPtr->AddComponent<amu::RenderComponent>(beamUPtr.get(), tct::img::VERTICAL_BAR);
	}

	amu::RenderComponent* rdrCompPtr{ beamUPtr->GetComponent<amu::RenderComponent>() };
	auto dim{ rdrCompPtr->GetSize() };
	rdrCompPtr->SetSourceRectangle(SDL_Rect{ 0, 0, dim.x, dim.y });

	scenePtr->Add(std::move(beamUPtr));
}
