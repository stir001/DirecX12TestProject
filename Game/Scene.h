#pragma once

class DxInput;

class Scene
{
public:
	Scene();
	~Scene();
	virtual void Initialize() = 0;
	virtual void Terminate() = 0;
	virtual void Update(const DxInput& input) = 0;
	virtual void Draw() = 0;
};

