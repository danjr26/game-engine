#ifndef DISABLEABLE_OBJECT_H
#define DISABLEABLE_OBJECT_H

class DisableableObject {
private:
	bool mIsEnabled;
public:
	DisableableObject();
	void Enable();
	void Disable();
	bool Is_Enabled();
};

#endif