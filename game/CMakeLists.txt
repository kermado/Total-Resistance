# Project Name.
project(Game)

# Set project include & source root directory path variables.
set(INC_ROOT ${PROJECT_SOURCE_DIR}/include)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/source)

# Include game headers.
include_directories(${INC_ROOT})

# Include engine headers.
include_directories(../libraries/include)

# Files.
set(GAME_SRCS
	${SRC_ROOT}/main.cpp

	${INC_ROOT}/LoadingScene.hpp
	${SRC_ROOT}/LoadingScene.cpp

	${INC_ROOT}/GameScene.hpp
	${SRC_ROOT}/GameScene.cpp

	${INC_ROOT}/VictoryScene.hpp
	${SRC_ROOT}/VictoryScene.cpp

	${INC_ROOT}/DefeatScene.hpp
	${SRC_ROOT}/DefeatScene.cpp

	${INC_ROOT}/Grid.hpp
	${SRC_ROOT}/Grid.cpp

	${INC_ROOT}/PlayingSurface.hpp
	${SRC_ROOT}/PlayingSurface.cpp

	${INC_ROOT}/PathFinder.hpp
	${SRC_ROOT}/PathFinder.cpp

	${INC_ROOT}/Round.hpp
	${SRC_ROOT}/Round.cpp

	${INC_ROOT}/LaserTowerFactory.hpp
	${SRC_ROOT}/LaserTowerFactory.cpp

	${INC_ROOT}/LaserFactory.hpp
	${SRC_ROOT}/LaserFactory.cpp

	${INC_ROOT}/RocketLauncherFactory.hpp
	${SRC_ROOT}/RocketLauncherFactory.cpp

	${INC_ROOT}/RocketFactory.hpp
	${SRC_ROOT}/RocketFactory.cpp

	${INC_ROOT}/MissileSiloFactory.hpp
	${SRC_ROOT}/MissileSiloFactory.cpp

	${INC_ROOT}/MissileFactory.hpp
	${SRC_ROOT}/MissileFactory.cpp

	${INC_ROOT}/WallFactory.hpp
	${SRC_ROOT}/WallFactory.cpp

	${INC_ROOT}/RangeFactory.hpp
	${SRC_ROOT}/RangeFactory.cpp

	${INC_ROOT}/ScoutFactory.hpp
	${SRC_ROOT}/ScoutFactory.cpp

	${INC_ROOT}/TankFactory.hpp
	${SRC_ROOT}/TankFactory.cpp

	${INC_ROOT}/HeavyBotFactory.hpp
	${SRC_ROOT}/HeavyBotFactory.cpp

	${INC_ROOT}/ExplosionFactory.hpp
	${SRC_ROOT}/ExplosionFactory.cpp

	${INC_ROOT}/ExhaustFactory.hpp
	${SRC_ROOT}/ExhaustFactory.cpp

	${INC_ROOT}/Attribute/Tags.hpp
	${SRC_ROOT}/Attribute/Tags.cpp

	${INC_ROOT}/Attribute/Health.hpp
	${SRC_ROOT}/Attribute/Health.cpp

	${INC_ROOT}/Attribute/Resources.hpp
	${SRC_ROOT}/Attribute/Resources.cpp

	${INC_ROOT}/Behaviour/MoveForwards.hpp
	${SRC_ROOT}/Behaviour/MoveForwards.cpp

	${INC_ROOT}/Behaviour/NavigateToCell.hpp
	${SRC_ROOT}/Behaviour/NavigateToCell.cpp

	${INC_ROOT}/Behaviour/AcquireNearestTarget.hpp
	${SRC_ROOT}/Behaviour/AcquireNearestTarget.cpp

	${INC_ROOT}/Behaviour/FaceAcquiredTarget.hpp
	${SRC_ROOT}/Behaviour/FaceAcquiredTarget.cpp

	${INC_ROOT}/Behaviour/MissileController.hpp
	${SRC_ROOT}/Behaviour/MissileController.cpp

	${INC_ROOT}/Behaviour/Projectile.hpp
	${SRC_ROOT}/Behaviour/Projectile.cpp

	${INC_ROOT}/Behaviour/SplashProjectile.hpp
	${SRC_ROOT}/Behaviour/SplashProjectile.cpp

	${INC_ROOT}/Behaviour/FireLaserAtAcquiredTarget.hpp
	${SRC_ROOT}/Behaviour/FireLaserAtAcquiredTarget.cpp

	${INC_ROOT}/Behaviour/FireRocketsAtAcquiredTarget.hpp
	${SRC_ROOT}/Behaviour/FireRocketsAtAcquiredTarget.cpp

	${INC_ROOT}/Behaviour/FireMissileAtAcquiredTarget.hpp
	${SRC_ROOT}/Behaviour/FireMissileAtAcquiredTarget.cpp

	${INC_ROOT}/Behaviour/ParticleAffector.hpp
	${SRC_ROOT}/Behaviour/ParticleAffector.cpp

	${INC_ROOT}/Behaviour/ToggleRange.hpp
	${SRC_ROOT}/Behaviour/ToggleRange.cpp

	${INC_ROOT}/Behaviour/Animate.hpp
	${SRC_ROOT}/Behaviour/Animate.cpp

	${INC_ROOT}/Behaviour/Animate.hpp
	${SRC_ROOT}/Behaviour/Animate.cpp

	${INC_ROOT}/Event/PlayingSurfaceModifiedEvent.hpp
	${SRC_ROOT}/Event/PlayingSurfaceModifiedEvent.cpp

	${INC_ROOT}/Event/TargetAcquiredEvent.hpp
	${SRC_ROOT}/Event/TargetAcquiredEvent.cpp

	${INC_ROOT}/Event/UpdateDestinationEvent.hpp
	${SRC_ROOT}/Event/UpdateDestinationEvent.cpp

	${INC_ROOT}/Event/AddTagEvent.hpp
	${SRC_ROOT}/Event/AddTagEvent.cpp

	${INC_ROOT}/Event/RemoveTagEvent.hpp
	${SRC_ROOT}/Event/RemoveTagEvent.cpp

	${INC_ROOT}/Event/ToggleRangeEvent.hpp
	${SRC_ROOT}/Event/ToggleRangeEvent.cpp

	${INC_ROOT}/Event/InflictDamageEvent.hpp
	${SRC_ROOT}/Event/InflictDamageEvent.cpp

	${INC_ROOT}/Event/EnemyDestroyedEvent.hpp
	${SRC_ROOT}/Event/EnemyDestroyedEvent.cpp

	${INC_ROOT}/Event/EnemySurvivedEvent.hpp
	${SRC_ROOT}/Event/EnemySurvivedEvent.cpp
)

# Create executable.
add_executable(${PROJECT_NAME} ${GAME_SRCS})

# Link the Engine library with the executable.
target_link_libraries(${PROJECT_NAME} Engine)

# Install the Game executable.
install(TARGETS ${PROJECT_NAME} DESTINATION .)
