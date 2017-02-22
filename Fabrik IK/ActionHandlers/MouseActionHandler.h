
#include "ActionHandler.h"
#include "../Elements/Vec3.h"

using namespace elements;

namespace action_handlers {

class MouseActionHandler : public ActionHandler {

private:

	Vec3<float> mLastPosition;
	Vec3<float> mCurrentPosition;
	Vec3<float> mDiffPosition;

public:
	bool handlerAction() {
		return true;
	}
	bool handleClickAction(int aButton, int aState, int aPosX, int aPosY, int aPosZ);
	bool handleMoveAction(int aNewPosX, int aNewPosY, int aNewPosZ);

	bool revertAction() { return true; };
	void setControllerFactory(ControllerFactory* aControllerFactory) { mControllerFactory = aControllerFactory; }
	void setOperationDataFactory(OperationDataFactory* aOperationDataFactory) { mOperationDataFactory = aOperationDataFactory; }
};

}
