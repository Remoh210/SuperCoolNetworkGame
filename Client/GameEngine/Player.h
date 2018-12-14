#ifndef _PLAYER_HG_
#define _PLAYER_HG_

#include <glm/vec3.hpp>
#include "cMeshObject.h"

class Player
{
public:
	Player();
	~Player();

	int playerID; //0 - 3, since there are only 4 players at most
	int msgid; //the current message id according to the server

	cMeshObject * obj; //the mesh object that belongs to this player

	glm::vec3 curPosition; //current position client-side
	glm::vec3 serverPosition; //current position server-side
};

#endif
