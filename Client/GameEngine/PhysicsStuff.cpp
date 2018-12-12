// PhysicsStuff
#include "globalStuff.h"	// 
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "sModelDrawInfo.h"
//#include "PlayerCollision.cpp"

typedef glm::vec3 Point;
typedef glm::vec3 Vector;

const float GROUND_PLANE_Y = -3.0f;			// Lowest the objects can go

const float LIMIT_POS_X =  100.0f;			// Lowest the objects can go
const float LIMIT_NEG_X = -100.0f;			// Lowest the objects can go
const float LIMIT_POS_Z =  100.0f;			// Lowest the objects can go
const float LIMIT_NEG_Z = -100.0f;			// Lowest the objects can go
Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);



//class cBox
//{
//public:
//	cPlanes sides[6];
//	float leftSide;
//	float rightSide;
//	//...
//	glm::vec3 minXYZ:
//	glm::vec3 maxXYZ;

	glm::vec3 facingOutsideVectors[6];
//	// 
//	glm::vec3 centreXYZ;
//	glm::vec3 sideLengths;
//	glm::vec3 halfSideLengths;
//}

bool AreSpheresPenetrating( cMeshObject* pA, cMeshObject* pB );

bool TestForCollision( cMeshObject* pA, cMeshObject* pB );

void PlayerColTest(double deltaTime, GLuint shaderProgramID);
// Called every frame
void DoPhysicsUpdate( double fDeltaTime, 
					  std::vector< cMeshObject* > &vec_pObjectsToDraw )
{
	float deltaTime = static_cast<float>(fDeltaTime);
	//PlayerColTest(fDeltaTime, program);

	// Make sure it's not tooooooo big
	const float LARGEST_DELTATIME = 0.10f;			// 10 ms = 10 Hz

	if ( deltaTime > LARGEST_DELTATIME )
	{
		deltaTime = LARGEST_DELTATIME;
	}//if ( deltaTime

	// Loop through all objects
	for ( std::vector< cMeshObject* >::iterator itMesh = vec_pObjectsToDraw.begin();
		  itMesh != vec_pObjectsToDraw.end(); itMesh++ )
	{
		cMeshObject* pCurMesh = *itMesh;		 

		if ( pCurMesh->bIsUpdatedByPhysics )
		{

				pCurMesh->velocity.x = pCurMesh->velocity.x + (pCurMesh->accel.x * deltaTime);
				pCurMesh->velocity.y = pCurMesh->velocity.y + (pCurMesh->accel.y * deltaTime);
				pCurMesh->velocity.z = pCurMesh->velocity.z + (pCurMesh->accel.z * deltaTime);

		

				pCurMesh->position.x = pCurMesh->position.x + (pCurMesh->velocity.x * deltaTime);
				pCurMesh->position.y = pCurMesh->position.y + (pCurMesh->velocity.y * deltaTime);
				pCurMesh->position.z = pCurMesh->position.z + (pCurMesh->velocity.z * deltaTime);

				//if (glm::length(pCurMesh->velocity) > 2.0f) {
				//	std::cout << glm::length(pCurMesh->velocity) << std::endl;
				//	pCurMesh->velocity.x = pCurMesh->velocity.x *= 0.99f;
				//	pCurMesh->velocity.y = pCurMesh->velocity.y *= 0.99f;
				//	pCurMesh->velocity.z = pCurMesh->velocity.z *= 0.99f;
				//	if (glm::length(pCurMesh->velocity) < 2.0f) { pCurMesh->velocity = glm::vec3(0.0f); }
				//}


			// The object can't go any lower than the "ground".
			// The "ground" is a plane along the x-z axis.
			// So we can decide a "height" along the y....
			// ... if it's below that, then reverse the direction of travel... 
			// ... IN THE Y
//
//			if ( pCurMesh->type == SPHERE && pOtherObject == TRIANGLE )
//			{
//				if ( DidTheCollide( pCurMesh, pOtherObject ) )
//				{
//					// Do Something..
//
//				}
//
//////			if ( pCurMesh->position.y <= GROUND_PLANE_Y )
//			if ( pCurMesh->position.y  <= GROUND_PLANE_Y )
//			{
//				// Normal to the ground plane is 0, +1, 0 (+1 in the y)w
//				glm::vec3 normalToGround = glm::vec3( 0.0f, 1.0f, 0.0f );
//
//				// Calcualte the REFLECTION vector (based on the normal and ???)
//
//				glm::vec3 newVel = glm::reflect( pCurMesh->velocity, normalToGround );
//				// reverse the direction of travel IN THE Y. 
//				// Why??? 
////				pCurMesh->velocity *= 0.99f;
////				pCurMesh->velocity.y = fabs(pCurMesh->velocity.y) ;
//				pCurMesh->velocity = newVel;
//			}
//
////			if ( pCurMesh->position.x >= LIMIT_POS_X )
//			// Sphere - Plane test... 
//			// Is this sphere PENETRATED this plane
//			// Is is INSIDE or OUTSIDE the plane?
//			if ( pCurMesh->position.x >= LIMIT_POS_X )
//			{
//				pCurMesh->velocity.x = -fabs(pCurMesh->velocity.x) ;
//			}
////			if ( pCurMesh->position.x <= LIMIT_NEG_X )
//			if ( pCurMesh->position.x <= LIMIT_NEG_X )
//			{
//				pCurMesh->velocity.x = fabs(pCurMesh->velocity.x) ;
//			}
//			if ( pCurMesh->position.z >= LIMIT_POS_Z )
//			{
//				pCurMesh->velocity.z = -fabs(pCurMesh->velocity.z) ;
//			}
//			if ( pCurMesh->position.z <= LIMIT_NEG_Z )
//			{
//				pCurMesh->velocity.z = fabs(pCurMesh->velocity.z) ;
//			}


		//	// Check if I'm contacting another sphere..
		//	for ( std::vector< cMeshObject* >::iterator itMesh = vec_pObjectsToDraw.begin();
		//		  itMesh != vec_pObjectsToDraw.end(); itMesh++ )
		//	{
		//		cMeshObject* pOtherMesh = *itMesh;
//
		//		if ( ! pOtherMesh->bIsUpdatedByPhysics )
		//		{
		//			// Or do I????? (can non moving things still collide with this???)
		//			continue;
		//		}
//
		//		// Same mesh
		//		if ( pCurMesh != pOtherMesh )
		//		{
		//			if ( AreSpheresPenetrating( pOtherMesh, pCurMesh ) )
		//			{
		//				pCurMesh->objColour = glm::vec3( 1.0f, 0.0f, 0.0f );
		//			}
		//			else
		//			{
		//				// Make it pink
		//			}
		//		}
		//		
//
//
		//	}// Inner sphere-sphere test
//
//
//
		}//if ( pCurMesh
	}//for ( std::vector< cMeshObject*

	if (vec_cur_AABB_tris.size() > 0) 
	{
		// Test for collisions
		for (std::vector< cMeshObject* >::iterator itObjectA = vec_pObjectsToDraw.begin();
			itObjectA != vec_pObjectsToDraw.end(); itObjectA++)
		{
			cMeshObject* pCurObj = *itObjectA;

			if (pCurObj->bIsUpdatedByPhysics) {
				for (std::vector<cAABB::sAABB_Triangle>::iterator itTri = vec_cur_AABB_tris.begin(); itTri != vec_cur_AABB_tris.end(); itTri++)
				{
					cAABB::sAABB_Triangle CurTri = *itTri;
					glm::vec3 closestPointToTri = ClosestPtPointTriangle(pCurObj->position,
						CurTri.verts[0], CurTri.verts[1], CurTri.verts[2]);

					// is this point LESS THAN the radius of the sphere? 
					if (glm::distance(closestPointToTri, pCurObj->position) < 1.0f)
					{
						pCurObj->velocity = glm::vec3(0.0f);
						std::cout << " collision " << std::endl;
					}

				}
			}
			

		}
	}

	PlayerColTest(fDeltaTime, program);

	// Test for collisions
	for ( std::vector< cMeshObject* >::iterator itObjectA = vec_pObjectsToDraw.begin();
		itObjectA != vec_pObjectsToDraw.end(); itObjectA++ )
	{
		// Go through all the other objects and test with this one...

		for ( std::vector< cMeshObject* >::iterator itObjectB = vec_pObjectsToDraw.begin();
				itObjectB != vec_pObjectsToDraw.end(); itObjectB++ )
		{

			cMeshObject* pObjectA = *itObjectA;
			cMeshObject* pObjectB = *itObjectB;
			// Same? 

			if ( pObjectA != pObjectB )
			{
				// Do BOTH of these have a "shape" defined
				// Could also test for the enum (which is WAY better)
				if ( ( pObjectA->pTheShape != NULL  ) && 
				     ( pObjectB->pTheShape != NULL ) )
				{
					if ( TestForCollision( pObjectA, pObjectB ) )
					{
						// Do something about that collision.
						// Usually it's the reflection vector+normal changing velocity

						//pObjectB->objColour = glm::vec3( 0.0f, 0.0f, 1.0f );
						//pObjectA->objColour = glm::vec3( 0.0f, 0.0f, 1.0f );

						
						if ( (pObjectA->shapeType == cMeshObject::SPHERE) && (pObjectB->shapeType == cMeshObject::SPHERE) )
						{
							// If it's a Sphere-Sphere, make the intersection lines yellow
							::g_pDebugRenderer->addLine( pObjectA->position, pObjectB->position, 
														 glm::vec3( 1.0f, 1.0f, 0.0f ), 
														 2.0f /*show for 2 seconds*/ );
						}
						else if ( (pObjectA->shapeType == cMeshObject::SPHERE) && (pObjectB->shapeType == cMeshObject::TRIANGLE) )
						{
							// If it's a Sphere-Triange, make the intersection lines magenta
							::g_pDebugRenderer->addLine( pObjectA->position, pObjectB->position, 
														 glm::vec3( 1.0f, 0.0f, 1.0f ), 
														 2.0f /*show for 2 seconds*/ );
						}
					}//if(TestForCollision(...
				}
			}//if(pObjectA != pObjectB)
		}// inner loop
	}// outer loop

	
	return;
}


//Triangles from AABB Hierarchy and Player collishion check


Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{
    Vector ab = b - a;
    Vector ac = c - a;
    Vector bc = c - b;

    // Compute parametric position s for projection P' of P on AB,
    // P' = A + s*AB, s = snom/(snom+sdenom)
 //   float snom = Dot(p - a, ab), sdenom = Dot(p - b, a - b);
	float snom = glm::dot( p - a, ab );
	float sdenom = glm::dot(p - b, a - b);

    // Compute parametric position t for projection P' of P on AC,
    // P' = A + t*AC, s = tnom/(tnom+tdenom)
//    float tnom = Dot(p - a, ac), tdenom = Dot(p - c, a - c);
    float tnom = glm::dot(p - a, ac);
	float tdenom = glm::dot(p - c, a - c);

    if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

    // Compute parametric position u for projection P' of P on BC,
    // P' = B + u*BC, u = unom/(unom+udenom)
//    float unom = Dot(p - b, bc), udenom = Dot(p - c, b - c);
    float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

    if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
    if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


    // P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
//    Vector n = Cross(b - a, c - a);
	Vector n = glm::cross( b - a, c - a);
//    float vc = Dot(n, Cross(a - p, b - p));

    float vc = glm::dot(n, glm::cross(a - p, b - p));

    // If P outside AB and within feature region of AB,
    // return projection of P onto AB
    if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
        return a + snom / (snom + sdenom) * ab;

    // P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
//    float va = Dot(n, Cross(b - p, c - p));
    float va = glm::dot(n, glm::cross(b - p, c - p));


    // If P outside BC and within feature region of BC,
    // return projection of P onto BC
    if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
        return b + unom / (unom + udenom) * bc;

    // P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
//    float vb = Dot(n, Cross(c - p, a - p));
    float vb = glm::dot(n, glm::cross(c - p, a - p));

   // If P outside CA and within feature region of CA,
    // return projection of P onto CA
    if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
        return a + tnom / (tnom + tdenom) * ac;

    // P must project inside face region. Compute Q using barycentric coordinates
    float u = va / (va + vb + vc);
    float v = vb / (va + vb + vc);
    float w = 1.0f - u - v; // = vc / (va + vb + vc)
    return u * a + v * b + w * c;
}

// Pass in the terrain
// Pass in the location of the Bunny (the one I can move)
// "return" (by reference) as list of points
// --> then I can draw those points

void CalculateClosestPointsOnMesh( sModelDrawInfo theMeshDrawInfo, 
								   glm::vec3 pointToTest, 
								   std::vector<sClosestPointData> &vecPoints )
{
	vecPoints.clear();

	// For each triangle in the mesh information...
	for ( unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++ )
	{
		sPlyTriangle CurTri = theMeshDrawInfo.pTriangles[triIndex];

		sClosestPointData closestTri;
		closestTri.triangleIndex = triIndex;

		// ... call the ClosestPointToTriangle...
		// Need to get the 3 vertices of the triangle
		sPlyVertex corner_1 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3];

		// Convert this to glm::vec3
		glm::vec3 vert_1 = glm::vec3( corner_1.x, corner_1.y, corner_1.z );
		glm::vec3 vert_2 = glm::vec3( corner_2.x, corner_2.y, corner_2.z );
		glm::vec3 vert_3 = glm::vec3( corner_3.x, corner_3.y, corner_3.z );

		closestTri.thePoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);

		vecPoints.push_back( closestTri );

	// ...and push back the restulting point
	}//for ( unsigned int triIndex = 0
		  
		  


}


bool AreSpheresPenetrating( cMeshObject* pA, cMeshObject* pB )
{
	sSphere* pSphereA = (sSphere*)(pA->pTheShape);
	sSphere* pSphereB = (sSphere*)(pB->pTheShape);

	if ( glm::distance( pA->position, pB->position ) < ( pSphereA->radius + pSphereB->radius ) )
	{
		// Yup
		return true;
	}
	// Nope 
	return false;
}

bool SphereTraingleTest( cMeshObject* pSphere, cMeshObject* pTriangle )
{
	sSphere* pSphereA = (sSphere*)(pSphere->pTheShape);
	sTriangle* pTri = (sTriangle*)(pTriangle->pTheShape);

	glm::vec3 closestPointToTri = ClosestPtPointTriangle( pSphere->position, 
														  pTri->v[0], pTri->v[1], pTri->v[2] );

	// is this point LESS THAN the radius of the sphere? 
	if ( glm::distance( closestPointToTri, pSphere->position ) <= pSphereA->radius )
	{
		return true;
	}

	return false;
}

bool TestForCollision( cMeshObject* pA, cMeshObject* pB )
{
	if ( pA->pTheShape == NULL ) {	return false; /*print error?*/ }
	if ( pB->pTheShape == NULL ) {	return false; /*print error?*/ }

	// Sphere - sphere
	if ( (pA->shapeType == cMeshObject::SPHERE) && 
		 (pB->shapeType == cMeshObject::SPHERE ) )
	{
		return AreSpheresPenetrating( pA, pB );
	}
	else if ( (pA->shapeType == cMeshObject::SPHERE ) && 
			  (pB->shapeType == cMeshObject::TRIANGLE ) )
	{
		return SphereTraingleTest( pA, pB );
	}
	//else


	return false;
}
