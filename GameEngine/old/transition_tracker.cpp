#include "transition_tracker.h"

TransitionTracker::TransitionTracker(double offont, double onofft) :
	state	(State::off),
	progressTracker	(offont, true),
	offToOnDuration	(offont),
	onToOffDuration	(onofft) 
{}

void TransitionTracker::Update(double in_newAccumulation) {
	switch (state) {
	case State::off:
	case State::on:
		break;
	case State::offToOn:
		progressTracker.Update(in_newAccumulation);
		if (progressTracker.Is_Done())
			state = State::on;
		break;
	case State::onToOff:
		progressTracker.Update(in_newAccumulation);
		if (progressTracker.Is_Done())
			state = State::off;
		break;
	}
}

void TransitionTracker::Turn_On() {
	switch (state) {
	case State::on:
	case State::offToOn:
		break;
	case State::off:
		state = State::offToOn;
		progressTracker = ProgressTracker(offToOnDuration, true);
		break;
	case State::onToOff:
		state = State::offToOn;
		progressTracker = ProgressTracker(offToOnDuration, true);
		progressTracker.accumulation = offToOnDuration * progressTracker.Inv_Progress();
		break;
	}
}

void TransitionTracker::Turn_Off() {
	switch (state) {
	case State::off:
	case State::onToOff:
		break;
	case State::on:
		state = State::onToOff;
		progressTracker = ProgressTracker(onToOffDuration, true);
		break;
	case State::offToOn:
		state = State::onToOff;
		progressTracker = ProgressTracker(onToOffDuration, true);
		progressTracker.accumulation = onToOffDuration * progressTracker.Inv_Progress();
		break;
	}
}

void TransitionTracker::Toggle() {
	switch (state) {
	case State::on:
	case State::offToOn:
		Turn_Off();
		break;
	case State::off:
	case State::onToOff:
		Turn_On();
		break;
	}
}

bool TransitionTracker::Is_Transitioning() const {
	return state == State::offToOn || state == State::onToOff;
}

Flags TransitionTracker::Get_State() const {
	return state;
}

double TransitionTracker::Get_Turn_On_Duration() const {
	return offToOnDuration;
}

double TransitionTracker::Get_Turn_Off_Duration() const {
	return onToOffDuration;
}

double TransitionTracker::Get_Progress_Accumulation() const {
	return progressTracker.accumulation;
}

void TransitionTracker::Set_State(Flags in_state) {
	state = in_state;
	progressTracker.accumulation = 0.0;
	Update(0.0);
}

void TransitionTracker::Set_Turn_On_Duration(double in_offToOnDuration) {
	offToOnDuration = in_offToOnDuration;
	Update(0.0);
}

void TransitionTracker::Set_Turn_Off_Duration(double in_onToOffDuration) {
	onToOffDuration = in_onToOffDuration;
	Update(0.0);
}

void TransitionTracker::Set_Progress_Accumulation(double in_progressAccumulation) {
	progressTracker.accumulation = in_progressAccumulation;
	Update(0.0);
}