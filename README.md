# Link to source control depot

https://github.com/TwanPaemeleire/Programming4-Exam-Assignment

# Engine Design

In the section below, I'll briefly cover which design choices and patterns I use in my engine, and where they've been applied.

## Component

Every game object in my game contains a list of components, this list always contains a TransFormComponent, which is responsible for storing the position and the object's parent (more about this in the SceneGraph section). This helps a lot with separating and re-using logic whenever needed and allows for tons of flexibility. For example my different types of enemies all have an EnemyMovementComponent. Which makes it so this code no longer needs to be duplicated, nor do we need a base enemy class. We can just make a component per enemy type and define the behavior there, shared behavior can then go into a different component that both enemies use.

## Game Loop and Update Method

These two patterns are fundamental to how the engine runs and go hand-in-hand. It makes it so that game objects can change values, draw things, etc. The game loop also has a FixedUpdate and a LateUpdate. FixedUpdate is useful for stuff like physics, whereas LateUpdate can be useful for things like a camera, or deleting objects that are marked for destruction.

## Command

Commands make it incredibly easy to handle user input, for example I use commands on the main menu to cycle through the buttons. Additionally using the Command pattern makes it so you have to add very little code to add the same functionality for keyboard and controller. There's no rewriting of logic, we only have to bind the command to some form of input. My engine supports both keyboard & any XINPUT compatible controller.

## Observer

The observer pattern helps in making it easy for things to react when a certain thing occurs. For example, when an enemy dies, it notifies the ScoreComponent, which will then in turn check the needed conditions (using this enemy) and grant an appropriate amount of score.

## State

The state patterns removes the need to check a ton of booleans to determine what state an object is in. Instead, we just have a current active state, this way, we know what state we're in, and can just drop any of those extra checks. I use this pattern for my player, the rocks in the level, and for both enemies.

## Singleton

The singleton pattern makes it so that an object only has 1 instance, and there's a global point of access to it. I use this pattern to have a GameManager. The GameManager can control the game, and also provides a point of access via which for example an event can be sent for the score to be increased.

## Service Locator

A service locator provides a global point of access to a so-called service, without actually coupling users to the concrete class that implements it, unlike a Singleton. This can also be used to easily swap out services. I use this pattern for my sound system, you can easily swap out the sound system with a NullSoundSystem, which will work perfectly, but not play any sounds. You can use this in case you want to, for example, test without sounds.

## Dirty Flag

The dirty flag pattern makes it possible to put off unnecessary (expensive) work until the result is needed. I use this in my engine for the TransformComponent. It makes it so the calculating of the world position (expensive) only gets done when the world position gets requested.

## Scenegraph

The scenegraph makes it so that an object can be parented to other objects, so that they can have a local position relative to their parent. As a result, you can for example move relative to your parent, or you could rotate around your parent, while they move.

## Persistent Scene

I have a persistent scene, similar to Unity's DontDestroyOnLoad scene. It's a scene that's always active and runs alongside the current scene. I use this to, for example, go to the next level, while keeping the player objects & commands alive.
