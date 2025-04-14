#pragma once
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"

class Node{
public:
	Node(const std::string& name);
	virtual ~Node();

	void AddChild(std::shared_ptr<Node> child);
	bool RemoveChild(const std::string& name);
	const std::vector<std::shared_ptr<Node>>& GetChildren() const;
	//const std::shared_ptr<Node>& GetParent() const;

	virtual void Update(float deltaTime);
	virtual void Draw() const;

	void SetPosition(Vector3 newWorldPosition);
	void Move(Vector3 addPosition);
	Vector3 GetPosition() const;

	void SetRotation(Quaternion newWorldRotation);
	void Rotate(Quaternion addRotation);
	Quaternion GetRotation() const;

	void SetScale(Vector3 newWorldScale);
	void Scale(Vector3 multScale);
	void Scale(float multScale);
	Vector3 GetScale() const;

protected:
	std::string name;

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	//std::shared_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> children;
};