#include "LevelFactory.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextureRenderComponent.h"
#include "LivesComponent.h"
#include "DisplayLivesComponent.h"
#include "ScoreComponent.h"
#include "DisplayPointsComponent.h"
#include "AnimationComponent.h"

#include "InputManager.h"
#include "MoveCommand.h"
#include "KillObjectCommand.h"
#include "KillEnemyCommand.h"
#include "JoystickMoveCommand.h"                            
#include "RectColliderComponent.h"

#include "DigDugComponent.h"
#include "GridComponent.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "PookaComponent.h"
#include "FygarComponent.h"
#include "EnemyMovementComponent.h"
#include "PumpCommand.h"
#include "RockComponent.h"

#include "MainMenuComponent.h"
#include "ButtonSwitchCommand.h"
#include "ButtonPressCommand.h"
#include "ScoreSavingComponent.h"
#include "ScoreCharCycleCommand.h"
#include "ScoreLetterSwitchCommand.h"
#include "ScoreFileComponent.h"
#include "ScoreConfirmSaveCommand.h"
#include "HighScoresDisplayComponent.h"
#include "SoundMuteCommand.h"
#include "SoundSystem.h"
#include "GoToNextLevelCommand.h"

#include "Event.h"
#include <fstream>

void LevelFactory::LoadMainMenu()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	Twengine::ServiceLocator::get_sound_system().RequestStopAllSound();
	Twengine::ServiceLocator::get_sound_system().RequestLoadMusic("StartScreen/StartScreenMusic.wav", make_sdbm_hash("StartScreenMusic"));
	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic(make_sdbm_hash("StartScreenMusic"), 0.1f);


	auto menuGraphicsObj = std::make_unique<Twengine::GameObject>();
	menuGraphicsObj->GetTransform()->SetLocalPosition(0.f, 50.f);

	auto titleObject = std::make_unique<Twengine::GameObject>();
	titleObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/DigDugLogo.png");
	titleObject->GetTransform()->SetLocalPosition(64, 128);
	titleObject->SetParent(menuGraphicsObj.get(), false);
	scene.Add(std::move(titleObject));

	auto digDugIconObject = std::make_unique<Twengine::GameObject>();
	digDugIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/DigDugIcon.png");
	digDugIconObject->GetTransform()->SetLocalPosition(32, 320);
	digDugIconObject->SetParent(menuGraphicsObj.get(), false);
	scene.Add(std::move(digDugIconObject));

	auto pookaIconObject = std::make_unique<Twengine::GameObject>();
	pookaIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/PookaIcon.png");
	pookaIconObject->GetTransform()->SetLocalPosition(240, 384);
	pookaIconObject->SetParent(menuGraphicsObj.get(), false);
	scene.Add(std::move(pookaIconObject));

	auto fygarIconObject = std::make_unique<Twengine::GameObject>();
	fygarIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/FygarIcon.png");
	fygarIconObject->GetTransform()->SetLocalPosition(304, 320);
	fygarIconObject->SetParent(menuGraphicsObj.get(), false);
	scene.Add(std::move(fygarIconObject));

	auto menuObject = std::make_unique<Twengine::GameObject>();
	menuObject->AddComponent<MainMenuComponent>();
	menuObject->SetParent(menuGraphicsObj.get(), false);

	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("MainMenu"));

	auto soundMuteObj = std::make_unique<Twengine::GameObject>();
	Twengine::InputManager::GetInstance().BindCommandToInput<SoundMuteCommand>(SDLK_F2, Twengine::InteractionStates::up, soundMuteObj.get(), -1);
	scene.Add(std::move(soundMuteObj));

	Twengine::InputManager::GetInstance().BindCommandToInput<ButtonSwitchCommand>(SDLK_DOWN, Twengine::InteractionStates::down, menuObject.get(), -1)->SetDirection(1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ButtonSwitchCommand>(SDLK_UP, Twengine::InteractionStates::down, menuObject.get(), -1)->SetDirection(-1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ButtonPressCommand>(SDLK_SPACE, Twengine::InteractionStates::down, menuObject.get(), -1);

	auto highScoreDisplayerObj = std::make_unique<Twengine::GameObject>();
	highScoreDisplayerObj->AddComponent<HighScoresDisplayComponent>();
	scene.Add(std::move(highScoreDisplayerObj));

	scene.Add(std::move(menuObject));
	scene.Add(std::move(menuGraphicsObj));
}

void LevelFactory::LoadPersistentScene()
{
	GameManager::GetInstance().ClearPlayerTransforms();

	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetPersistentScene();

	///// LEVEL /////////////////////////////////////////////////////////////////////////
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	GameManager::GetInstance().SetGround(ground);

	auto scoreFileObj = std::make_unique<Twengine::GameObject>();
	auto* scoreFileComponent = scoreFileObj->AddComponent<ScoreFileComponent>();
	GameManager::GetInstance().SetScoreFileComponent(scoreFileComponent);

	scene.Add(std::move(levelDrawObject));
	scene.Add(std::move(gridObject));
	scene.Add(std::move(scoreFileObj));
	///// LEVEL /////////////////////////////////////////////////////////////////////////

	auto* smallFont = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 8);

	///// SCORE /////////////////////////////////////////////////////////////////////////
	auto digdugScoreText = std::make_unique<Twengine::GameObject>();
	digdugScoreText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
	auto* digdugPointsDisplayComp = digdugScoreText->AddComponent<DisplayPointsComponent>();
	digdugScoreText->GetTransform()->SetLocalPosition(128, 5);

	auto scoreObj = std::make_unique<Twengine::GameObject>();
	auto* scoreComponent = scoreObj->AddComponent<ScoreComponent>();
	scoreComponent->GetScoreChangedEvent()->AddObserver(digdugPointsDisplayComp);
	GameManager::GetInstance().SetScoreComponent(scoreComponent);

	scene.Add(std::move(digdugScoreText));
	scene.Add(std::move(scoreObj));
	///// SCORE /////////////////////////////////////////////////////////////////////////

	///// PLAYER 1 /////////////////////////////////////////////////////////////////////////
	auto digdugLivesText = std::make_unique<Twengine::GameObject>();
	auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();
	digDugLivesDisplayComp->GetOwner()->GetTransform()->SetLocalPosition(grid->GetPositionFromIndex(grid->GetRows() - 1, 0));

	auto digdug = std::make_unique<Twengine::GameObject>();
	GameManager::GetInstance().AddPlayerTransform(digdug->GetTransform());

	auto* diDugHealth = digdug->AddComponent<LivesComponent>();
	diDugHealth->GetObjectDiedEvent()->AddObserver(digDugLivesDisplayComp);
	scene.Add(std::move(digdugLivesText));

	digdug->AddComponent<DigDugComponent>();

	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("Game"));
	SetupKeyboardCommands(digdug.get());
	//SetupControllerCommands(digdug.get(), 0);
	scene.Add(std::move(digdug));
	///// PLAYER 1 /////////////////////////////////////////////////////////////////////////


	switch (GameManager::GetInstance().CurrentGameMode())
	{
	case GameMode::Coop:
	{
		///// PLAYER 2 /////////////////////////////////////////////////////////////////////////

		auto secondDigdugLivesText = std::make_unique<Twengine::GameObject>();
		auto* secondDigDugLivesDisplayComp = secondDigdugLivesText->AddComponent<DisplayLivesComponent>();
		secondDigdugLivesText->GetTransform()->SetLocalPosition(grid->GetPositionFromIndex(grid->GetRows() - 1, grid->GetColumns() - 3));

		auto secondDigDug = std::make_unique<Twengine::GameObject>();
		GameManager::GetInstance().AddPlayerTransform(secondDigDug->GetTransform());

		auto* secondDigDugHealth = secondDigDug->AddComponent<LivesComponent>();
		secondDigDugHealth->GetObjectDiedEvent()->AddObserver(secondDigDugLivesDisplayComp);

		secondDigDug->AddComponent<DigDugComponent>();
		SetupControllerCommands(secondDigDug.get(), 0);
		///// PLAYER 2 /////////////////////////////////////////////////////////////////////////

		scene.Add(std::move(secondDigDug));
		scene.Add(std::move(secondDigdugLivesText));
	}
	break;
	case GameMode::Versus:
	{
		///// PLAYER CONTROLLED ENEMY /////////////////////////////////////////////////////////////////////////
		std::unique_ptr<Twengine::GameObject> fygar = std::make_unique<Twengine::GameObject>();
		fygar->AddComponent<Twengine::AnimationComponent>();
		fygar->AddComponent<EnemyMovementComponent>();
		fygar->GetTransform()->SetLocalPosition(grid->GetPositionFromIndex(9, 7));
		fygar->AddComponent<Twengine::RectColliderComponent>();
		FygarComponent* fygarComp = fygar->AddComponent<FygarComponent>();
		fygarComp->EnablePlayerControlled();

		fygarComp->GetOnDeathEvent()->AddObserver(GameManager::GetInstance().GetScoreComponent());
		fygarComp->GetOnDeathEvent()->AddObserver(&GameManager::GetInstance());

		scene.Add(std::move(fygar));
		///// PLAYER CONTROLLED ENEMY /////////////////////////////////////////////////////////////////////////
	}
	break;
	}

}

void LevelFactory::LoadLevel1()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	Twengine::ServiceLocator::get_sound_system().RequestStopAllSound();
	Twengine::ServiceLocator::get_sound_system().RequestLoadMusic("Level/LevelMusic.wav", make_sdbm_hash("LevelMusic"));
	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic(make_sdbm_hash("LevelMusic"), 0.1f);

	GameManager::GetInstance().ResetEnemyCount();

	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("Game"));
	auto commandsObj = std::make_unique<Twengine::GameObject>();
	Twengine::InputManager::GetInstance().BindCommandToInput<SoundMuteCommand>(SDLK_F2, Twengine::InteractionStates::up, commandsObj.get(), -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<GoToNextLevelCommand>(SDLK_F1, Twengine::InteractionStates::up, commandsObj.get(), -1);
	scene.Add(std::move(commandsObj));
	LoadLevelFromFile(scene, GameManager::GetInstance().GetGround(), GameManager::GetInstance().GetGrid(), "Level/Level1.bin");
}

void LevelFactory::LoadLevel2()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	Twengine::ServiceLocator::get_sound_system().RequestStopAllSound();
	Twengine::ServiceLocator::get_sound_system().RequestLoadMusic("Level/LevelMusic.wav", make_sdbm_hash("LevelMusic"));
	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic(make_sdbm_hash("LevelMusic"), 0.1f);

	GameManager::GetInstance().ResetEnemyCount();
	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("Game"));
	auto commandsObj = std::make_unique<Twengine::GameObject>();
	Twengine::InputManager::GetInstance().BindCommandToInput<SoundMuteCommand>(SDLK_F2, Twengine::InteractionStates::up, commandsObj.get(), -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<GoToNextLevelCommand>(SDLK_F1, Twengine::InteractionStates::up, commandsObj.get(), -1);
	scene.Add(std::move(commandsObj));
	LoadLevelFromFile(scene, GameManager::GetInstance().GetGround(), GameManager::GetInstance().GetGrid(), "Level/Level2.bin");
}

void LevelFactory::LoadLevel3()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	Twengine::ServiceLocator::get_sound_system().RequestStopAllSound();
	Twengine::ServiceLocator::get_sound_system().RequestLoadMusic("Level/LevelMusic.wav", make_sdbm_hash("LevelMusic"));
	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic(make_sdbm_hash("LevelMusic"), 0.1f);

	GameManager::GetInstance().ResetEnemyCount();
	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("Game"));
	auto commandsObj = std::make_unique<Twengine::GameObject>();
	Twengine::InputManager::GetInstance().BindCommandToInput<SoundMuteCommand>(SDLK_F2, Twengine::InteractionStates::up, commandsObj.get(), -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<GoToNextLevelCommand>(SDLK_F1, Twengine::InteractionStates::up, commandsObj.get(), -1);
	scene.Add(std::move(commandsObj));
	LoadLevelFromFile(scene, GameManager::GetInstance().GetGround(), GameManager::GetInstance().GetGrid(), "Level/Level3.bin");
}

void LevelFactory::LoadHighScoreScene()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	Twengine::ServiceLocator::get_sound_system().RequestStopAllSound();
	Twengine::ServiceLocator::get_sound_system().RequestLoadMusic("HighScoreScreen/HighScoreScreenMusic.wav", make_sdbm_hash("HighScoreScreenMusic"));
	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic(make_sdbm_hash("HighScoreScreenMusic"), 0.1f);

	auto scoreSaveObj = std::make_unique<Twengine::GameObject>();
	scoreSaveObj->AddComponent<ScoreSavingComponent>();

	Twengine::InputManager::GetInstance().SetCommandMap(make_sdbm_hash("ScoreSaving"));
	auto soundMuteObj = std::make_unique<Twengine::GameObject>();
	Twengine::InputManager::GetInstance().BindCommandToInput<SoundMuteCommand>(SDLK_F2, Twengine::InteractionStates::up, soundMuteObj.get(), -1);
	scene.Add(std::move(soundMuteObj));
	Twengine::InputManager::GetInstance().BindCommandToInput<ScoreCharCycleCommand>(SDLK_DOWN, Twengine::InteractionStates::down, scoreSaveObj.get(), -1)->SetDirection(1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ScoreCharCycleCommand>(SDLK_UP, Twengine::InteractionStates::down, scoreSaveObj.get(), -1)->SetDirection(-1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ScoreLetterSwitchCommand>(SDLK_RIGHT, Twengine::InteractionStates::down, scoreSaveObj.get(), -1)->SetDirection(1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ScoreLetterSwitchCommand>(SDLK_LEFT, Twengine::InteractionStates::down, scoreSaveObj.get(), -1)->SetDirection(-1);
	Twengine::InputManager::GetInstance().BindCommandToInput<ScoreConfirmSaveCommand>(SDLK_SPACE, Twengine::InteractionStates::up, scoreSaveObj.get(), -1);
	scene.Add(std::move(scoreSaveObj));

	auto highScoreDisplayerObj = std::make_unique<Twengine::GameObject>();
	highScoreDisplayerObj->AddComponent<HighScoresDisplayComponent>()->ChangeBaseDrawPosition({ 20.f, 250.f });
	scene.Add(std::move(highScoreDisplayerObj));

}

void LevelFactory::SetupKeyboardCommands(Twengine::GameObject* object)
{
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::pressed, object, -1)->SetDirection(0, -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::pressed, object, -1)->SetDirection(0, 1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::pressed, object, -1)->SetDirection(-1, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::pressed, object, -1)->SetDirection(1, 0);

	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::up, object, -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::up, object, -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::up, object, -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::up, object, -1)->SetDirection(0, 0);

	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::pressed, object, -1)->IsPressBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::down, object, -1)->IsDownBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::up, object, -1)->IsReleaseBound();
}

void LevelFactory::SetupControllerCommands(Twengine::GameObject* object, int controllerIdx)
{
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0001, Twengine::InteractionStates::pressed, object, controllerIdx)->SetDirection(0, -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0002, Twengine::InteractionStates::pressed, object, controllerIdx)->SetDirection(0, 1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0004, Twengine::InteractionStates::pressed, object, controllerIdx)->SetDirection(-1, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0008, Twengine::InteractionStates::pressed, object, controllerIdx)->SetDirection(1, 0);

	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0001, Twengine::InteractionStates::up, object, controllerIdx)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0002, Twengine::InteractionStates::up, object, controllerIdx)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0004, Twengine::InteractionStates::up, object, controllerIdx)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0008, Twengine::InteractionStates::up, object, controllerIdx)->SetDirection(0, 0);

	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(0x4000, Twengine::InteractionStates::pressed, object, controllerIdx)->IsPressBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(0x4000, Twengine::InteractionStates::down, object, controllerIdx)->IsDownBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(0x4000, Twengine::InteractionStates::up, object, controllerIdx)->IsReleaseBound();
}

void LevelFactory::LoadLevelFromFile(Twengine::Scene& scene, GroundComponent* groundComponent, GridComponent* gridComponent, const std::string& filePath)
{
	std::ifstream input;
	input.open(filePath, std::ios::binary);
	if (input.is_open())
	{
		// Reading and erasing parts that need to be erased
		uint32_t amountOfRects;
		input.read(reinterpret_cast<char*>(&amountOfRects), sizeof(amountOfRects));
		std::vector<SDL_Rect> dugRects(amountOfRects);
		for (SDL_Rect& rect : dugRects)
		{
			input.read(reinterpret_cast<char*>(&rect.x), sizeof(rect.x));
			input.read(reinterpret_cast<char*>(&rect.y), sizeof(rect.y));
			input.read(reinterpret_cast<char*>(&rect.w), sizeof(rect.w));
			input.read(reinterpret_cast<char*>(&rect.h), sizeof(rect.h));
			groundComponent->ErasePlayerTrail(rect, false);
		}

		// Reading and spawning Pookas
		uint32_t amountOfPookas;
		input.read(reinterpret_cast<char*>(&amountOfPookas), sizeof(amountOfPookas));
		std::vector<std::pair<int, int>> pookaIndices(amountOfPookas);
		for (std::pair<int, int> indices : pookaIndices)
		{
			input.read(reinterpret_cast<char*>(&indices.first), sizeof(indices.first));
			input.read(reinterpret_cast<char*>(&indices.second), sizeof(indices.second));
			CreateAndAddPooka(scene, indices.first, indices.second, gridComponent);
			GameManager::GetInstance().IncreaseEnemyCount();
		}

		// Reading and spawning Fygars
		uint32_t amountOfFygars;
		input.read(reinterpret_cast<char*>(&amountOfFygars), sizeof(amountOfFygars));
		std::vector<std::pair<int, int>> fygarIndices(amountOfFygars);
		for (std::pair<int, int> indices : fygarIndices)
		{
			input.read(reinterpret_cast<char*>(&indices.first), sizeof(indices.first));
			input.read(reinterpret_cast<char*>(&indices.second), sizeof(indices.second));
			CreateAndAddFygar(scene, indices.first, indices.second, gridComponent);
			GameManager::GetInstance().IncreaseEnemyCount();
		}

		// Reading and spawning Rocks
		uint32_t amountOfRocks;
		input.read(reinterpret_cast<char*>(&amountOfRocks), sizeof(amountOfRocks));
		std::vector<std::pair<int, int>> rockIndices(amountOfRocks);
		for (std::pair<int, int> indices : rockIndices)
		{
			input.read(reinterpret_cast<char*>(&indices.first), sizeof(indices.first));
			input.read(reinterpret_cast<char*>(&indices.second), sizeof(indices.second));
			CreateAndAddRock(scene, indices.first, indices.second, gridComponent);
		}
	}
}

void LevelFactory::CreateAndAddPooka(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> pooka = std::make_unique<Twengine::GameObject>();
	pooka->AddComponent<Twengine::AnimationComponent>();
	pooka->AddComponent<EnemyMovementComponent>();
	pooka->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	pooka->AddComponent<Twengine::RectColliderComponent>();
	PookaComponent* pookaComp = pooka->AddComponent<PookaComponent>();
	pookaComp->GetOnDeathEvent()->AddObserver(GameManager::GetInstance().GetScoreComponent());
	pookaComp->GetOnDeathEvent()->AddObserver(&GameManager::GetInstance());
	scene.Add(std::move(pooka));
}

void LevelFactory::CreateAndAddFygar(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> fygar = std::make_unique<Twengine::GameObject>();
	fygar->AddComponent<Twengine::AnimationComponent>();
	fygar->AddComponent<EnemyMovementComponent>();
	fygar->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	fygar->AddComponent<Twengine::RectColliderComponent>();
	FygarComponent* fygarComp = fygar->AddComponent<FygarComponent>();
	fygarComp->GetOnDeathEvent()->AddObserver(GameManager::GetInstance().GetScoreComponent());
	fygarComp->GetOnDeathEvent()->AddObserver(&GameManager::GetInstance());
	scene.Add(std::move(fygar));
}

void LevelFactory::CreateAndAddRock(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> rock = std::make_unique<Twengine::GameObject>();
	Twengine::TextureRenderComponent* texture = rock->AddComponent<Twengine::TextureRenderComponent>();
	texture->SetTexture("Level/Rock.png");
	auto pos = gridComponent->GetPositionFromIndex(row, column);
	rock->AddComponent<RockComponent>();
	rock->AddComponent<Twengine::RectColliderComponent>();
	rock->AddComponent<Twengine::AnimationComponent>();
	rock->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	gridComponent->PutRockInCell(std::pair<int, int>(row, column));
	scene.Add(std::move(rock));
}
