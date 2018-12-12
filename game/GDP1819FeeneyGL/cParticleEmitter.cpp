#include "cParticleEmitter.h"

#include <iostream>

sParticle::sParticle()
{
	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->lifeRemaining = 0.0f;
//	this->lifetime = 0.0f; // Optional, might not need this
	this->particleIndex = 0;
	return;
}
	
cParticleEmitter::cParticleEmitter()
{
	// TODO: Set to some "sensible" values
	this->m_minLifeTime = 1.0f;
	this->m_maxLifeTime = 5.0f;
	this->m_position = glm::vec3(0.0f,0.0f,0.0f);
	this->m_minVelocity = glm::vec3(-1.0f,-1.0f,-1.0f);
	this->m_maxVelocity = glm::vec3(1.0f,1.0f,1.0f);
	this->m_minInitNumParticles = 1;
	this->m_maxInitNumParticles = 1;

	this->m_acceleration = glm::vec3(0.0f);

	this->m_bIsInit = false;

	return;
}

cParticleEmitter::~cParticleEmitter()
{
	return;
}

	// EXPENSIVE operation, called when created or when
	//  we want to drastically change the parameters
void cParticleEmitter::Init( 
	float minLifeTime, float maxLifeTime, 
	unsigned int totalNumberOfParticles,		// 500
	glm::vec3 position,
	glm::vec3 minVelocity, glm::vec3 maxVelocity, 
	glm::vec3 acceleration,  
	unsigned int minInitNumParticles,		// 10	
	unsigned int maxInitNumParticles )		// 50
{
	// Clear any existing particles...
	this->vecParticles.clear();

	// This is really slow...
	for ( unsigned int index = 0; index != totalNumberOfParticles; index++ )
	{
		sParticle newParticle;			// NEW
		newParticle.lifeRemaining = -1.0f;
		newParticle.particleIndex = index;
		newParticle.position = glm::vec4( this->m_position, 1.0f );

		this->vecParticles.push_back( newParticle );
	}

	for ( unsigned int index = 0; index != totalNumberOfParticles; index++ )
	{
		this->vecParticles[index].particleIndex = index;
	}

	this->m_minLifeTime = minLifeTime;
	this->m_maxLifeTime = maxLifeTime;
	this->m_position = position;
	this->m_minVelocity = minVelocity;
	this->m_maxVelocity = maxVelocity;
	this->m_minInitNumParticles = minInitNumParticles;
	this->m_maxInitNumParticles = maxInitNumParticles;

	this->m_acceleration = acceleration;


	this->m_bIsInit = true;

	return;
}

// BAM!
void cParticleEmitter::Reset(void)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Set all the particles in motion
	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	// "kill" all the particles
	for ( ; index != totalParticles; index++ )
	{
		this->vecParticles[index].lifeRemaining = 0.0f;
	}

	// ******************************************
	// Generate new particles...
	// Create X particles to start 
	// How many particle are we creating this time? 
	unsigned int numParticles = this->getRandInRange( this->m_minInitNumParticles, 
													  this->m_maxInitNumParticles );
	sParticle* pCurParticle = NULL;

	for ( int count = 0; count != numParticles; count++ )
	{
		if ( this->m_findNextDeadParticle(pCurParticle) )
		{
			this->m_ResetParticle(pCurParticle);
		}
		else
		{
			// No more dead particles to "recycle"
			break;		// Break out of the for loop (??)
		}
	}//for ( int count = 0
	// ******************************************


	return;
}//void cParticleEmitter::Reset(void)


void cParticleEmitter::m_ResetParticle(sParticle* pParticle)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Pick a random lifetime
	pParticle->lifeRemaining = this->getRandInRange( this->m_minLifeTime, 
													 this->m_maxLifeTime );

	// Pick a random velocity
	pParticle->velocity = this->getRandInRange( this->m_minVelocity, 
												this->m_maxVelocity );

	pParticle->position = this->m_position;

	return;
}


void cParticleEmitter::Update(double deltaTime)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Update each "living" particle using Euler (or whatever)

	float fDeltaTime = (float)deltaTime;

	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	// Reference to particle (to make reading easier)
	//sParticle& curParticle = this->vecParticles[index];
	// Sort of like this, but a little safer (for stupid human programmers
	sParticle* pCurParticle = &( this->vecParticles[index] );



	for ( ; index != totalParticles; index++ )
	{
		//curParticle = this->vecParticles[index];
		pCurParticle = &( this->vecParticles[index] );

		// Is the particle alive? 
//		if ( curParticle.lifeRemaining > 0.0f )
		if ( pCurParticle->lifeRemaining > 0.0f )
		{
			// Is alive, so update
			// Velocity from accel
			//curParticle.velocity += (fDeltaTime * this->m_acceleration);
			pCurParticle->velocity += (fDeltaTime * this->m_acceleration);

			//curParticle.position += (fDeltaTime * curParticle.velocity);
			pCurParticle->position += (fDeltaTime * pCurParticle->velocity);

			// Take some life away...
			//curParticle.lifeRemaining -= fDeltaTime;
			pCurParticle->lifeRemaining -= fDeltaTime;
		}
	}

	// ******************************************
	// Generate new particles...
	// Create X particles to start 
	// How many particle are we creating this time? 
	unsigned int numParticles = this->getRandInRange( this->m_minInitNumParticles, 
													  this->m_maxInitNumParticles );
	pCurParticle = NULL;

	for ( int count = 0; count != numParticles; count++ )
	{
		if ( this->m_findNextDeadParticle(pCurParticle) )
		{
			this->m_ResetParticle(pCurParticle);
		}
		else
		{
			// No more dead particles to "recycle"
			break;		// Break out of the for loop (??)
		}
	}//for ( int count = 0
	// ******************************************

	return;
}

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
float cParticleEmitter::getRandInRange( float min, float max )
{
	float value = 
	min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(static_cast<float>(max-min) )));
	return value;
}

unsigned int cParticleEmitter::getRandInRange( unsigned int min, unsigned int max )
{
	// You could do this with the modulus operator (since it's an int)
	float value = 
	min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(static_cast<float>(max-min) )));
	return (unsigned int)value;
}


glm::vec3 cParticleEmitter::getRandInRange( glm::vec3 min, glm::vec3 max )
{
	glm::vec3 returnVal;

	returnVal.x = this->getRandInRange( min.x, max.x );
	returnVal.y = this->getRandInRange( min.y, max.y );
	returnVal.z = this->getRandInRange( min.z, max.z );

	return returnVal;
}

bool cParticleEmitter::m_findNextDeadParticle(sParticle* &pFoundParticle)
{
	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	for ( ; index != totalParticles; index++ )
	{
		if ( this->vecParticles[index].lifeRemaining <= 0.0f )
		{
			// It's dead, Jim...
			pFoundParticle = &(this->vecParticles[index]);

			return true;
		}
	}

	// DIDN'T find any...
	return false;
}
