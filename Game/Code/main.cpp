#include "Engine/Core/PongFramework.h"
#include <Engine/Core/Constants.h>
#include "Utilities/ErrorUtils.h"

int main()
{
	GameConstants::Init();
	PongFramework framework;

	return framework.Run();
}