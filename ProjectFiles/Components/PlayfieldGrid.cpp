#include "PlayfieldGrid.h"

#include <iostream>
#include <random>
#include <chrono>
#include <memory>
#include <array>

#include "RenderComponent.h"
#include "TextComponent.h"

#include "Configuration.h"

tct::TectonicGridComponent::TectonicGridComponent(amu::GameObject* ownerObjectPtr, amu::Scene* scenePtr, int rows, int cols, int highestNr)
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

	std::vector<int> nrToGenerateVec{};
	nrToGenerateVec.reserve(m_HighestNumber - 1);
	for (int idx{}; idx < m_HighestNumber; ++idx)
	{
		nrToGenerateVec.emplace_back(idx);
	}

	std::discrete_distribution<int> biasedDist{ nrToGenerateVec.begin(), nrToGenerateVec.end() };

	int ID{};

	for (int arrIdx{}; arrIdx < std::size(m_CellVec); ++arrIdx)
	{
		if (m_CellVec[arrIdx].RegionID == error::IN)
		{
			int const regionSize{ biasedDist(rng) + 1 };
			m_CellVec[arrIdx].RegionID = ID;
			m_CellVec[arrIdx].RegionSize = regionSize;

			int currentArrIdx{ arrIdx };

			for (int regionIdx{ 1 }; regionIdx < regionSize; ++regionIdx)
			{
				std::vector<direction> const possibleNeighbourVec{ GetNeighbourDirectionsWithoutID(currentArrIdx) };
				int const nrNeighbours{ static_cast<int>(std::size(possibleNeighbourVec)) };

				if (nrNeighbours > 0)
				{
					std::uniform_int_distribution<int> distDirs{ 0, nrNeighbours - 1 };
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
	auto* ownerPtr{ GetComponentOwner() };

	for (int rowIdx{}; rowIdx < m_Rows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < m_Cols; ++colIdx)
		{
			int const arrIdx{ rowIdx * m_Cols + colIdx };
			
			std::unique_ptr emptyCellUPtr{ std::make_unique<amu::GameObject>() };
			amu::TransformComponent* transformEmptyCompPtr{ emptyCellUPtr->AddComponent<amu::TransformComponent>(emptyCellUPtr.get(), glm::vec2{}) };
			emptyCellUPtr->SetParent(ownerPtr, true);

			auto* rdrEmptyCompPtr{ emptyCellUPtr->AddComponent<amu::RenderComponent>(emptyCellUPtr.get(), tct::img::EMPTY_TILE) };
			auto dim{ rdrEmptyCompPtr->GetSize() };
			rdrEmptyCompPtr->SetSourceRectangle(SDL_Rect{ 0, 0, dim.x, dim.y });

			glm::vec2 const emptyCellDim{ rdrEmptyCompPtr->GetSize() };

			transformEmptyCompPtr->SetLocalPosition(glm::vec2{ emptyCellDim.x * colIdx, emptyCellDim.y * rowIdx });

			std::unique_ptr numberUPtr{ std::make_unique<amu::GameObject>() };
			amu::TransformComponent* transformCompPtr{ numberUPtr->AddComponent<amu::TransformComponent>(numberUPtr.get(), glm::vec2{}) };
			numberUPtr->SetParent(emptyCellUPtr.get(), false);
			
			amu::TextComponent* textCompPtr{ numberUPtr->AddComponent<amu::TextComponent>(numberUPtr.get(), std::to_string(m_CellVec[arrIdx].RegionID), fnt::LINGUA, 20) };
			
			glm::vec2 const numberDim{ textCompPtr->GetSize() };

			transformCompPtr->SetLocalPosition(glm::vec2{ emptyCellDim.x / 2 - numberDim.x / 2, emptyCellDim.y / 2 - numberDim.y / 2 });

			scenePtr->Add(std::move(numberUPtr));
			scenePtr->Add(std::move(emptyCellUPtr));
		}
	}

	int constexpr borderThickness{ 3 };

	for (int arrIdx{}; arrIdx < std::size(m_CellVec); ++arrIdx)
	{
		assert(arrIdx < ownerPtr->GetChildCount());

		auto* childUPtr{ ownerPtr->GetChildAt(arrIdx) };

		glm::vec2 const cellDim{ childUPtr->GetComponent<amu::RenderComponent>()->GetSize() };

		if (int const leftIdx{ GetNeighbourIdxLeft(arrIdx) }; leftIdx != error::IN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[leftIdx].RegionID)
			{
				SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ 0, 0 });
			}
		}
		else
		{
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ 0, 0 });
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ borderThickness, 0 });
		}
		if (int const rightIdx{ GetNeighbourIdxRight(arrIdx) }; rightIdx != error::IN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[rightIdx].RegionID)
			{
				SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ cellDim.x - borderThickness, 0 });
			}
		}
		else
		{
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ cellDim.x - borderThickness, 0 });
			SpawnBar(Orientation::Vertical, scenePtr, childUPtr, glm::vec2{ cellDim.x - borderThickness * 2, 0 });
		}
		if (int const topIdx{ GetNeighbourIdxUp(arrIdx) }; topIdx != error::IN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[topIdx].RegionID)
			{
				SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, 0 });
			}
		}
		else
		{
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, 0 });
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, borderThickness });
		}
		if (int const bottomIdx{ GetNeighbourIdxDown(arrIdx) }; bottomIdx != error::IN)
		{
			if (m_CellVec[arrIdx].RegionID != m_CellVec[bottomIdx].RegionID)
			{ 
				SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, cellDim.y - borderThickness});
			}
		}
		else
		{
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, cellDim.y - borderThickness });
			SpawnBar(Orientation::Horizontal, scenePtr, childUPtr, glm::vec2{ 0, cellDim.y - borderThickness * 2 });
		}
	}
}

int tct::TectonicGridComponent::GetNeighbourIdxLeft(int arrIdx)
{
	int const colIdxLeft{ arrIdx % m_Cols - 1 };
	if (colIdxLeft >= 0 and colIdxLeft < m_Cols)
	{
		return arrIdx - 1;
	}
	return error::IN;
}

int tct::TectonicGridComponent::GetNeighbourIdxRight(int arrIdx)
{
	int const colIdxRight{ arrIdx % m_Cols + 1 };
	if (colIdxRight < m_Cols)
	{
		return arrIdx + 1;
	}
	return error::IN;
}

int tct::TectonicGridComponent::GetNeighbourIdxUp(int arrIdx)
{
	int const rowIdxUp{ arrIdx / m_Cols - 1 };
	if (rowIdxUp >= 0 and rowIdxUp < m_Rows)
	{
		return arrIdx - m_Cols;
	}
	return error::IN;
}

int tct::TectonicGridComponent::GetNeighbourIdxDown(int arrIdx)
{
	int const rowIdxDown{ arrIdx / m_Cols + 1 };
	if (rowIdxDown < m_Rows)
	{
		return arrIdx + m_Cols;
	}
	return error::IN;
}

int tct::TectonicGridComponent::GetNeighbourIdx(direction const& dir, int arrIdx)
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
	return error::IN;
}

bool tct::TectonicGridComponent::DoAdjecentCellsContainValue(int arrIdx, int cellValue)
{
	std::unordered_set<int> tempSet{};

	int const cellLeftIdx{ GetNeighbourIdxLeft(arrIdx) };
	int const cellRightIdx{ GetNeighbourIdxRight(arrIdx) };
	int const cellTopIdx{ GetNeighbourIdxUp(arrIdx) };
	int const cellBottomIdx{ GetNeighbourIdxDown(arrIdx) };

	if (cellLeftIdx != error::IN) 
	{
		tempSet.insert(m_CellVec[cellLeftIdx].Value);
	}

	if (cellRightIdx != error::IN)
	{
		tempSet.insert(m_CellVec[cellRightIdx].Value);
	}

	if (cellTopIdx != error::IN)
	{
		tempSet.insert(m_CellVec[cellTopIdx].Value);
	}

	if (cellBottomIdx != error::IN)
	{
		tempSet.insert(m_CellVec[cellBottomIdx].Value);
	}

	if (cellLeftIdx != error::IN and cellTopIdx != error::IN)
	{
		tempSet.insert(m_CellVec[arrIdx - 1 - m_Cols].Value);
	}

	if (cellLeftIdx != error::IN and cellBottomIdx != error::IN)
	{
		tempSet.insert(m_CellVec[arrIdx - 1 + m_Cols].Value);
	}

	if (cellRightIdx != error::IN and cellTopIdx != error::IN)
	{
		tempSet.insert(m_CellVec[arrIdx + 1 - m_Cols].Value);
	}

	if (cellRightIdx != error::IN and cellBottomIdx != error::IN)
	{
		tempSet.insert(m_CellVec[arrIdx + 1 + m_Cols].Value);
	}

	return tempSet.contains(cellValue);
}

std::vector<tct::direction> tct::TectonicGridComponent::GetPossibleNeighbourDirections(int arrIdx)
{
	std::vector<direction> possibleDirsVec{};
	possibleDirsVec.reserve(tct::dir::NR_OF_DIRECTIONS);
	if (GetNeighbourIdxLeft(arrIdx) != error::IN)
	{
		possibleDirsVec.emplace_back(tct::dir::LEFT);
	}
	if (GetNeighbourIdxRight(arrIdx) != error::IN)
	{
		possibleDirsVec.emplace_back(tct::dir::RIGHT);
	}
	if (GetNeighbourIdxUp(arrIdx) != error::IN)
	{
		possibleDirsVec.emplace_back(tct::dir::UP);
	}
	if (GetNeighbourIdxDown(arrIdx) != error::IN)
	{
		possibleDirsVec.emplace_back(tct::dir::DOWN);
	}
	return possibleDirsVec;
}

std::vector<tct::direction> tct::TectonicGridComponent::GetNeighbourDirectionsWithoutID(int arrIdx)
{
	std::vector<direction> dirWithoutIDVec{};
	for (direction const& dir : GetPossibleNeighbourDirections(arrIdx))
	{
		if (m_CellVec[GetNeighbourIdx(dir, arrIdx)].RegionID == error::IN)
		{
			dirWithoutIDVec.emplace_back(dir);
		}
	}
	return dirWithoutIDVec;
}

void tct::TectonicGridComponent::SpawnBar(Orientation const& orientationBar, amu::Scene* scenePtr, amu::GameObject* parentPtr, glm::vec2 const& offsetToParent)
{
	std::unique_ptr beamUPtr{ std::make_unique<amu::GameObject>() };
	amu::TransformComponent* transformCompPtr{ beamUPtr->AddComponent<amu::TransformComponent>(beamUPtr.get(), glm::vec2{}) };
	beamUPtr->SetParent(parentPtr, false);
	transformCompPtr->SetLocalPosition(glm::vec2{ offsetToParent.x, offsetToParent.y });
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
