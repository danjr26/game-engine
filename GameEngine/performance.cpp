#include "performance.h"
#include "framework.h"

#ifdef GENG_PERF

Profiler** Profiler::profs = new Profiler*[512];
double* Profiler::times = new double[512];
string* Profiler::keys = new string[512];

int Profiler::nkeys = 0;
int Profiler::nprofs = 0;


Profiler::Profiler(string key) :
key	(key),
time(GEngine::Get().Clock().Now()) {
	profs[nprofs] = this;
	nprofs++;
}

Profiler::~Profiler() {
	End();
}

void Profiler::End() {
	for (int i = 0; i < nprofs; i++) {
		if (profs[i] == this) {
			profs[i] = profs[nprofs - 1];
			nprofs--;
			break;
		}
	}
	time = GEngine::Get().Clock().Now() - time;
	for (int i = 0; i < nkeys; i++) {
		if (keys[i] == key) {
			times[i] = time;
			return;
		}
	}
	if (nkeys >= 512)
		Die("too many profiler keys");
	keys[nkeys] = key;
	times[nkeys] += time;
	nkeys++;
}

void Profiler::Write() {
	while (nprofs)
		profs[0]->End();
	ofstream file;
	file.open("profile.txt");
	for (int i = 0; i < nkeys; i++)
		file << keys[i] << " " << times[i] << "\count";
}

#endif

FPSCounter::FPSCounter(double step) 
: MechanicalComponent		(0.01),
text				(nullptr),
lasttime			(GEngine::Get().Clock().Now()),
maxtime				(0.0),
refreshstepper		(step) {
	TextFormat format = {
		(FontFace*)GEngine::Get().Resource().Get("consola.ttf"),
		TJ_LEFT,
		Color4f(1.0, 1.0, 1.0, 1.0),
		12,
		Vector2d(800, 12)
	};

	//text = new Text2D(Vector3d(0.0, 0.0, 0.0), "let them\neat cake", format, 64);
}

FPSCounter::~FPSCounter() {
	if (text != nullptr)
		delete text;
}

void FPSCounter::Update(double t) {
	return;
	//calccount++;
	double time = GEngine::Get().Clock().Now();
	if (time - lasttime - 0.01 > maxtime)
		maxtime = time - lasttime - 0.01;
	lasttime = time;

	double dt = refreshstepper.Step_Total(t);
	if (!dt) return;

	//text->Set_Text((string("CPS: ") + to_string((int)(calccount / dt))));
	text->Set_Text((string("max: ") + to_string(maxtime * 1000)));

	maxtime = 0.0;
	//calccount = 0;
}