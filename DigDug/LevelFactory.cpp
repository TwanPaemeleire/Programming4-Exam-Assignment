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
#include "DigDugMovementComponent.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "PookaComponent.h"

#include "Event.h"
#include <fstream>

void LevelFactory::LoadMenu()
{
	Twengine::Scene& scene = Twengine::SceneManager::GetInstance().GetCurrentScene();
	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

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
	enemyTutorialText->SetText("Use The Left Joystick To Move DigDug");
	enemyTutorialText->SetFont(smallFont);
	enemyTutorial->GetTransform()->SetLocalPosition(5, 20);
	scene.Add(std::move(enemyTutorial));

	// Display DigDug Lives
	auto digdugLivesText = std::make_unique<Twengine::GameObject>();
	auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();

	// Display DigDug Score
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
	digdug->AddComponent<DigDugMovementComponent>();

	auto emptyCheck = std::make_unique<Twengine::GameObject>();
	auto collisionCheck = std::make_unique<Twengine::GameObject>();
	auto* animationColl = collisionCheck->AddComponent<Twengine::AnimationComponent>();
	//animationColl->AddAnimation("DigDug/DigDugMove.png", 2);
	//animationColl->PlayAnimation("DigDugMove");
	auto* collider = collisionCheck->AddComponent<Twengine::RectColliderComponent>();
	collider->SetHitBox(collisionCheck->GetTransform()->GetWorldPosition(), animationColl->GetAnimationFrameWidth(), animationColl->GetAnimationFrameHeight());
	collisionCheck->SetParent(emptyCheck.get(), true);
	collisionCheck->SetTag(make_sdbm_hash("CollisionTest"));
	collisionCheck->AddComponent<DigDugMovementComponent>();

	Twengine::InputManager::GetInstance().BindJoystickCommandToInput<JoystickMoveCommand>(Twengine::InteractionStates::pressed, digdug.get(), 0);
	// Bindings For Keyboard
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::pressed, collisionCheck.get(), -1)->SetDirection(0, -1);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::pressed, collisionCheck.get(), -1)->SetDirection(0, 1);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::pressed, collisionCheck.get(), -1)->SetDirection(-1, 0);
	//Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::pressed, collisionCheck.get(), -1)->SetDirection(1, 0);
	Twengine::InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(SDLK_c, Twengine::InteractionStates::up, digdug.get(), -1);
	Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_z, Twengine::InteractionStates::up, digdug.get(), -1)->GetEnemyKilledEvent()->AddObserver(digDugScore);
	Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_x, Twengine::InteractionStates::up, digdug.get(), -1);
	//scene.Add(std::move(collisionCheck));

	digDugLivesDisplayComp->Initialize(grid);

	scene.Add(std::move(gridObject));
	scene.Add(std::move(digdugLivesText));
	scene.Add(std::move(digdug));
	scene.Add(std::move(digdugScoreText));

	std::unique_ptr<Twengine::SDLSoundSystem> soundSystem = std::make_unique<Twengine::SDLSoundSystem>();;
	Twengine::ServiceLocator::register_sound_system(std::move(soundSystem));
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
		// Reading And Erasing Parts That Need To Be Erased
		size_t amountOfRects;
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

		// Reading And Spawning Pookas
		size_t amountOfPookas;
		input.read(reinterpret_cast<char*>(&amountOfPookas), sizeof(amountOfPookas));
		std::vector<std::pair<int, int>> pookaIndeces(amountOfPookas);
		for (std::pair<int, int> indices : pookaIndeces)
		{
			input.read(reinterpret_cast<char*>(&indices.first), sizeof(indices.first));
			input.read(reinterpret_cast<char*>(&indices.second), sizeof(indices.second));
			CreateAndAddPooka(scene, indices.first, indices.second, gridComponent);
		}

		// Reading And Spawning Fygars
		size_t amountOfFygars;
		input.read(reinterpret_cast<char*>(&amountOfFygars), sizeof(amountOfFygars));
		std::vector<std::pair<int, int>> fygarIndeces(amountOfFygars);
		for (std::pair<int, int> indices : fygarIndeces)
		{
			input.read(reinterpret_cast<char*>(&indices.first), sizeof(indices.first));
			input.read(reinterpret_cast<char*>(&indices.second), sizeof(indices.second));
			CreateAndAddFygar(scene, indices.first, indices.second, gridComponent);
		}

		// Reading And Spawning Rocks
		size_t amountOfRocks;
		input.read(reinterpret_cast<char*>(&amountOfRocks), sizeof(amountOfRocks));
		std::vector<std::pair<int, int>> rockIndeces(amountOfRocks);
		for (std::pair<int, int> indices : rockIndeces)
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
	Twengine::AnimationComponent* animation = pooka->AddComponent<Twengine::AnimationComponent>();
	animation->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaMove"), 2);
	animation->PlayAnimation(make_sdbm_hash("PookaMove"));
	pooka->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
	pooka->AddComponent<PookaComponent>();
	scene.Add(std::move(pooka));
}

void LevelFactory::CreateAndAddFygar(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent)
{
	std::unique_ptr<Twengine::GameObject> fygar = std::make_unique<Twengine::GameObject>();
	Twengine::AnimationComponent* animation = fygar->AddComponent<Twengine::AnimationComponent>();
	animation->AddAnimation("Fygar/FygarMove.png", make_sdbm_hash("FygarMove"), 2);
	animation->PlayAnimation(make_sdbm_hash("FygarMove"));
	fygar->GetTransform()->SetLocalPosition(gridComponent->GetPositionFromIndex(row, column));
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
