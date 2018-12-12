#ifndef _cParticleEmitter_HG_
#define _cParticleEmitter_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct sParticle
{
	sParticle();
	glm::vec3 position;
	glm::vec3 velocity;
	//float lifetime;			// Second this will live
	float lifeRemaining;	// How long it's been alive
	unsigned int particleIndex;
};

class cParticleEmitter
{
public:
	cParticleEmitter();
	~cParticleEmitter();

	// EXPENSIVE operation, called when created or when
	//  we want to drastically change the parameters
	void Init( float minLifeTime, float maxLifeTime, 
			   unsigned int totalNumberOfParticles,		// 500
			   glm::vec3 position,
			   // 3 units per second   (-0.1, 0.5f, -0.1f) 
				//					   (+0.1, 1.0f, +0.1f)
			   glm::vec3 minVelocity, glm::vec3 maxVelocity, 
			   // Gravity, -ve gravity, thrust (0.0f, +0.1f, 0.0f)
			   glm::vec3 acceleration,  
			   unsigned int minInitNumParticles,		// 500	
			   unsigned int maxInitNumParticles );		// 500

	// This is less expensive, will keep the number of particles 
	//  and update the initial state.
	void Reset(void);		// BAM!

	void Update(double deltaTime);

	// note it's pass by reference...
	void getAliveParticles( std::vector<sParticle> &vecParticles );
//	std::vector<sParticle> getAliveParticles( void );
//	std::vector<sParticle> &getAliveParticles( void );


	// Public so we can "see" the particle vector
	std::vector<sParticle> vecParticles;

private:

	void m_ResetParticle(sParticle* pParticle);

	// Is checked by Update
	bool m_bIsInit;	// = false;

	// I'm putting this here so I don't have to include "globalStuff.h"
	// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
	float getRandInRange( float min, float max );
	unsigned int getRandInRange( unsigned int min, unsigned int max );
	glm::vec3 getRandInRange( glm::vec3 min, glm::vec3 max );

	glm::vec3 m_position;

	glm::vec3 m_minVelocity;
	glm::vec3 m_maxVelocity;
	glm::vec3 m_acceleration;		// Gravity?
	unsigned int m_minInitNumParticles;
	unsigned int m_maxInitNumParticles;

	float m_minLifeTime;
	float m_maxLifeTime;
	unsigned int m_numberOfParticles;


	bool m_findNextDeadParticle(sParticle* &pFoundParticle); 

};

#endif