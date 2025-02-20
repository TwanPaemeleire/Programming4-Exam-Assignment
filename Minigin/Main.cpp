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
#include "GameObject.h"
#include "Scene.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<GameObject>();
	go->AddComponent<TextureRenderComponent>();
	go->GetComponent<TextureRenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(go));

	auto logo = std::make_unique<GameObject>();
	logo->AddComponent<TextureRenderComponent>()->SetTexture("logo.tga");
	logo->GetTransform()->SetLocalPosition(glm::vec3{ 216, 180, 0 });
	scene.Add(std::move(logo));

	auto* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<GameObject>();
	TextComponent* text = to->AddComponent<TextComponent>();
	text->SetText("Programming 4 Assignment");
	text->SetFont(font);
	to->GetTransform()->SetLocalPosition(glm::vec3{80, 20, 0});
	scene.Add(std::move(to));

	auto fpsDisplayer = std::make_unique<GameObject>();
	TextComponent* fpsText = fpsDisplayer->AddComponent<TextComponent>();
	fpsText->SetFont(font);
	fpsDisplayer->AddComponent<FPSComponent>();
	scene.Add(std::move(fpsDisplayer));

	auto digdug = std::make_unique<GameObject>();
	digdug->AddComponent<TextureRenderComponent>()->SetTexture("digdug.png");
	digdug->GetTransform()->SetLocalPosition(glm::vec3(200, 200, 0));
	RotationAroundPointComponent* rotation = digdug->AddComponent<RotationAroundPointComponent>();
	rotation->SetRotationPoint(glm::vec3(200, 350, 0));
	rotation->SetSpeed(5.f);
	rotation->SetRadius(25.f);

	auto enemy = std::make_unique<GameObject>();
	enemy->AddComponent<TextureRenderComponent>()->SetTexture("enemy.png");
	RotationAroundPointComponent* enemyRotation = enemy->AddComponent<RotationAroundPointComponent>();
	enemyRotation->SetRotationPoint(glm::vec3(0.f, 0.f, 0.f));
	enemyRotation->SetSpeed(8.f);
	enemyRotation->SetRadius(35.f);
	enemy->SetParent(digdug.get(), false);

	scene.Add(std::move(digdug));
	scene.Add(std::move(enemy));
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}