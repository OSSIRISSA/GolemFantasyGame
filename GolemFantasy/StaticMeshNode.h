#pragma once
#include <string>
#include "Node.h"
class StaticMeshNode : public Node{
public:
    StaticMeshNode(Vector3 startPosition, std::string name);
    StaticMeshNode(std::string name);
    virtual ~StaticMeshNode();

    virtual void Update(float delta) override;
    virtual void Draw() const override;

protected:
    virtual void GenMesh();

    Model model;
    float radius;
};

