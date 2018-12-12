#include "globalStuff.h"

// These are for the transformation glm stuff
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


void PlayerColTest(double deltaTime, GLuint shaderProgramID)
{
//	// Get the MiG-29
//	cMeshObject* pMig29 = findObjectByFriendlyName("mig");
//	cMeshObject* pDebugBall = findObjectByFriendlyName("DebugSphere");
//	// So show that the points move with the mig, I'll update the position and orientation here	
//	if (camera.Front.y > 0.3f) { pMig29->adjMeshOrientationEulerAngles(glm::vec3(0.01f, 0.0f, 0.0f), false); }
//	if (camera.Front.y < -0.3f) { pMig29->adjMeshOrientationEulerAngles(glm::vec3(-0.01f, 0.0f, 0.0f), false); }
//	if (camera.Front.x < 0.3f) { pMig29->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.01f, 0.0f), false); }
//	if (camera.Front.x > -0.3f) { pMig29->adjMeshOrientationEulerAngles(glm::vec3(0.0, -0.01f, 0.0f), false); }
////	pMig29->setMeshOrientationEulerAngles(glm::vec3(camera.Front.y * 2, camera.Front.x * 2, camera.Front.z * 2), false);
//
//	// *****************************************************
//	// Move the Mig-29 in the direction it's facing..
//	// You do the same sort of thing as below, but ONLY use orienation.
//	// So, find where the "nose" point is pointing in MODEL SPACE only.
//
//	// Here, "forward" is along the z axis (i.e. the plane is facing that way)
//	// So x and y are 0.0, z = 1.0f 
//	// w = 1.0 because it's the 4th value
//	glm::vec4 vecForwardDirection_ModelSpace = glm::vec4(0.0f, 0.0f, /**/1.0f/**/, 1.0f);
//
//	// Now orientation
//	glm::quat qMig29Rotation = pMig29->getQOrientation();
//	// Make a mat4x4 from that quaternion
//	glm::mat4 matQMig29rotation = glm::mat4(qMig29Rotation);
//
//	glm::vec4 vecForwardDirection_WorldSpace = matQMig29rotation * vecForwardDirection_ModelSpace;
//
//	// optional normalize
//	vecForwardDirection_WorldSpace = glm::normalize(vecForwardDirection_WorldSpace);
//
//	// Adjust the speed relative to the direction
//	float forwardSpeed = 10.0f;
//	float forwardSpeedThisFrame = forwardSpeed * deltaTime;
//
//	glm::vec3 positionAdjustThisFrame = vecForwardDirection_WorldSpace * forwardSpeedThisFrame;
//
//	// Update the position (in the direction it's facing)
//	pMig29->position += positionAdjustThisFrame;
//	camera.Position = glm::vec3(pMig29->position.x, pMig29->position.y, pMig29->position.z + 1.0f);
//	// *****************************************************
//	//pMig29->position.x += 0.01f;
//	//pMig29->position.y += 0.02f;
//
//
//
//
//	// Collision point on the nose of the plane.
//	// Plane it facing forward along the +ve Z axis
//
//	// NOTE: These are vec4 because we will be multiplying them by mat4, eventually.
//	// (As usual, if you don't know, make the 4th value a 1.0f)
//
//	// From MeshLab, the nose is about 0,0,0.4705
//	glm::vec4 noseContactPoint_ModelSpace = glm::vec4(0.0f, 0.0f, 0.4705f, 1.0f);
//	// The wings are on the XZ plane, about -0.3 back from the centre
//	glm::vec4 leftWingTipContactPoint_ModelSpace = glm::vec4(0.3692f, 0.0f, -0.9f, 1.0f);
//	glm::vec4 rightWingTipContactPoint_ModelSpace = glm::vec4(-0.3692f, 0.0f, -0.9f, 1.0f);
//
//	// Adjust the points based on orientation and position of the Mig-29:
//
//	// These transformations are taken from the draw call. 
//	// We want them to be EXACTLY THE SAME, same order, etc.
//
//	glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix
//
//	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
//		pMig29->position);
//
//	matTransform = matTransform * matTranslation;		// matMove
//
//	// Now orientation
//	glm::quat qRotation = pMig29->getQOrientation();
//	// Make a mat4x4 from that quaternion
//	glm::mat4 matQrotation = glm::mat4(qRotation);
//
//	matTransform = matTransform * matQrotation;
//
//
//	// Calcualte the collision points
//	glm::vec4 noseContactPoint_WorldSpace = glm::vec4(0.0f);
//	glm::vec4 leftWingTipContactPoint_WorldSpace = glm::vec4(0.0f);
//	glm::vec4 rightWingTipContactPoint_WorldSpace = glm::vec4(0.0f);
//
//	noseContactPoint_WorldSpace = matTransform * noseContactPoint_ModelSpace;
//	leftWingTipContactPoint_WorldSpace = matTransform * leftWingTipContactPoint_ModelSpace;
//	rightWingTipContactPoint_WorldSpace = matTransform * rightWingTipContactPoint_ModelSpace;
//
//	// In this case, we will draw them as spheres
//	// But you would pass these "world space" points into the collision test...
//
//	
//
//	glm::vec3 oldPosition = pDebugBall->position;
//	glm::vec3 oldScale = pDebugBall->nonUniformScale;
//
//	pDebugBall->bIsVisible = true;
//	pDebugBall->bIsWireFrame = true;
//	pDebugBall->bDontLight = true;
//	pDebugBall->setUniformScale(0.25f);
//
//	glm::mat4 matIdentity = glm::mat4(1.0f);
//
//	// Draw nose:
//	pDebugBall->materialDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//	pDebugBall->position = noseContactPoint_WorldSpace;
//	DrawObject(pDebugBall, matIdentity, shaderProgramID);
//	// Draw left wing point:
//	pDebugBall->materialDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	pDebugBall->position = leftWingTipContactPoint_WorldSpace;
//	DrawObject(pDebugBall, matIdentity, shaderProgramID);
//	// Draw right wing point:
//	pDebugBall->materialDiffuse = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
//	pDebugBall->position = rightWingTipContactPoint_WorldSpace;
//	DrawObject(pDebugBall, matIdentity, shaderProgramID);
//
//	// Put everything back
//	pDebugBall->bIsVisible = false;
//	pDebugBall->position = oldPosition;
//	pDebugBall->nonUniformScale = oldScale;
//
//	return;
}