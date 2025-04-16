#include "GroundComponent.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <iostream>
#include "Renderer.h"

GroundComponent::GroundComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	m_Transform->SetLocalPosition(GameManager::GetInstance().GetGrid()->GetPositionFromIndex(2, 0));
	m_Surface = IMG_Load("Level/FullLevel.png");
	// Make Sure The Blend Mode Is Set Correctly So We Can "Erase" Parts Of The Texture
	SDL_SetSurfaceBlendMode(m_Surface, SDL_BLENDMODE_BLEND);
	m_Texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Surface);
	SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);

	m_TransparentValue = SDL_MapRGBA(m_Surface->format, 0, 0, 0, 0);
}

void GroundComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();

	SDL_Rect dst;
	SDL_QueryTexture(m_Texture, nullptr, nullptr, &dst.w, &dst.h);

	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(pos.x);
	dstRect.y = static_cast<int>(pos.y);
	dstRect.w = static_cast<int>(dst.w);
	dstRect.h = static_cast<int>(dst.h);
	SDL_RenderCopy(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Texture, nullptr, &dstRect);
}

void GroundComponent::ErasePlayerTrail(SDL_Rect playerRect, bool isInWorldSpace)
{
	SDL_Rect rectRelativeToPos = playerRect;
	if (isInWorldSpace)
	{
		// Calculate Position Relative To Where The Ground Starts, As This Won't Be The Same As Player World Position
		rectRelativeToPos.x -= static_cast<int>(m_Transform->GetWorldPosition().x);
		rectRelativeToPos.y -= static_cast<int>(m_Transform->GetWorldPosition().y);
	}

	Uint32* pixels = (Uint32*)m_Surface->pixels;

	// Gives The Number Of Bytes In A Single Row Of Pixel Data On This Surface
	int pitchInBytes = m_Surface->pitch;
	// / 4 Because Each Pixel Is 4 Bytes And We Want To Work In The 1D "pixels" Array, So This Way We Get The Number Of Pixels Per Row, Not Number Of Bytes
	int pitch = pitchInBytes / 4;

	// Set All Of The Calculated Rect's Pixels To TransParent
	for (int y = rectRelativeToPos.y; y < rectRelativeToPos.y + rectRelativeToPos.h; y++)
	{
		for (int x = rectRelativeToPos.x; x < rectRelativeToPos.x + rectRelativeToPos.w; x++)
		{
			// Make Sure We Don't Go Out Of Bounds
			if (x >= 0 && x < m_Surface->w && y >= 0 && y < m_Surface->h)
			{
				pixels[y * pitch + x] = m_TransparentValue;
			}
		}
	}
	
	// Recreate Texture From Updated Surface
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Surface);
	SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
}

bool GroundComponent::PositionIsDugOut(const glm::vec2& pos)
{
	// Calculate Position Relative To Where The Ground Starts, As This Won't Be The Same As Player World Position
	glm::vec2 relativePos = pos - glm::vec2(m_Transform->GetWorldPosition());
	int x = static_cast<int>(relativePos.x);
	int y = static_cast<int>(relativePos.y);

	// Make Sure We Don't Go Out Of Bounds
	if (x < 0 || x >= m_Surface->w || y < 0 || y >= m_Surface->h)
		return false;

	// Gives The Number Of Bytes In A Single Row Of Pixel Data On This Surface
	int pitchInBytes = m_Surface->pitch;
	// / 4 Because Each Pixel Is 4 Bytes And We Want To Work In The 1D "pixels" Array, So This Way We Get The Number Of Pixels Per Row, Not Number Of Bytes
	int pitch = pitchInBytes / 4;

	Uint32* pixels = (Uint32*)m_Surface->pixels;
	return pixels[y * pitch + x] == m_TransparentValue;
}

