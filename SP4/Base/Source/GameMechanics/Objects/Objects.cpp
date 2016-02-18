#include "Objects.h"


CObjects::CObjects()
	: position(0,0,0)
	, interactable(false)
	, velocity(0.f)
	, m_cModelMesh(NULL)
	, dialogue("")
{
}
CObjects::~CObjects()
{
	/*if (m_cModelMesh)
	{
		delete m_cModelMesh;
		m_cModelMesh = NULL;
	}*/
}
//initialise the entity's items
void CObjects::Init(Vector3 position, bool interactable, float velocity, std::string dialogue, Mesh* m_cModelMesh)
{
	this->position = position;
	this->interactable = interactable;
	this->velocity = velocity;
	this->dialogue = dialogue;
	this->m_cModelMesh = m_cModelMesh;
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
//set and get the interactivity of entity
void CObjects::setInteractable(bool interactable)
{
	this->interactable = interactable;
}
bool CObjects::getInteractable(void)
{
	return interactable;
}
//set and get the velocity of entity
void CObjects::setVelocity(float velocity)
{
	this->velocity = velocity;
}
float CObjects::getVelocity(void)
{
	return velocity;
}
//set and get the sprite of entity
void CObjects::setMesh(Mesh* m_cModelMesh)
{
	this->m_cModelMesh = m_cModelMesh;
}
Mesh* CObjects::getMesh(void)
{
	return m_cModelMesh;
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
void CObjects::setScale(Vector3 scale)
{
	this->scale = scale;
}

Vector3 CObjects::getScale()
{
	return scale;
}