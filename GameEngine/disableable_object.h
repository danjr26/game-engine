#ifndef DISABLEABLE_OBJECT_H
#define DISABLEABLE_OBJECT_H

class DisableableObject {
private:
	bool mIsEnabled;
public:
	DisableableObject();
	void enable();
	void disable();
	bool isEnabled();
};

#endif