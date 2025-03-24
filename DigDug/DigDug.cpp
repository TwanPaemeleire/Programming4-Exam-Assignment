#if _DEBUG
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
#include "ScoreComponent.h"
#include "DisplayPointsComponent.h"
#include "AnimatedTextureComponent.h"
#include "GameObject.h"
#include "Scene.h"

#include "InputManager.h"
#include "MoveCommand.h"
#include "KillObjectCommand.h"
#include "KillEnemyCommand.h"

#include "Event.h"

void load()
{
auto& scene = Twengine::SceneManager::GetInstance().CreateScene("Demo");

auto go = std::make_unique<Twengine::GameObject>();
go->AddComponent<Twengine::TextureRenderComponent>();
go->GetComponent<Twengine::TextureRenderComponent>()->SetTexture("background.tga");
scene.Add(std::move(go));

auto logo = std::make_unique<Twengine::GameObject>();
logo->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("logo.tga");
logo->GetTransform()->SetLocalPosition(216, 180);
scene.Add(std::move(logo));

auto* font = Twengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
auto to = std::make_unique<Twengine::GameObject>();
Twengine::TextComponent* text = to->AddComponent<Twengine::TextComponent>();
text->SetText("Programming 4 Assignment");
text->SetFont(font);
to->GetTransform()->SetLocalPosition(80, 20);
scene.Add(std::move(to));

auto fpsDisplayer = std::make_unique<Twengine::GameObject>();
Twengine::TextComponent* fpsText = fpsDisplayer->AddComponent<Twengine::TextComponent>();
fpsText->SetFont(font);
fpsDisplayer->AddComponent<Twengine::FPSComponent>();
scene.Add(std::move(fpsDisplayer));


auto* smallFont = Twengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
// Tutorial 
auto enemyTutorial = std::make_unique<Twengine::GameObject>();
auto* enemyTutorialText = enemyTutorial->AddComponent<Twengine::TextComponent>();
enemyTutorialText->SetText("Use The D-Pad To Move Enemy, X To Inflict Damage, A & B To Increase Your Score");
enemyTutorialText->SetFont(smallFont);
enemyTutorial->GetTransform()->SetLocalPosition(5, 100);
scene.Add(std::move(enemyTutorial));

auto digDugTutorial = std::make_unique<Twengine::GameObject>();
auto* digDugTutorialText = digDugTutorial->AddComponent<Twengine::TextComponent>();
digDugTutorialText->SetText("Use WASD To Move DigDug, C To Inflict Damage, Z & X To Increase Your Score");
digDugTutorialText->SetFont(smallFont);
digDugTutorial->GetTransform()->SetLocalPosition(5, 120);
scene.Add(std::move(digDugTutorial));


// Display DigDug Lives
auto digdugLivesText = std::make_unique<Twengine::GameObject>();
digdugLivesText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
auto* digDugLivesDisplayComp = digdugLivesText->AddComponent<DisplayLivesComponent>();
digdugLivesText->GetTransform()->SetLocalPosition(5, 170);

// Display DigDug Score
auto digdugScoreText = std::make_unique<Twengine::GameObject>();
digdugScoreText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
auto* digdugPointsDisplayComp = digdugScoreText->AddComponent<DisplayPointsComponent>();
digdugScoreText->GetTransform()->SetLocalPosition(5, 190);

// DIGDUG
auto digdug = std::make_unique<Twengine::GameObject>();
digdug->GetTransform()->SetLocalPosition(200, 350);
digdug->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("digdug.png");

auto* diDugHealth = digdug->AddComponent<HealthComponent>();
diDugHealth->GetObjectDiedEvent()->AddObserver(digDugLivesDisplayComp);
diDugHealth->SetMaxLives(3);

auto* digDugScore = digdug->AddComponent<ScoreComponent>();
digDugScore->GetScoreChangedEvent()->AddObserver(digdugPointsDisplayComp);



// Display Enemy Lives
auto enemyLivesText = std::make_unique<Twengine::GameObject>();
enemyLivesText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
auto* enemyLivesDisplayComp = enemyLivesText->AddComponent<DisplayLivesComponent>();
enemyLivesText->GetTransform()->SetLocalPosition(5, 210);

// Display Enemy Score
auto enemyScoreText = std::make_unique<Twengine::GameObject>();
enemyScoreText->AddComponent<Twengine::TextComponent>()->SetFont(smallFont);
auto* enemyPointsDisplayComp = enemyScoreText->AddComponent<DisplayPointsComponent>();
enemyScoreText->GetTransform()->SetLocalPosition(5, 230);

// ENEMY
auto enemy = std::make_unique<Twengine::GameObject>();
enemy->GetTransform()->SetLocalPosition(300, 350);
enemy->AddComponent<Twengine::TextureRenderComponent>()->SetTexture("enemy.png");

auto* enemyHealth = enemy->AddComponent<HealthComponent>();
enemyHealth->GetObjectDiedEvent()->AddObserver(enemyLivesDisplayComp);
enemyHealth->SetMaxLives(3);

auto* enemyScore = enemy->AddComponent<ScoreComponent>();
enemyScore->GetScoreChangedEvent()->AddObserver(enemyPointsDisplayComp);

// Bindings For Controller
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0001, Twengine::InteractionStates::pressed, enemy.get(), 0)->SetDirection(0, -50);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0002, Twengine::InteractionStates::pressed, enemy.get(), 0)->SetDirection(0, 50);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0004, Twengine::InteractionStates::pressed, enemy.get(), 0)->SetDirection(-50, 0);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(0x0008, Twengine::InteractionStates::pressed, enemy.get(), 0)->SetDirection(50, 0);
Twengine::InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(0x4000, Twengine::InteractionStates::up, enemy.get(), 0);
Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(0x1000, Twengine::InteractionStates::up, enemy.get(), 0);
Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(0x2000, Twengine::InteractionStates::up, enemy.get(), 0);

// Bindings For Keyboard
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_w, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, -100);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_s, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(0, 100);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_a, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(-100, 0);
Twengine::InputManager::GetInstance().BindCommandToInput<MoveCommand>(SDLK_d, Twengine::InteractionStates::pressed, digdug.get(), -1)->SetDirection(100, 0);
Twengine::InputManager::GetInstance().BindCommandToInput<KillObjectCommand>(SDLK_c, Twengine::InteractionStates::up, digdug.get(), -1);
Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_z, Twengine::InteractionStates::up, digdug.get(), -1)->GetEnemyKilledEvent()->AddObserver(digDugScore);
Twengine::InputManager::GetInstance().BindCommandToInput<KillEnemyCommand>(SDLK_x, Twengine::InteractionStates::up, digdug.get(), -1);

scene.Add(std::move(digdug));
scene.Add(std::move(digdugLivesText));
scene.Add(std::move(digdugScoreText));

scene.Add(std::move(enemy));
scene.Add(std::move(enemyLivesText));
scene.Add(std::move(enemyScoreText));

auto animatedObject = std::make_unique<Twengine::GameObject>();
animatedObject->GetTransform()->SetLocalPosition(300, 400);
Twengine::AnimatedTextureComponent* animator = animatedObject->AddComponent<Twengine::AnimatedTextureComponent>();
animator->InitializeSpriteSheet("playerSheet.png", 11, 11);
animator->PlayAnimation(3, 11, 0.2f, 2);
scene.Add(std::move(animatedObject));
}

int main(int, char* [])
{
	Twengine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}