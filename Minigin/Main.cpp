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
#include "ThrashTheCacheComponent.h"
#include "GameObject.h"
#include "Scene.h"

#include "InputManager.h"
#include "MoveCommands.h"

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

	auto digdug = std::make_unique<GameObject>();
	digdug->GetTransform()->SetLocalPosition(200, 350);
	digdug->AddComponent<TextureRenderComponent>()->SetTexture("digdug.png");

	//auto digdugRotationObject = std::make_unique<GameObject>();
	//RotationAroundPointComponent* rotation = digdugRotationObject->AddComponent<RotationAroundPointComponent>();
	//digdugRotationObject->AddComponent<TextureRenderComponent>()->SetTexture("digdug.png");
	//rotation->SetSpeed(5.f);
	//rotation->SetRadius(25.f);
	//digdugRotationObject->SetParent(digdug.get(), false);

	auto enemy = std::make_unique<GameObject>();
	enemy->AddComponent<TextureRenderComponent>()->SetTexture("enemy.png");
	enemy->GetTransform()->SetLocalPosition(250, 350);

	// Bindings For Controller
	InputManager::GetInstance().BindCommandToInput<MoveUpCommand>(0x0001, InteractionStates::pressed, enemy.get(), 0)->SetSpeed(50);
	InputManager::GetInstance().BindCommandToInput<MoveDownCommand>(0x0002, InteractionStates::pressed, enemy.get(), 0)->SetSpeed(50.f);
	InputManager::GetInstance().BindCommandToInput<MoveLeftCommand>(0x0004, InteractionStates::pressed, enemy.get(), 0)->SetSpeed(50.f);
	InputManager::GetInstance().BindCommandToInput<MoveRightCommand>(0x0008, InteractionStates::pressed, enemy.get(), 0)->SetSpeed(50.f);

	// Bindings For Keyboard
	InputManager::GetInstance().BindCommandToInput<MoveUpCommand>(SDLK_w, InteractionStates::pressed, digdug.get(), -1)->SetSpeed(100.f);
	InputManager::GetInstance().BindCommandToInput<MoveDownCommand>(SDLK_s, InteractionStates::pressed, digdug.get(), -1)->SetSpeed(100.f);
	InputManager::GetInstance().BindCommandToInput<MoveLeftCommand>(SDLK_a, InteractionStates::pressed, digdug.get(), -1)->SetSpeed(100.f);
	InputManager::GetInstance().BindCommandToInput<MoveRightCommand>(SDLK_d, InteractionStates::pressed, digdug.get(), -1)->SetSpeed(100.f);

	//RotationAroundPointComponent* enemyRotation = enemy->AddComponent<RotationAroundPointComponent>();
	//enemyRotation->SetSpeed(20.f);
	//enemyRotation->SetRadius(35.f);
	//enemy->SetParent(digdugRotationObject.get(), false);

	scene.Add(std::move(digdug));
	scene.Add(std::move(enemy));
	//scene.Add(std::move(digdugRotationObject));

	//auto trashTheCache = std::make_unique<GameObject>();
	//trashTheCache->AddComponent<ThrashTheCacheComponent>();
	//scene.Add(std::move(trashTheCache));
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}