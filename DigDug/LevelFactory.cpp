#include "LevelFactory.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextureRenderComponent.h"
#include "HealthComponent.h"
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

#include "Event.h"
#include <fstream>

void LevelFactory::LoadDevScene()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	std::unique_ptr<Twengine::SDLSoundSystem> soundSystem = std::make_unique<Twengine::SDLSoundSystem>();
	Twengine::ServiceLocator::register_sound_system(std::move(soundSystem));

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	scene.Add(std::move(levelDrawObject));
	LoadLevelFromFile(scene, ground, grid, "Level/Level1.bin");
	GameManager::GetInstance().SetGround(ground);

	auto* font = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 12);

	auto fpsDisplayer = std::make_unique<Twengine::GameObject>();
	Twengine::TextComponent* fpsText = fpsDisplayer->AddComponent<Twengine::TextComponent>();
	fpsText->SetFont(font);
	fpsDisplayer->AddComponent<Twengine::FPSComponent>();
	scene.Add(std::move(fpsDisplayer));

	auto* smallFont = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 8);
	// Tutorial 
	auto enemyTutorial = std::make_unique<Twengine::GameObject>();
	auto* enemyTutorialText = enemyTutorial->AddComponent<Twengine::TextComponent>();
	enemyTutorialText->SetText("Use WASD To Move DigDug");
	enemyTutorialText->SetFont(smallFont);
	enemyTutorial->GetTransform()->SetLocalPosition(5, 20);
	scene.Add(std::move(enemyTutorial));

	auto soundTutorial = std::make_unique<Twengine::GameObject>();
	auto* soundTutorialText = soundTutorial->AddComponent<Twengine::TextComponent>();
	soundTutorialText->SetText("Press Z To Trigger A Sound");
	soundTutorialText->SetFont(smallFont);
	soundTutorial->GetTransform()->SetLocalPosition(5, 36);
	scene.Add(std::move(soundTutorial));

	// Display DigDug lives
	//auto digdugLivesText = std::make_unique<Twengine::GameObject>();
	//auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();
	//
	//// Display DigDug score
	//auto digdugScoreText = std::make_unique<Twengine::GameObject>();
	//digdugScoreText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
	//auto* digdugPointsDisplayComp = digdugScoreText->AddComponent<DisplayPointsComponent>();
	//digdugScoreText->GetTransform()->SetLocalPosition(5, 190);
	//
	//// DIGDUG
	//auto digdug = std::make_unique<Twengine::GameObject>();
	//digdug->GetTransform()->SetLocalPosition(200, 350);
	//GameManager::GetInstance().SetPlayerTransform(digdug->GetTransform());
	//
	//auto* diDugHealth = digdug->AddComponent<HealthComponent>();
	//diDugHealth->GetObjectDiedEvent()->AddObserver(digDugLivesDisplayComp);
	//diDugHealth->SetMaxLives(3);
	//
	//auto* digDugScore = digdug->AddComponent<ScoreComponent>();
	//digDugScore->GetScoreChangedEvent()->AddObserver(digdugPointsDisplayComp);
	//
	//digdug->AddComponent<DigDugComponent>();


	//Twengine::InputManager::GetInstance().BindJoystickCommandToInput<JoystickMoveCommand>(Twengine::InteractionStates::pressed, digdug.get(), 0);
	// Bindings for keyboard
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, -1);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, 1);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(-1, 0);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(1, 0);
	////
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);

	//Twengine::InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(SDLK_c, Twengine::InteractionStates::up, digdug.get(), -1);
	//Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_z, Twengine::InteractionStates::up, digdug.get(), -1)->GetEnemyKilledEvent()->AddObserver(digDugScore);
	//Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_x, Twengine::InteractionStates::up, digdug.get(), -1);

	scene.Add(std::move(gridObject));
	//scene.Add(std::move(digdugLivesText));
	//scene.Add(std::move(digdug));
	//scene.Add(std::move(digdugScoreText));
}

void LevelFactory::LoadMainMenu()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto titleObject = std::make_unique<Twengine::GameObject>();
	titleObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/DigDugLogo.png");
	titleObject->GetTransform()->SetLocalPosition(64, 128);
	scene.Add(std::move(titleObject));

	auto digDugIconObject = std::make_unique<Twengine::GameObject>();
	digDugIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/DigDugIcon.png");
	digDugIconObject->GetTransform()->SetLocalPosition(32, 320);
	scene.Add(std::move(digDugIconObject));

	auto pookaIconObject = std::make_unique<Twengine::GameObject>();
	pookaIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/PookaIcon.png");
	pookaIconObject->GetTransform()->SetLocalPosition(240, 384);
	scene.Add(std::move(pookaIconObject));

	auto fygarIconObject = std::make_unique<Twengine::GameObject>();
	fygarIconObject->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("StartScreen/FygarIcon.png");
	fygarIconObject->GetTransform()->SetLocalPosition(304, 320);
	scene.Add(std::move(fygarIconObject));
}

void LevelFactory::LoadPersistentScene()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetPersistentScene();

	auto* smallFont = Twengine::ResourceManager::GetInstance().LoadFont("GameFont.otf", 8);

	// Display DigDug lives
	auto digdugLivesText = std::make_unique<Twengine::GameObject>();
	auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();

	// Display DigDug score
	auto digdugScoreText = std::make_unique<Twengine::GameObject>();
	digdugScoreText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
	auto* digdugPointsDisplayComp = digdugScoreText->AddComponent<DisplayPointsComponent>();
	digdugScoreText->GetTransform()->SetLocalPosition(5, 190);

	// DIGDUG
	auto digdug = std::make_unique<Twengine::GameObject>();
	digdug->GetTransform()->SetLocalPosition(200, 350);
	GameManager::GetInstance().SetPlayerTransform(digdug->GetTransform());

	auto* diDugHealth = digdug->AddComponent<HealthComponent>();
	diDugHealth->GetObjectDiedEvent()->AddObserver(digDugLivesDisplayComp);
	diDugHealth->SetMaxLives(3);

	auto* digDugScore = digdug->AddComponent<ScoreComponent>();
	digDugScore->GetScoreChangedEvent()->AddObserver(digdugPointsDisplayComp);

	digdug->AddComponent<DigDugComponent>();

	Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_z, Twengine::InteractionStates::up, digdug.get(), -1)->GetEnemyKilledEvent()->AddObserver(digDugScore);

	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, 1);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(-1, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(1, 0);
	//
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::up, digdug.get(), -1)->SetDirection(0, 0);

	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::pressed, digdug.get(), -1)->IsPressBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::down, digdug.get(), -1)->IsPressBound();
	Twengine::InputManager::GetInstance().BindCommandToInput<PumpCommand>(SDLK_SPACE, Twengine::InteractionStates::up, digdug.get(), -1)->IsReleaseBound();


	scene.Add(std::move(digdugLivesText));
	scene.Add(std::move(digdug));
	scene.Add(std::move(digdugScoreText));
}

void LevelFactory::LoadLevel1()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	LoadLevelFromFile(scene, ground, grid, "Level/Level2.bin");
	GameManager::GetInstance().SetGround(ground);
	scene.Add(std::move(levelDrawObject));
}

void LevelFactory::LoadLevel2()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	LoadLevelFromFile(scene, ground, grid, "Level/Level2.bin");
	GameManager::GetInstance().SetGround(ground);
	scene.Add(std::move(levelDrawObject));
}

void LevelFactory::LoadLevel3()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	LoadLevelFromFile(scene, ground, grid, "Level/Level3.bin");
	GameManager::GetInstance().SetGround(ground);
	scene.Add(std::move(levelDrawObject));
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
	pooka->AddComponent<PookaComponent>();
	scene.Add(std::move(pooka));
}

void LevelFactory::CreateAndAddFygar(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> fygar = std::make_unique<Twengine::GameObject>();
	fygar->AddComponent<Twengine::AnimationComponent>();
	fygar->AddComponent<EnemyMovementComponent>();
	fygar->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	fygar->AddComponent<Twengine::RectColliderComponent>();
	fygar->AddComponent<FygarComponent>();
	scene.Add(std::move(fygar));
}

void LevelFactory::CreateAndAddRock(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> rock = std::make_unique<Twengine::GameObject>();
	Twengine::TextureRenderComponent* texture = rock->AddComponent<Twengine::TextureRenderComponent>();
	texture->SetTexture("Level/Rock.png");
	auto pos = gridComponent->GetPositionFromIndex(row, column);
	rock->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	scene.Add(std::move(rock));
}
