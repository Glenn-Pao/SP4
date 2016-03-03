#include "Image.h"


Image::Image()
{
}
Image::Image(string ID, Mesh* mesh, Vector3 CurrentPos, Vector3 Scale)
{
	//Parent class variables
	this->setID(ID);
	this->setCurrentPos(CurrentPos);
	this->setDefaultPos(CurrentPos);
	this->setScale(Scale);
	this->setUI_Type(UIFeature::UT_IMAGE);

	//Child class variables
	this->mesh = mesh;
}

void Image::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* Image::getMesh()
{
	return mesh;
}

Image::~Image()
{
}
