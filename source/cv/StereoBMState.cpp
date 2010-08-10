#include "StereoBMState.hpp"
#include "Image.hpp"

namespace Cv {

StereoBMState::StereoBMState(int presetFlag, int numberOfDisparities):
	state(0)
{
	state = cvCreateStereoBMState(presetFlag, numberOfDisparities);
}

StereoBMState::~StereoBMState()
{
	fprintf(stderr, "StereoBMState::~StereoBMState() NOT deallocing! Fixme!!\n");
	//cvReleaseBMState(&state); // TODO: Why the hell won't this compile???
}

void StereoBMState::findCorrespondence(Image* left, Image* right, Image* ret)
{
	cvFindStereoCorrespondenceBM(left->getPtr(), right->getPtr(), ret->getPtr(), 
					state);
}

} // end namespace Cv
