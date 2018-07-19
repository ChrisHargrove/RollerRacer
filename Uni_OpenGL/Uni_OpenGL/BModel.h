#pragma once

#include "BGameObject.h"
#include "Model.h"

#include <string>

class BModel : public BGameObject
{
public:
    BModel(glm::vec3 position, glm::vec3 rotation, float mass, Model* model, std::string shader);
    ~BModel();

    // Inherited via BGameObject
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render(std::string shader = "") override;

    void Scale(glm::vec3 scale);

private:
    Model* _Model;

};

