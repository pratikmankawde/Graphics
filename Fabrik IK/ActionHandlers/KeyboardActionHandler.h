
#include "ActionHandler.h"

namespace action_handlers {

class KeyboardActionHandler : public ActionHandler {

public:
	bool handleAction();
	bool handleNormalKeyPressEvent(unsigned char aKey, int aMouseXPos, int aMouseYPos);
	bool revertAction() { return true; };

};
}
