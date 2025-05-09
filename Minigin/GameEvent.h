#pragma once
#include "SdbmHash.h" // Everything that includes GameEvent.h will also need SdbmHash.h, so just include it here for ease of access
using EventId = unsigned int;

struct GameEvent
{
    const EventId id;

    explicit GameEvent(EventId _id) : id{ _id } {}
};