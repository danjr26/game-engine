#ifndef TRANSITION_TRACKER_H
#define TRANSITION_TRACKER_H

#include "progress_tracker.h"

class TransitionTracker {
public:
	enum class State {
		off,
		offToOn,
		on,
		onToOff
	};
private:
	State state;
	ProgressTracker	progressTracker;
	double offToOnDuration;
	double onToOffDuration;

public:
	TransitionTracker(double in_offToOnDuration, double in_onToOffDuration);
	void Update(double in_newAccumulation);
	void Turn_On();
	void Turn_Off();
	void Toggle();
	bool Is_Transitioning() const;
	State Get_State() const;
	double Get_Turn_On_Duration() const;
	double Get_Turn_Off_Duration() const;
	double Get_Progress_Accumulation() const;
	void Set_State(State in_state);
	void Set_Turn_On_Duration(double in_offToOnDuration);
	void Set_Turn_Off_Duration(double in_onToOffDuration);
	void Set_Progress_Accumulation(double in_progressAccumulation);

	template<class T>
	T Lin_Interp(T in_a, T in_b) const {
		switch (state) {
		case TransitionStates::on:
			return in_b;
		case TransitionStates::off:
			return in_a;
		case TransitionStates::offToOn:
			return progressTracker.Lin_Interp(in_a, in_b);
		case TransitionStates::onToOff:
			return progressTracker.Lin_Interp(in_b, in_a);
		}
		return in_a;
	}
};


#endif