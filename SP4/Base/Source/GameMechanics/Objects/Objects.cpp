#include "Objects.h"

//bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh
CObjects::CObjects()
	: active(false)
	, interactable(false)
	, dialogue("")
	, position(0,0,0)
	, rotate(0,0,0)
	, scale(0,0,0)
	, ModelMesh(NULL)
	, BoundingBox(NULL)
{
}
CObjects::CObjects(bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh)
{
	this->active = active;
	this->interactable = interactable;
	this->dialogue = dialogue;
	
	defaultPos = position;	
	this->position = position;

	defaultRot = rotate;
	this->rotate = rotate;

	defaultScale = scale;
	this->scale = scale;

	this->ModelMesh = NULL;							//initialize this first
	this->ModelMesh = ModelMesh;
	
	BoundingBox = NULL;								//set up a bounding box in case there is a need to use it
}
//bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, bounding box
CObjects::~CObjects()
{
	if (BoundingBox)
	{
		delete BoundingBox;
		BoundingBox = NULL;
	}
}
//set and get the entity's active status
void CObjects::setActive(bool active)
{
	this->active = active;
}
bool CObjects::getActive(void)
{
	return active;
}
//set and get the entity's interactivity status
void CObjects::setInteractivity(bool interactable)
{
	this->interactable = interactable;
}
bool CObjects::getInteractivity(void)
{
	return interactable;
}
//set and get the dialogue of entity
void CObjects::setDialogue(std::string dialogue)
{
	this->dialogue = dialogue;
}
std::string CObjects::getDialogue(void)
{
	return dialogue;
}
//set and get the position of the entity
void CObjects::setPosition(Vector3 position)
{
	this->position = position;
}
Vector3 CObjects::getPosition(void)
{
	return position;
}
//set and get the position X of the entity
void CObjects::setPositionX(float x)
{
	position.x = x;
}
float CObjects::getPositionX(void)
{
	return position.x;
}
//set and get the position X of the entity
void CObjects::setPositionY(float y)
{
	position.y = y;
}
float CObjects::getPositionY(void)
{
	return position.y;
}
//set and get the scale of the entity
void CObjects::setScale(Vector3 scale)
{
	this->scale = scale;
}
Vector3 CObjects::getScale()
{
	return scale;
}
//set and get the scale X of entity
void CObjects::setScaleX(float x)
{
	scale.x = x;
}
float CObjects::getScaleX(void)
{
	return scale.x;
}
//set and get the scale Y of entity
void CObjects::setScaleY(float y)
{
	scale.y = y;
}
float CObjects::getScaleY(void)
{
	return scale.y;
}
//set and get the sprite of entity
void CObjects::setMesh(Mesh* ModelMesh)
{
	this->ModelMesh = ModelMesh;
}
Mesh* CObjects::getMesh(void)
{
	return ModelMesh;
}

//set and get the bounding box of entity. if needed.
void CObjects::setBoundingBox(Vector3 topleft, Vector3 bottomright)
{
	//set up a bounding box in case there is a need to use it
	if (BoundingBox == NULL)
		BoundingBox = new CBoundingBox(topleft, bottomright);		
	else
		return;
}
CBoundingBox* CObjects::getBoundingBox(void)
{
	return BoundingBox;
}