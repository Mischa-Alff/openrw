#ifndef _RWGAME_HPP_
#define _RWGAME_HPP_

#include <core/Logger.hpp>
#include <engine/GameWorld.hpp>
#include <render/GameRenderer.hpp>
#include <script/ScriptMachine.hpp>
#include "game.hpp"

#include <SFML/Graphics.hpp>

class RWGame
{
	Logger log;
	GameWorld* engine;
	// must be allocated after Logger setup.
	GameRenderer* renderer;
	ScriptMachine* script;
	sf::RenderWindow window;
	sf::Clock clock;
	bool inFocus;
	ViewCamera lastCam, nextCam;
	bool showDebugStats;
	int lastDraws; /// Number of draws issued for the last frame.

	float accum;
	float timescale;
public:

	RWGame(const std::string& gamepath, int argc, char* argv[]);
	~RWGame();

	int run();

	GameWorld* getWorld() const
	{
		return engine;
	}
	
	GameRenderer* getRenderer() const
	{
		return renderer;
	}

	sf::RenderWindow& getWindow()
	{
		return window;
	}

	ScriptMachine* getScript() const
	{
		return script;
	}

	bool hitWorldRay(glm::vec3 &hit, glm::vec3 &normal, GameObject** object = nullptr)
	{
		auto vc = nextCam;
		glm::vec3 from(vc.position.x, vc.position.y, vc.position.z);
		glm::vec3 tmp = vc.rotation * glm::vec3(1000.f, 0.f, 0.f);

		return hitWorldRay(from, tmp, hit, normal, object);
	}

	bool hitWorldRay(const glm::vec3 &start, const glm::vec3 &direction, glm::vec3 &hit, glm::vec3 &normal, GameObject **object = nullptr)
	{
		auto from = btVector3(start.x, start.y, start.z);
		auto to = btVector3(start.x+direction.x, start.y+direction.y, start.z+direction.z);
		btCollisionWorld::ClosestRayResultCallback ray(from, to);

		engine->dynamicsWorld->rayTest(from, to, ray);
		if( ray.hasHit() )
		{
			hit = glm::vec3(ray.m_hitPointWorld.x(), ray.m_hitPointWorld.y(),
							ray.m_hitPointWorld.z());
			normal = glm::vec3(ray.m_hitNormalWorld.x(), ray.m_hitNormalWorld.y(),
							   ray.m_hitNormalWorld.z());
			if(object) {
				*object = static_cast<GameObject*>(ray.m_collisionObject->getUserPointer());
			}
			return true;
		}
		return false;
	}
	
	void startScript(const std::string& name);

private:
	void tick(float dt);
	void render(float alpha, float dt);
	
	void renderDebugStats(float time, GLuint worldRenderTime);

	void globalKeyEvent(const sf::Event& event);
};

#endif
