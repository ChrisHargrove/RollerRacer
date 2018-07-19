#include "BModel.h"
#include "RigidBodyFromOBJ.h"



BModel::BModel(glm::vec3 position, glm::vec3 rotation, float mass, Model* model, std::string shader) :
    BGameObject(position, rotation, mass, CreateShape(model), shader),
    _Model(model)
{

}


BModel::~BModel()
{
    delete _Model;
}

void BModel::Input()
{
}

void BModel::Update(float delta)
{
}

void BModel::Render(std::string shader)
{
    _Model->Render(shader);
}

void BModel::Scale(glm::vec3 scale)
{
    BGameObject::Scale(scale);
}
