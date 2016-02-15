#pragma once
#include "Vector3.h"
#include "Mesh.h"
#include "SceneGraph/SceneNode.h"	// Scene Graph
#include "SpatialPartition/SpatialPartition.h"	// SpatialPartition

class CPlayInfo3PV
{
public:
	CPlayInfo3PV(void);
	virtual ~CPlayInfo3PV(void);

	enum GEOMETRY_TYPE
	{
		GEO_AVATAR,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);

	// Reset this class instance
	void Reset();

	// Set Model
	bool SetNode(CSceneNode* theAvatarNode);

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set position z of the player
	void SetPos_z(int pos_y);

	// Update Movements
	void MoveFrontBack(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);
	void RotateLeftRight(const bool mode, const float timeDiff, CSpatialPartition* sp);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Get position z of the player
	int GetPos_z(void);
	// Get position of the player
	Vector3 GetPosition();
	// Get direction of the player
	Vector3 GetCharDirection();
	// Get direction of the Camera
	Vector3 GetCamDirection();
	// Get Floating Amount of the player
	float GetFloatingAmount(void);
	// Get New Position of the player
	Vector3 GetNewPosition();
	// Get New Direction of the camera
	Vector3 GetNewDirection();
	// Get is player moving
	Vector3 IsMoving();

	// Update
	void UpdateMovement(const unsigned char key, const bool status = true);
	// Update
	void Update(const double dt, const bool cooldowning, CSpatialPartition* sp, bool canPlay);
	// Check Collision
	bool CheckCollision(CSpatialPartition* sp, int hitpoints);

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder,
		const int topBorder, const int bottomBorder,
		float timeDiff);

	// Avatar's SceneNode
	CSceneNode*	theAvatarNode;

	// Basic methods
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);
	virtual void LookUp(const double dt);
	virtual void LookDown(const double dt);

	// Applied methods
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);

	// Jump
	void Jump(const double dt);
	// Update Jump
	void UpdateJump(const double dt);

	// Update Movement
	void UpdateMovement(const double dt, CSpatialPartition* sp);
	// Update Bottom
	void UpdateBottom(const double dt, const Vector3 prev_RotationVel);

	// Update Camera
	void UpdateCamera(const double dt, const Vector3 prev_Position, const Vector3 prev_CharDirection);

	// Update Camera to Right
	void CPlayInfo3PV::UpdateCameraToRight(void);
	// Update Camera to Left
	void CPlayInfo3PV::UpdateCameraToLeft(void);

private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curCharDirection;
	Vector3 curCamDirection;
	Vector3 up;
	Vector3 MoveVel;
	Vector3 RotateVel;
	Vector3 bottomRotationLeftRightUpDown;
	bool floatingUp;
	float floatingAmount;

	bool m_bJumping;
	float JumpVel;
	float JUMPACCEL;
	float JUMPMAXSPEED;
	float GRAVITY;

	bool myKeys[255];

	Vector3 newPosition;
	Vector3 newDirection;
	Vector3 maxNewPosition;
	Vector3 maxNewDirection;
	bool camAtRight;
	bool moving;
};