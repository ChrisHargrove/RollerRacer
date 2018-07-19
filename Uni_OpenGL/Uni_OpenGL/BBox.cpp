#include "BBox.h"



BBox::BBox(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, float mass, std::string shader) :
    BGameObject(position, rotation, mass, new btBoxShape(btVector3(size.x,size.y,size.z)), shader)
{
    _Model = new Cuboid(size.x * 2, size.y * 2, size.z * 2, shader);
}


BBox::~BBox()
{
    delete _Model;
}

void BBox::Input()
{
}

void BBox::Update(float delta)
{
}

void BBox::Render(std::string shader)
{
    _Model->Render(shader);
}

void BBox::SetColor(float r, float g, float b)
{
    _Model->SetColor(glm::vec3(r, g, b));
}

void BBox::SetColor(glm::vec3 color)
{
    _Model->SetColor(color);
}
