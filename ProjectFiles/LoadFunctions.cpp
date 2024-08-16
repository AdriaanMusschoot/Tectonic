#include "LoadFunctions.h"

#include "Configuration.h"

#include "GameObject.h"
#include "RenderComponent.h"

#include "Components/PlayfieldGrid.h"

void tct::LoadMainScene(amu::Scene* scenePtr)
{
	{
		std::unique_ptr playfieldGridPtr{ std::make_unique<amu::GameObject>() };

		playfieldGridPtr->AddComponent<amu::TransformComponent>(playfieldGridPtr.get(), glm::vec2{ 0, 0 });

		playfieldGridPtr->AddComponent<tct::TectonicGridComponent>(playfieldGridPtr.get(), scenePtr, 10, 10, 5);

		/*auto* rdrCompPtr{playfieldGridPtr->AddComponent<amu::RenderComponent>(playfieldGridPtr.get(), tct::img::nr1)};

		auto dim{ rdrCompPtr->GetSize() };

		rdrCompPtr->SetSourceRectangle(SDL_Rect{ 0, 0, dim.x, dim.y });*/

		scenePtr->Add(std::move(playfieldGridPtr));
	}
}
