#pragma once
#include "Node.h"

Node::Node(const std::string& name)
	: m_name(name), m_position({ 0.0f, 0.0f, 0.0f }), m_rotation(Quaternion()), m_scale({ 1.0f, 1.0f, 1.0f }) {
}

Node::~Node() {
	m_children.clear();
}

void Node::AddChild(std::shared_ptr<Node> child) {
	if (child != nullptr) { m_children.push_back(child); }
}

bool Node::RemoveChild(const std::string& name) {
	for (auto it = m_children.begin(); it != m_children.end(); ++it) {
		if ((*it)->m_name == name) {
			m_children.erase(it);
			return true;
		}
	}
	return false;
}

void Node::Update(float deltaTime) {
	for (auto& child : m_children) { child->Update(deltaTime); }
}

void Node::Draw() const {
	for (auto& child : m_children) { child->Draw(); }
}

void Node::SetPosition(Vector3 position) { m_position = position; }

Vector3 Node::GetPosition() const { return m_position; }

void Node::SetRotation(Quaternion rotation) { m_rotation = rotation; }

Quaternion Node::GetRotation() const { return m_rotation; }

void Node::SetScale(Vector3 scale) { m_scale = scale; }

Vector3 Node::GetScale() const { return m_scale; }

const std::vector<std::shared_ptr<Node>>& Node::GetChildren() const { return m_children; }