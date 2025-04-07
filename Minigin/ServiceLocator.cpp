#include "ServiceLocator.h"
std::unique_ptr<Twengine::SoundSystem> Twengine::ServiceLocator::s_SoundSystem{ std::make_unique<NullSoundSystem>()};