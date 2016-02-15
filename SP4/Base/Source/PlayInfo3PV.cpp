#include "PlayInfo3PV.h"
#include "MeshBuilder.h"
#include "Application.h"

float MOVE_ACC = 50.f;
float ROTATE_ACC = 100.f;
float MOVE_CAMERA_SPEED = 200.f;

CPlayInfo3PV::CPlayInfo3PV(void)
	: theAvatarNode(NULL)
	, floatingUp(true)
	, floatingAmount(0.f)
	, m_bJumping(false)
	, JumpVel(0.f)
	, JUMPACCEL(50.f)
	, JUMPMAXSPEED(50.f)
	, GRAVITY(-100.f)
	, camAtRight(true)
	, moving(false)
{
	Init();
}


CPlayInfo3PV::~CPlayInfo3PV(void)
{
	if (theAvatarNode)
	{
		delete theAvatarNode;
		theAvatarNode = NULL;
	}
}

// Initialise this class instance
void CPlayInfo3PV::Init(void)
{
	curPosition.Set(0, 0, 0);
	curCharDirection.Set(0, 0, 1);
	curCamDirection.Set(0, 0, 1);
	MoveVel.SetZero();
	RotateVel.SetZero();
	bottomRotationLeftRightUpDown.SetZero();

	Vector3 view = curCamDirection;
	Vector3 right = view.Cross( Vector3(0,1,0) );
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();

	// Set camera look 45 degree downward
	Mtx44 rotation;
	rotation.SetToRotation(-45, right.x, right.y, right.z);
	curCamDirection = rotation * curCamDirection;

	// Initialise the Avatar's movement flags
	for (int i = 0; i<255; i++){
		myKeys[i] = false;
	}
}


// Reset this class instance
void CPlayInfo3PV::Reset(void)
{
	curPosition.Set(0, 0, 0);
	curCharDirection.Set(0, 0, 1);
	curCamDirection.Set(0, 0, 1);
	MoveVel.SetZero();
	RotateVel.SetZero();
	bottomRotationLeftRightUpDown.SetZero();

	Vector3 view = curCamDirection;
	Vector3 right = view.Cross(Vector3(0, 1, 0));
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();

	// Set camera look 45 degree downward
	Mtx44 rotation;
	rotation.SetToRotation(-45, right.x, right.y, right.z);
	curCamDirection = rotation * curCamDirection;

	// Initialise the Avatar's movement flags
	for (int i = 0; i<255; i++){
		myKeys[i] = false;
	}


	theAvatarNode->SetTransform(CTransform(0, 0, 0));

	SetPos_x(500);
	SetPos_z(488);


	floatingUp = true;
	floatingAmount = 0.f;
	m_bJumping = false;
	JumpVel = 0.f;
	camAtRight = true;
	moving = false;
}

// Set Model
bool CPlayInfo3PV::SetNode(CSceneNode* theAvatarNode)
{
	this->theAvatarNode = theAvatarNode;
	if (this->theAvatarNode == NULL)
		return false;

	return true;
}

// Returns true if the player is on ground
bool CPlayInfo3PV::isOnGround(void)
{
	if (m_bJumping)
		return false;
	return true;
}

// Set position x of the player
void CPlayInfo3PV::SetPos_x(int pos_x)
{
	curPosition.x = pos_x;

	theAvatarNode->SetTranslation(curPosition.x, curPosition.y, curPosition.z);
	newPosition = curPosition;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_y(int pos_y)
{
	curPosition.y = pos_y;

	theAvatarNode->SetTranslation(curPosition.x, curPosition.y, curPosition.z);
	newPosition = curPosition;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_z(int pos_z)
{
	curPosition.z = pos_z;

	theAvatarNode->SetTranslation(curPosition.x, curPosition.y, curPosition.z);
	newPosition = curPosition;
}

/********************************************************************************
Hero Move Up Down
********************************************************************************/
void CPlayInfo3PV::MoveFrontBack(const bool mode, const float timeDiff)
{
	if (mode)
	{
		//curPosition.z = curPosition.z - (int)(200.0f * timeDiff);

		// Update the avatar velocity
		MoveVel += curCharDirection * (MOVE_ACC * timeDiff);

		RotateVel.x += ROTATE_ACC * timeDiff;
	}
	else
	{
		//curPosition.z = curPosition.z + (int)(200.0f * timeDiff);

		// Update the avatar velocity
		MoveVel += curCharDirection * (-MOVE_ACC * timeDiff);

		RotateVel.x += -ROTATE_ACC * timeDiff;
	}
}

/********************************************************************************
Hero Move Left Right
********************************************************************************/
void CPlayInfo3PV::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		//curPosition.x = curPosition.x + (int)(200.0f * timeDiff);

		Vector3 view = curCharDirection;
		Vector3 right = view.Cross(Vector3(0, 1, 0));
		right.y = 0;
		right.Normalize();

		// Update the avatar velocity
		MoveVel += right * (MOVE_ACC * timeDiff);

		RotateVel.z += ROTATE_ACC * timeDiff;
	}
	else
	{
		//curPosition.x = curPosition.x - (int)(200.0f * timeDiff);

		Vector3 view = curCharDirection;
		Vector3 right = view.Cross(Vector3(0, 1, 0));
		right.y = 0;
		right.Normalize();

		// Update the avatar velocity
		MoveVel += right * (-MOVE_ACC * timeDiff);

		RotateVel.z += -ROTATE_ACC * timeDiff;
	}
}

/********************************************************************************
Hero Rotate Left Right
********************************************************************************/
void CPlayInfo3PV::RotateLeftRight(const bool mode, const float timeDiff, CSpatialPartition* sp)
{
	float yaw;
	if (mode)
	{
		yaw = (float)(50 * (float)timeDiff);
	}
	else
	{
		yaw = (float)(-50 * (float)timeDiff);
	}
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curCharDirection = rotation * curCharDirection;
	theAvatarNode->ApplyRotate(yaw, 0, 1, 0);


	// Check collision with objects
	if (sp->CheckForCollision(theAvatarNode, 1))
	{
		rotation.SetToRotation(-yaw, 0, 1, 0);
		curCharDirection = rotation * curCharDirection;
		theAvatarNode->ApplyRotate(-yaw, 0, 1, 0);
	}
}

// Get position x of the player
int CPlayInfo3PV::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
int CPlayInfo3PV::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
int CPlayInfo3PV::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 CPlayInfo3PV::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 CPlayInfo3PV::GetCharDirection()
{
	return curCharDirection;
}

// Get direction of the camera
Vector3 CPlayInfo3PV::GetCamDirection()
{
	return curCamDirection;
}


// Get Floating Amount of the player
float CPlayInfo3PV::GetFloatingAmount(void)
{
	return floatingAmount;
}

// Get New Position of the player
Vector3 CPlayInfo3PV::GetNewPosition()
{
	return newPosition;
}

// Get New Direction of the camera
Vector3 CPlayInfo3PV::GetNewDirection()
{
	return newDirection;
}

// Get is player moving
Vector3 CPlayInfo3PV::IsMoving()
{
	return moving;
}


// Constrain the position of the Hero to within the border
void CPlayInfo3PV::ConstrainHero(const int leftBorder, const int rightBorder,
	const int topBorder, const int bottomBorder,
	float timeDiff)
{
	if (curPosition.x < leftBorder)
	{
		curPosition.x = leftBorder;
	}
	else if (curPosition.x > rightBorder)
	{
		curPosition.x = rightBorder;
	}

	if (curPosition.y < topBorder)
		curPosition.y = topBorder;
	else if (curPosition.y > bottomBorder)
		curPosition.y = bottomBorder;
}


/********************************************************************************
Update Movement
********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
Update
********************************************************************************/
void CPlayInfo3PV::Update(const double dt, const bool cooldowning, CSpatialPartition* sp, bool canPlay)
{
	bool targeting = false;
	Vector3 prev_Position = curPosition;
	Vector3 prev_CharDirection = curCharDirection;
	Vector3 prev_RotationVel = RotateVel;

	if (canPlay)
	{
		// WASD movement
		if (myKeys['w'] == true)
		{
			MoveFrontBack(true, dt);
		}
		if (myKeys['s'] == true)
		{
			MoveFrontBack(false, dt);
		}
		if (myKeys['a'] == true)
		{
			MoveLeftRight(false, dt);
		}
		if (myKeys['d'] == true)
		{
			MoveLeftRight(true, dt);
		}
		// QE rotate
		if (myKeys['q'] == true)
		{
			RotateLeftRight(true, dt, sp);
		}
		if (myKeys['e'] == true)
		{
			RotateLeftRight(false, dt, sp);
		}
		// Jump
		if (myKeys[32] == true)
		{
			Jump(dt);
			myKeys[32] = false;
		}
		// targeting
		if (myKeys[160] == true)
		{
			targeting = true;
			myKeys[160] = false;
		}
		//Update the camera direction based on mouse move
		// left-right rotate
		if (Application::camera_yaw != 0)
			Yaw(dt);
		if (Application::camera_pitch != 0)
			Pitch(dt);

		theAvatarNode->UpdateFlickering(dt);

		UpdateJump(dt);

		UpdateMovement(dt, sp);

		UpdateBottom(dt, prev_RotationVel);

		// Floating
		if (m_bJumping == false)
		{
			if (floatingUp)
			{
				floatingAmount += dt + MoveVel.Length() * 0.01 * dt;
				if (floatingAmount >= 0.5f)
					floatingUp = false;
			}
			else
			{
				floatingAmount -= dt + MoveVel.Length() * 0.01 * dt;
				if (floatingAmount <= -0.5f)
					floatingUp = true;
			}
		}

		if (prev_Position == curPosition && cooldowning == false && targeting == false)
		{
			moving = false;
		}
		else
		{
			// Siwtch camera
			static bool tabKey = false;
			if (myKeys[9] == true && tabKey == false)
			{
				if (camAtRight == true)
				{
					camAtRight = false;
				}
				else if (camAtRight == false)
				{
					camAtRight = true;
				}
				tabKey = true;
			}
			else if (myKeys[9] == false && tabKey == true)
			{
				tabKey = false;
			}

			moving = true;
		}
	}

	UpdateCamera(dt, prev_Position, prev_CharDirection);
}

/********************************************************************************
Turn left
********************************************************************************/
void CPlayInfo3PV::TurnLeft(const double dt)
{
	float yaw = (float)(-200 * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curCamDirection = rotation * curCamDirection;
	Vector3 right = curCamDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(curCamDirection).Normalized();
}
/********************************************************************************
Turn right
********************************************************************************/
void CPlayInfo3PV::TurnRight(const double dt)
{
	float yaw = (float)(-200 * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curCamDirection = rotation * curCamDirection;
	Vector3 right = curCamDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(curCamDirection).Normalized();
}
/********************************************************************************
LookUp
********************************************************************************/
void CPlayInfo3PV::LookUp(const double dt)
{
	float pitch = (float)(200 * (float)dt);
	Vector3 right = curCamDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(curCamDirection).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	curCamDirection = rotation * curCamDirection;
	if (curCamDirection.y < -0.7f)
	{
		curCamDirection.y = -0.7f;
	}
}
/********************************************************************************
LookDown
********************************************************************************/
void CPlayInfo3PV::LookDown(const double dt)
{
	float pitch = (float)(200 * (float)dt);
	Vector3 right = curCamDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(curCamDirection).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	curCamDirection = rotation * curCamDirection;
	if (curCamDirection.y > 0.15f)
	{
		curCamDirection.y = 0.15f;
	}
}

/********************************************************************************
Pitch. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Pitch(const double dt)
{
	if (Application::camera_pitch > 0.0 && curCamDirection.y > -0.7f)
		LookUp(-Application::camera_pitch * dt);
	else if (Application::camera_pitch < 0.0 && curCamDirection.y < 0.15f)
		LookDown(-Application::camera_pitch * dt);
}
/********************************************************************************
Yaw. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Yaw(const double dt)
{
	if (Application::camera_yaw > 0.0)
		TurnRight(Application::camera_yaw * dt);
	else if (Application::camera_yaw < 0.0)
		TurnLeft(Application::camera_yaw * dt);
}



/********************************************************************************
Jump
********************************************************************************/
void CPlayInfo3PV::Jump(const double dt)
{
	if (m_bJumping == false)
	{
		m_bJumping = true;
		floatingAmount = 0.f;

		// Calculate the jump velocity
		JumpVel += JUMPACCEL;// * dt;

		// Factor in maximum speed limit
		if (JumpVel > JUMPMAXSPEED)
			JumpVel = JUMPMAXSPEED;
	}
}

/********************************************************************************
Update Jump
********************************************************************************/
void CPlayInfo3PV::UpdateJump(const double dt)
{
	if (m_bJumping == true)
	{
		// Factor in gravity
		JumpVel += GRAVITY * dt;

		// Update the avatar node
		theAvatarNode->ApplyTranslate(0, JumpVel * (float)dt, 0);
		// Update the avatar position
		curPosition.y += JumpVel * (float)dt;

		// Check if the camera has reached the ground
		if (curPosition.y <= 0)
		{
			theAvatarNode->ApplyTranslate(0, -curPosition.y, 0);
			curPosition.y = 0;
			JumpVel = 0.0f;
			m_bJumping = false;
		}
	}
}

/********************************************************************************
Update Movement
********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const double dt, CSpatialPartition* sp)
{
	if (!MoveVel.IsZero())
	{
		if (myKeys['w'] == false && myKeys['s'] == false
			&& myKeys['a'] == false && myKeys['d'] == false)
		{
			Vector3 preMoveVel = MoveVel;

			MoveVel += MoveVel.Normalized() * -MOVE_ACC * dt;

			if ((preMoveVel.x < 0 && MoveVel.x > 0)
				|| (preMoveVel.x > 0 && MoveVel.x < 0))
				MoveVel.x = 0;
			if ((preMoveVel.z < 0 && MoveVel.z > 0)
				|| (preMoveVel.z > 0 && MoveVel.z < 0))
				MoveVel.z = 0;
		}

		// Cap move velocity at max
		if (MoveVel.Length() > 50.f)
			MoveVel = MoveVel.Normalized() * 50.f;

		// Update the avatar position
		curPosition += MoveVel * dt;

		// Update the avatar node
		theAvatarNode->ApplyTranslate(MoveVel.x * dt, 0, MoveVel.z * dt);

		// Check collision with objects
		if (CheckCollision(sp, 1))
		{
			curPosition -= MoveVel * dt;
			theAvatarNode->ApplyTranslate(-MoveVel.x * dt, 0, -MoveVel.z * dt);
			MoveVel.SetZero();
		}
		else
		{
			sp->CheckObjectGrid(theAvatarNode);
		}
	}
}

/********************************************************************************
Check Collision
********************************************************************************/
bool CPlayInfo3PV::CheckCollision(CSpatialPartition* sp, int hitpoints)
{
	if (sp->CheckForCollision(theAvatarNode, hitpoints))
	{
		return true;
	}
	// Front
	if (curPosition.z >= 1000.f - 5.5f)
	{
		return true;
	}
	// Back
	else if (curPosition.z <= 0.f + 5.5f)
	{
		return true;
	}
	// Left
	else if (curPosition.x >= 1000.f - 5.5f)
	{
		return true;
	}
	// Right
	else if (curPosition.x <= 0.f + 5.5f)
	{
		return true;
	}
	return false;
}

/********************************************************************************
Update bottom
********************************************************************************/
void CPlayInfo3PV::UpdateBottom(const double dt, const Vector3 prev_RotationVel)
{
	//Not moving
	// Up Down
	if (prev_RotationVel.x == RotateVel.x)
	{
		// Set rotate velocity to zero if haven't
		if (RotateVel.x != 0)
			RotateVel.x = 0;

		// Record old to rotate amount
		float prev_rotateX = bottomRotationLeftRightUpDown.x;
		// Check if to rotate up
		if (bottomRotationLeftRightUpDown.x < 0)
		{
			bottomRotationLeftRightUpDown.x += ROTATE_ACC * 20 * dt * dt;
		}
		// else to rotate down
		else
		{
			bottomRotationLeftRightUpDown.x += -ROTATE_ACC * 20 * dt * dt;
		}

		// if reach 0 or more set it to zero
		if ((prev_rotateX < 0 && bottomRotationLeftRightUpDown.x > 0)
			|| (prev_rotateX > 0 && bottomRotationLeftRightUpDown.x < 0))
		{
			bottomRotationLeftRightUpDown.x = 0;
		}
		// Get new to rotate amount
		float new_RotateX = bottomRotationLeftRightUpDown.x - prev_rotateX;
		// Update the avatar node
		theAvatarNode->GetNode(11)->ApplyRotate(new_RotateX, 1, 0, 0);
	}
	// Left Right
	if (prev_RotationVel.z == RotateVel.z)
	{
		// Set rotate velocity to zero if haven't
		if (RotateVel.z != 0)
			RotateVel.z = 0;
		// Record old to rotate amount
		float prev_rotateZ = bottomRotationLeftRightUpDown.z;
		// Check if to rotate right
		if (bottomRotationLeftRightUpDown.z < 0)
		{
			bottomRotationLeftRightUpDown.z += ROTATE_ACC * 20 * dt * dt;
		}
		// else to rotate left
		else
		{
			bottomRotationLeftRightUpDown.z += -ROTATE_ACC * 20 * dt * dt;
		}

		// if reach 0 or more set it to zero
		if ((prev_rotateZ < 0 && bottomRotationLeftRightUpDown.z > 0)
			|| (prev_rotateZ > 0 && bottomRotationLeftRightUpDown.z < 0))
		{
			bottomRotationLeftRightUpDown.z = 0;
		}
		// Get new to rotate amount
		float new_RotateZ = bottomRotationLeftRightUpDown.z - prev_rotateZ;
		// Update the avatar node
		theAvatarNode->GetNode(11)->ApplyRotate(new_RotateZ, 0, 0, 1);
	}
	// If Moving
	if (!RotateVel.IsZero())
	{
		// Cap rotate velocity at max
		if (RotateVel.Length() > 50.f)
			RotateVel = RotateVel.Normalized() * 50.f;

		// Up Down
		if (RotateVel.x != 0.f
			&& bottomRotationLeftRightUpDown.x > -20.f
			&& bottomRotationLeftRightUpDown.x < 20.f)
		{
			// Record old to rotate amount
			float prev_rotateX = bottomRotationLeftRightUpDown.x;
			// Add  rotate amount to bottomRotationLeftRightUpDown
			bottomRotationLeftRightUpDown.x += RotateVel.x * dt;
			// Backward
			if (bottomRotationLeftRightUpDown.x > 20.f)
			{
				bottomRotationLeftRightUpDown.x = 20.f;
			}
			// Forward
			else if (bottomRotationLeftRightUpDown.x < -20.f)
			{
				bottomRotationLeftRightUpDown.x = -20.f;
			}
			// Get new to rotate amount
			float new_RotateX = bottomRotationLeftRightUpDown.x - prev_rotateX;
			// Update the avatar node
			theAvatarNode->GetNode(11)->ApplyRotate(new_RotateX, 1, 0, 0);
		}
		// Left Right
		if (RotateVel.z != 0.f
			&& bottomRotationLeftRightUpDown.z > -50.f
			&& bottomRotationLeftRightUpDown.z < 50.f)
		{
			// Record old to rotate amount
			float prev_rotateZ = bottomRotationLeftRightUpDown.z;
			// Add  rotate amount to bottomRotationLeftRightUpDown
			bottomRotationLeftRightUpDown.z += RotateVel.z * dt;
			// Backward
			if (bottomRotationLeftRightUpDown.z > 20.f)
			{
				bottomRotationLeftRightUpDown.z = 20.f;
			}
			// Forward
			else if (bottomRotationLeftRightUpDown.z < -20.f)
			{
				bottomRotationLeftRightUpDown.z = -20.f;
			}
			// Get new to rotate amount
			float new_RotateZ = bottomRotationLeftRightUpDown.z - prev_rotateZ;
			// Update the avatar node
			theAvatarNode->GetNode(11)->ApplyRotate(new_RotateZ, 0, 0, 1);
		}
	}
}

/********************************************************************************
Update Camera
********************************************************************************/
void CPlayInfo3PV::UpdateCamera(const double dt, const Vector3 prev_Position, const Vector3 prev_CharDirection)
{
	if (moving == false)
	{
		if (newDirection != curCamDirection)
		{
			Vector3 prevNewDirection = newDirection;

			newDirection += (curCamDirection - newDirection).Normalized() * 2 * dt;

			if ((curCamDirection - newDirection).Length() > (curCamDirection - prevNewDirection).Length())
			{
				newDirection = curCamDirection;
			}
		}
		else
		{
			newDirection = curCamDirection;
		}

		if (newPosition != curPosition)
		{
			Vector3 prevNewPosition = newPosition;

			newPosition += (curPosition - newPosition).Normalized() * MOVE_CAMERA_SPEED * dt;

			if ((curPosition - newPosition).Length() > (curPosition - prevNewPosition).Length())
			{
				newPosition = curPosition;
			}
		}
		else
		{
			newPosition = curPosition;
		}
	}
	else
	{
		if (camAtRight == true)
		{
			UpdateCameraToRight();
		}
		else
		{
			UpdateCameraToLeft();
		}

		if (newDirection != maxNewDirection)
		{
			Vector3 prevNewDirection = newDirection;

			newDirection += (maxNewDirection - newDirection).Normalized() * 2 * dt;

			if ((maxNewDirection - newDirection).Length() > (maxNewDirection - prevNewDirection).Length())
			{
				newDirection = maxNewDirection;
			}
		}
		else
		{
			newDirection = maxNewDirection;
		}

		if (newPosition != maxNewPosition)
		{
			Vector3 prevNewPosition = newPosition;

			newPosition += (maxNewPosition - newPosition).Normalized() * MOVE_CAMERA_SPEED * dt;

			if ((maxNewPosition - newPosition).Length() > (maxNewPosition - prevNewPosition).Length())
			{
				newPosition = maxNewPosition;
			}
		}
		else
		{
			newPosition = maxNewPosition;
		}
	}
}


/********************************************************************************
Update Camera to Right
********************************************************************************/
void CPlayInfo3PV::UpdateCameraToRight(void)
{
	Vector3 right = curCharDirection.Cross(Vector3(0, 1, 0));
	right.y = 0;
	right.Normalize();

	Mtx44 rotation;

	rotation.SetToRotation(2.5f, 0, 1, 0);
	maxNewDirection = rotation * curCharDirection;

	rotation.SetToRotation(-2.f, right.x, right.y, right.z);
	maxNewDirection = rotation * maxNewDirection;

	maxNewPosition = curPosition + curCharDirection.Normalized() * 15.f + right * 5.f + Vector3(0, 4.f, 0);


	// Set camera look 45 degree downward
	rotation.SetToRotation(-45, right.x, right.y, right.z);
	curCamDirection = rotation * curCharDirection;
}

/********************************************************************************
Update Camera to Left
********************************************************************************/
void CPlayInfo3PV::UpdateCameraToLeft(void)
{
	Vector3 right = curCharDirection.Cross(Vector3(0, 1, 0));
	right.y = 0;
	right.Normalize();

	Mtx44 rotation;

	rotation.SetToRotation(-2.5f, 0, 1, 0);
	maxNewDirection = rotation * curCharDirection;

	rotation.SetToRotation(-2.f, right.x, right.y, right.z);
	maxNewDirection = rotation * maxNewDirection;

	maxNewPosition = curPosition + curCharDirection.Normalized() * 15.f + right * -5.f + Vector3(0, 4.f, 0);


	// Set camera look 45 degree downward
	rotation.SetToRotation(-45, right.x, right.y, right.z);
	curCamDirection = rotation * curCharDirection;
}