#pragma once
#include "StaticMeshNode.h"
class StaticMeshPlane : public StaticMeshNode{
public:
	StaticMeshPlane(Vector3 startPosition, std::string name, float width, float height);

	virtual void Draw() const override;

protected:
	virtual void GenMesh() override;

	float width;
	float length;
};

