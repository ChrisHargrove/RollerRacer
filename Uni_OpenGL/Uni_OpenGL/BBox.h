#pragma once

#include "BGameObject.h"
#include "Cuboid.h"

#include <string>

class BBox : public BGameObject
{
public:
    BBox(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, float mass, std::string shader);
    ~BBox();

    // Inherited via BGameObject
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render(std::string shader = "") override;

    void SetColor(float r, float g, float b);
    void SetColor(glm::vec3 color);

private:
    Cuboid* _Model;
    glm::vec3 _Color;
};

