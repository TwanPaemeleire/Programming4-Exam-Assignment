#include "GameObjectCommand.h"

class SoundMuteCommand : public Twengine::GameObjectCommand
{
public:
	SoundMuteCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
};



