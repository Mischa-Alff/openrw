#pragma once
#ifndef _PROJECTILEOBJECT_HPP_
#define _PROJECTILEOBJECT_HPP_
#include <engine/GameObject.hpp>
#include <bullet/btBulletDynamicsCommon.h>

/**
 * @brief Implements weapon projectile (e.g. molotovs, RPGs etc.)
 */
class ProjectileObject : public GameObject
{
public:

	enum ProjectileType {
		Grenade,
		Molotov,
		RPG,
	};

	struct ProjectileInfo {
		ProjectileType type;
		glm::vec3 direction;
		float velocity;

		/** Time to dentonation or removal */
		float time;
	};

private:

	ProjectileInfo _info;

	btSphereShape* _shape;
	union {
		btRigidBody* _body;
	};

	void explode();

public:

	/**
	 * @brief ProjectileObject constructor
	 */
	ProjectileObject(GameWorld* world, const glm::vec3& position, const ProjectileInfo& info);

	~ProjectileObject();

	void tick(float dt);
};

#endif
