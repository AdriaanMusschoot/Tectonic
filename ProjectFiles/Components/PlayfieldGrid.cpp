#include "PlayfieldGrid.h"

tct::PlayfieldGridComponent::PlayfieldGridComponent(amu::GameObject* ownerObjectPtr, int rows, int cols)
	: Component(ownerObjectPtr)
	, m_GridVec(rows * cols)
{
}
