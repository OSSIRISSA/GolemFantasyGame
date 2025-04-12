#pragma once
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"

class Node {
public:
	Node(const std::string& name);
	virtual ~Node();

	void AddChild(std::shared_ptr<Node> child);
	bool RemoveChild(const std::string& name);
	const std::vector<std::shared_ptr<Node>>& GetChildren() const;

	virtual void Update(float deltaTime);
	virtual void Draw() const;

	void SetPosition(Vector3 position);
	Vector3 GetPosition() const;

	void SetRotation(Quaternion rotation);
	Quaternion GetRotation() const;

	void SetScale(Vector3 scale);
	Vector3 GetScale() const;

protected:
	std::string m_name;

	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;

	std::vector<std::shared_ptr<Node>> m_children;
};