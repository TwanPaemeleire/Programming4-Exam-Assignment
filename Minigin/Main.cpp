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
#include "GameObject.h"
#include "Scene.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<GameObject>();
	go->AddComponent(std::make_unique<TextureRenderComponent>("background.tga"));
	scene.Add(std::move(go));


	go = std::make_unique<GameObject>();
	go->AddComponent(std::make_unique<TextureRenderComponent>("logo.tga"));
	go->SetPosition(216, 180);
	scene.Add(std::move(go));

	auto* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<GameObject>();
	to->AddComponent(std::make_unique<TextComponent>("Programming 4 Assignment", font));
	to->SetPosition(80, 20);
	scene.Add(std::move(to));

	auto fpsDisplayer = std::make_unique<GameObject>();
	fpsDisplayer->AddComponent(std::make_unique<TextComponent>("FPS:", font));
	fpsDisplayer->AddComponent(std::make_unique<FPSComponent>());
	scene.Add(std::move(fpsDisplayer));
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}