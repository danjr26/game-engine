#include "framework.h"

int WINAPI WinMain(HINSTANCE in_hInst, HINSTANCE in_hPrevInst, LPSTR arg, int nArgs) {
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);
	new GEngine(in_hInst);
	/*
	Level1Script();

	SwarmDroid* sd = new SwarmDroid(Transform3d(Vector3d(0, 0, -20), Orientation3d()), SwarmDroid::defsettings);
	Clock c;
	double t = c.Time();
	for (int i = 0; i < 10000000; i++) {
		Avoid_Entities(sd, nullptr, 0);
	}
	Die(to_string((c.Time() - t) * 0.1) + " us");
	*/

	GEngine::Get().TRQ().Updater_Thread_Entry();
}