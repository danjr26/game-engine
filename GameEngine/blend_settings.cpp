#include "blend_settings.h"

BlendSettings::BlendSettings(Factor in_destinationFactor, Factor in_sourceFactor, 
	Operation in_operation) :
	mDestinationFactor(in_destinationFactor),
	mSourceFactor(in_sourceFactor),
	mOperation(in_operation)
{}

void BlendSettings::use(const BlendSettings* in_currentSettings) {
	if(in_currentSettings == nullptr) {
		glBlendFunc((GLenum)mSourceFactor, (GLenum)mDestinationFactor);
		glBlendEquation((GLenum)mOperation);
	}
	else {
		if (mDestinationFactor != in_currentSettings->mDestinationFactor || 
			mSourceFactor != in_currentSettings->mSourceFactor) {

			glBlendFunc((GLenum)mSourceFactor, (GLenum)mDestinationFactor);
		}
		if (mOperation != in_currentSettings->mOperation) {
			glBlendEquation((GLenum)mOperation);
		}
	}
}

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
	return BlendSettings(Factor::one, Factor::src_a, Operation::subtract);
}

BlendSettings BlendSettings::multiply() {
	return BlendSettings(Factor::one, Factor::one, Operation::multiply);
}

BlendSettings BlendSettings::custom(Factor in_destinationFactor, Factor in_sourceFactor, 
	Operation in_operation) {
	return BlendSettings(in_destinationFactor, in_sourceFactor, in_operation);
}


