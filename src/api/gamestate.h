#pragma once

#include <string>

class gamestate {
public:
	// gamestate() = delete;
	// gamestate(const std::string &_name) : name(_name)
	// {}

	virtual ~gamestate() {}

	virtual  int  Create() = 0;
	virtual void Destroy() = 0;

	virtual void InputHandle(const void *data_handle) = 0;
	
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	virtual bool isRunning() = 0;

protected:
	std::string name;
};
