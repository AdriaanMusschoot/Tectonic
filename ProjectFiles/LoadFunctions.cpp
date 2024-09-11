#include "LoadFunctions.h"

#include "GameObject.h"

#include "Components/PlayfieldGrid.h"

void tct::LoadMainScene(amu::Scene* scenePtr)
{
	std::unique_ptr playfieldGridPtr{ std::make_unique<amu::GameObject>() };

	playfieldGridPtr->AddComponent<amu::TransformComponent>(playfieldGridPtr.get(), glm::vec2{ 0, 0 });

	playfieldGridPtr->AddComponent<tct::TectonicGridComponent>(playfieldGridPtr.get(), scenePtr, 10, 10, 5);

	scenePtr->Add(std::move(playfieldGridPtr));
}
