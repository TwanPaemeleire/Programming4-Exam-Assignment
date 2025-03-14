#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextureRenderComponent.h"
#include "RotationAroundPointComponent.h"
#include "HealthComponent.h"
#include "DisplayLivesComponent.h"
#include "PointComponent.h"
#include "DisplayPointsComponent.h"
#include "GameObject.h"
#include "Scene.h"

#include "InputManager.h"
#include "MoveCommand.h"
#include "KillObjectCommand.h"
#include "IncreaseScoreCommand.h"

#include "Event.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<GameObject>();
	go->AddComponent<TextureRenderComponent>();
	go->GetComponent<TextureRenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(go));

	auto logo = std::make_unique<GameObject>();
	logo->AddComponent<TextureRenderComponent>()->SetTexture("logo.tga");
	logo->GetTransform()->SetLocalPosition(216, 180);
	scene.Add(std::move(logo));

	auto* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<GameObject>();
	TextComponent* text = to->AddComponent<TextComponent>();
	text->SetText("Programming 4 Assignment");
	text->SetFont(font);
	to->GetTransform()->SetLocalPosition(80, 20);
	scene.Add(std::move(to));

	auto fpsDisplayer = std::make_unique<GameObject>();
	TextComponent* fpsText = fpsDisplayer->AddComponent<TextComponent>();
	fpsText->SetFont(font);
	fpsDisplayer->AddComponent<FPSComponent>();
	scene.Add(std::move(fpsDisplayer));


	auto* smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	// Tutorial 
	auto enemyTutorial = std::make_unique<GameObject>();
	auto* enemyTutorialText = enemyTutorial->AddComponent<TextComponent>();
	enemyTutorialText->SetText("Use The D-Pad To Move Enemy, X To Inflict Damage, A & B To Increase Your Score");
	enemyTutorialText->SetFont(smallFont);
	enemyTutorial->GetTransform()->SetLocalPosition(5, 100);
	scene.Add(std::move(enemyTutorial));

	auto digDugTutorial = std::make_unique<GameObject>();
	auto* digDugTutorialText = digDugTutorial->AddComponent<TextComponent>();
	digDugTutorialText->SetText("Use WASD To Move DigDug, C To Inflict Damage, Z & X To Increase Your Score");
	digDugTutorialText->SetFont(smallFont);
	digDugTutorial->GetTransform()->SetLocalPosition(5, 120);
	scene.Add(std::move(digDugTutorial));
	

	// Display DigDug Lives
	auto digdugLivesText = std::make_unique<GameObject>();
	digdugLivesText->AddComponent<TextComponent>()->SetFont(smallFont);
	auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();
	digdugLivesText->GetTransform()->SetLocalPosition(5, 170);

	// Display DigDug Score
	auto digdugScoreText = std::make_unique<GameObject>();
	digdugScoreText->AddComponent<TextComponent>()->SetFont(smallFont);
	auto* digdugPointsDisplayComp = digdugScoreText->AddComponent<DisplayPointsComponent>();
	digdugScoreText->GetTransform()->SetLocalPosition(5, 190);

	// DIGDUG
	auto digdug = std::make_unique<GameObject>();
	digdug->GetTransform()->SetLocalPosition(200, 350);
	digdug->AddComponent<TextureRenderComponent>()->SetTexture("digdug.png");

	auto* diDugHealth = digdug->AddComponent<HealthComponent>();
	diDugHealth->GetObjectDiedEvent()->AddObserver(digDugLivesDisplayComp);
	diDugHealth->SetMaxLives(3);

	auto* digDugScore = digdug->AddComponent<PointComponent>();
	digDugScore->GetScoreChangedEvent()->AddObserver(digdugPointsDisplayComp);



	// Display Enemy Lives
	auto enemyLivesText = std::make_unique<GameObject>();
	enemyLivesText->AddComponent<TextComponent>()->SetFont(smallFont);
	auto* enemyLivesDisplayComp = enemyLivesText->AddComponent<DisplayLivesComponent>();
	enemyLivesText->GetTransform()->SetLocalPosition(5, 210);

	// Display Enemy Score
	auto enemyScoreText = std::make_unique<GameObject>();
	enemyScoreText->AddComponent<TextComponent>()->SetFont(smallFont);
	auto* enemyPointsDisplayComp = enemyScoreText->AddComponent<DisplayPointsComponent>();
	enemyScoreText->GetTransform()->SetLocalPosition(5, 230);

	// ENEMY
	auto enemy = std::make_unique<GameObject>();
	enemy->GetTransform()->SetLocalPosition(300, 350);
	enemy->AddComponent<TextureRenderComponent>()->SetTexture("enemy.png");

	auto* enemyHealth = enemy->AddComponent<HealthComponent>();
	enemyHealth->GetObjectDiedEvent()->AddObserver(enemyLivesDisplayComp);
	enemyHealth->SetMaxLives(3);

	auto* enemyScore = enemy->AddComponent<PointComponent>();
	enemyScore->GetScoreChangedEvent()->AddObserver(enemyPointsDisplayComp);

	// Bindings For Controller
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0001, InteractionStates::pressed, enemy.get(), 0)->SetDirection(0, -50);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0002, InteractionStates::pressed, enemy.get(), 0)->SetDirection(0, 50);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0004, InteractionStates::pressed, enemy.get(), 0)->SetDirection(-50, 0);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0008, InteractionStates::pressed, enemy.get(), 0)->SetDirection(50, 0);
	InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(0x4000, InteractionStates::up, enemy.get(), 0);
	InputManager::GetInstance().BindCommandToInput<IncreaseScoreCommand>(0x1000, InteractionStates::up, enemy.get(), 0)->SetPointIncrease(10);
	InputManager::GetInstance().BindCommandToInput<IncreaseScoreCommand>(0x2000, InteractionStates::up, enemy.get(), 0)->SetPointIncrease(100);

	// Bindings For Keyboard
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, -100);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, 100);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, InteractionStates::pressed, digdug.get(), -1)->SetDirection(-100, 0);
	InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, InteractionStates::pressed, digdug.get(), -1)->SetDirection(100, 0);
	InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(SDLK_c, InteractionStates::up, digdug.get(), -1);
	InputManager::GetInstance().BindCommandToInput<IncreaseScoreCommand>(SDLK_z, InteractionStates::up, digdug.get(), -1)->SetPointIncrease(10);
	InputManager::GetInstance().BindCommandToInput<IncreaseScoreCommand>(SDLK_x, InteractionStates::up, digdug.get(), -1)->SetPointIncrease(100);

	scene.Add(std::move(digdug));
	scene.Add(std::move(digdugLivesText));
	scene.Add(std::move(digdugScoreText));

	scene.Add(std::move(enemy));
	scene.Add(std::move(enemyLivesText));
	scene.Add(std::move(enemyScoreText));
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}