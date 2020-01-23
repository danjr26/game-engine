#include "../include/internal/blend_settings.h"

BlendSettings::BlendSettings(Factor i_destinationFactor, Factor i_sourceFactor, 
	Operation i_operation) :
	mDestinationFactor(i_destinationFactor),
	mSourceFactor(i_sourceFactor),
	mOperation(i_operation)
{}

BlendSettings::BlendSettings() :
	mDestinationFactor(Factor::inv_src_a),
	mSourceFactor(Factor::src_a),
	mOperation(Operation::add)
{}

/*
void BlendSettings::use(const BlendSettings* i_currentSettings) {
	if(i_currentSettings == nullptr) {
		glBlendFunc((GLenum)mSourceFactor, (GLenum)mDestinationFactor);
		glBlendEquation((GLenum)mOperation);
	}
	else {
		if (mDestinationFactor != i_currentSettings->mDestinationFactor || 
			mSourceFactor != i_currentSettings->mSourceFactor) {

			glBlendFunc((GLenum)mSourceFactor, (GLenum)mDestinationFactor);
		}
		if (mOperation != i_currentSettings->mOperation) {
			glBlendEquation((GLenum)mOperation);
		}
	}
}
*/

BlendSettings BlendSettings::none() {
	return BlendSettings(Factor::zero, Factor::one, Operation::add);
}

BlendSettings BlendSettings::standard() {
	return BlendSettings(Factor::inv_src_a, Factor::src_a, Operation::add);
}

BlendSettings BlendSettings::additive() {
	return BlendSettings(Factor::one, Factor::src_a, Operation::add);
}

BlendSettings BlendSettings::subtractive() {
	return BlendSettings(Factor::one, Factor::src_a, Operation::inv_subtract);
}

BlendSettings BlendSettings::multiply() {
	return BlendSettings(Factor::src_rgba, Factor::zero, Operation::add);
}

BlendSettings BlendSettings::custom(Factor i_destinationFactor, Factor i_sourceFactor, 
	Operation i_operation) {
	return BlendSettings(i_destinationFactor, i_sourceFactor, i_operation);
}


