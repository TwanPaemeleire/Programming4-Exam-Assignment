#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextureRenderComponent.h"
#include "HealthComponent.h"
#include "DisplayLivesComponent.h"
#include "ScoreComponent.h"
#include "DisplayPointsComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "Scene.h"

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
#include "LevelFactory.h"
#include "GameManager.h"
#include "GroundComponent.h"

#include "Event.h"

void load()
{
	auto& scene = Twengine::SceneManager::GetInstance().CreateScene("Game");


	auto gridObject = std::make_unique<Twengine::GameObject>();
	auto* grid = gridObject->AddComponent<GridComponent>();
	GameManager::GetInstance().SetGrid(grid);

	auto levelDrawObject = std::make_unique<Twengine::GameObject>();
	auto* ground = levelDrawObject->AddComponent<GroundComponent>();
	GameManager::GetInstance().SetGround(ground);
	scene.Add(std::move(levelDrawObject));

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
	enemyTutorialText->SetText("Use The Left Joystick To Move The Second DigDug");
	enemyTutorialText->SetFont(smallFont);
	enemyTutorial->GetTransform()->SetLocalPosition(5, 100);
	scene.Add(std::move(enemyTutorial));

	auto digDugTutorial = std::make_unique<Twengine::GameObject>();
	auto* digDugTutorialText = digDugTutorial->AddComponent<Twengine::TextComponent>();
	digDugTutorialText->SetText("Use WASD To Move DigDug");
	digDugTutorialText->SetFont(smallFont);
	digDugTutorial->GetTransform()->SetLocalPosition(5, 120);
	scene.Add(std::move(digDugTutorial));


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
	//Twengine::ServiceLocator::register_sound_system(std::move(soundSystem));

	Twengine::SceneManager::GetInstance().SetCurrentScene("Game");
}

int main(int, char* [])
{
	constexpr int MAX_STEPS_UP{ 5 };
	std::filesystem::path dataFolderName{ "Data" };
	int counter{ 0 };
	while (not std::filesystem::exists(dataFolderName) and counter < MAX_STEPS_UP)
	{
		std::filesystem::current_path(".."); // If "Data" Doesn't Exist In This Current Folder, Navigate One Up In The Hierarchy
		counter++;
	}
	std::filesystem::current_path(dataFolderName); // Set The Current Path, This Way The File System Paths Will Be Relative To This One And Thus To Data (If It Found A Data Folder)

	Twengine::Minigin engine{};
	engine.Run(load);
	return 0;
}