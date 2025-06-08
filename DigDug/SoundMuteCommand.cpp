#include "SoundMuteCommand.h"
#include "ServiceLocator.h"

SoundMuteCommand::SoundMuteCommand(Twengine::GameObject* gameObject)	
	:GameObjectCommand{gameObject}
{
}

void SoundMuteCommand::Execute()
{
	int i = 0;
	i;
	if (Twengine::ServiceLocator::get_sound_system().IsMuted()) Twengine::ServiceLocator::get_sound_system().RequestUnMuteAllSound();
	else Twengine::ServiceLocator::get_sound_system().RequestMuteAllSound();
}
