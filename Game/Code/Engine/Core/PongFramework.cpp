#include "PongFramework.h"

#include "../Collisions/PongCollisionManager.h"
#include "../States/TitleState.h"
#include "../States/DebugState.h"
#include "../../Utilities/DifficultyMode.h"
#include <Utilities/Utils.h>

PongFramework::PongFramework()
{
	GameConstants::ObjectSpeed = 8.f;
	GameConstants::WindowTitle = "SFMLPongGame";
	GameConstants::WindowColour = Colour::Black;

	Initialise();
}

void PongFramework::Initialise()
{
	m_gameMgr.GetFontMgr().AddFonts("../Game/Resources/Fonts/");
	m_gameMgr.GetShaderMgr().AddShaders("../Game/Resources/Shaders/");
	m_gameMgr.GetSoundMgr().AddSounds("../Game/Resources/Sounds/");
	m_gameMgr.GetSoundMgr().AddMusic("../Game/Resources/Music/");
	m_gameMgr.GetTextureMgr().AddTextures("../Game/Resources/Textures/");

	DECL_GET_OR_RETURN(renderer, m_gameMgr.GetRenderer())
	renderer->Initialise(GameConstants::ScreenDim, GameConstants::WindowTitle);
	m_gameMgr.SetICollisionManager(std::make_shared<PongCollisionManager>());

	m_gameMgr.GetGameStateMgr()->ChangeState(new TitleState(&m_gameMgr));
}

void PongFramework::Shutdown()
{
}