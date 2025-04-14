#pragma once
#include "Node.h"
#include "raymath.h"
#include "raylib.h"

Node::Node(const std::string& name)
	: name(name), position({ 0.0f, 0.0f, 0.0f }), rotation(Quaternion()), scale({ 1.0f, 1.0f, 1.0f }) {
}

Node::~Node() {
	children.clear();
}

void Node::AddChild(std::shared_ptr<Node> child) {
	if (child != nullptr) { 
		//child->parent = std::make_shared<Node>(this);
		children.push_back(child); 
	}
}

bool Node::RemoveChild(const std::string& name) {
	for (auto it = children.begin(); it != children.end(); ++it) {
		if ((*it)->name == name) {
			//(*it)->parent = nullptr;
			children.erase(it);
			return true;
		}
	}
	return false;
}

const std::vector<std::shared_ptr<Node>>& Node::GetChildren() const { return children; }
//const std::shared_ptr<Node>& Node::GetParent() const { return parent; }

void Node::Update(float deltaTime) {
	for (auto& child : children) { child->Update(deltaTime); }
}

void Node::Draw() const {
	for (auto& child : children) { child->Draw(); }
}


// --- POSITION ACTIONS --- 
void Node::SetPosition(Vector3 newWorldPosition) { 
	Vector3 delta = newWorldPosition - position;
	for (auto& child : children) {
		child->Move(delta);
	}
	position = newWorldPosition;
}

void Node::Move(Vector3 addPosition) {
	for (auto& child : children) {
		child->Move(addPosition);
	}
	position += addPosition;
}

Vector3 Node::GetPosition() const { return position; }


// --- ROTATION ACTIONS --- 
void Node::SetRotation(Quaternion newWorldRotation) {
	Quaternion delta = newWorldRotation - rotation;
	for (auto& child : children) {
		child->Rotate(delta);
	}
	rotation = newWorldRotation;
}

void Node::Rotate(Quaternion addRotation) {
	for (auto& child : children) {
		child->Rotate(addRotation);
	}
	rotation += addRotation;
}

Quaternion Node::GetRotation() const { return rotation; }


// --- SCALE ACTIONS ---
void Node::SetScale(Vector3 newWorldScale) {
	Vector3 delta = newWorldScale / position;
	for (auto& child : children) {
		child->Move(delta);
	}
	scale = newWorldScale;
}

void Node::Scale(Vector3 multScale) {
	for (auto& child : children) {
		child->Scale(multScale);
	}
	scale *= multScale;
}

void Node::Scale(float multScale) {
	for (auto& child : children) {
		child->Scale(multScale);
	}
	scale *= multScale;
}

Vector3 Node::GetScale() const { return scale; }