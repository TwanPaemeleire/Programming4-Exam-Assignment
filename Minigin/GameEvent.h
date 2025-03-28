#pragma once
#include "SdbmHash.h" // Everything That Includes Event Will Need This File, So Just Include It Here For Ease Of Access
using EventId = unsigned int;

struct GameEvent
{
    const EventId id;

    explicit GameEvent(EventId _id) : id{ _id } {}
};