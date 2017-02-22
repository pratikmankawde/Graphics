#include "KeyboardActionHandler.h"
namespace action_handlers {

bool
KeyboardActionHandler::handleAction() {


	return false;
}

bool
KeyboardActionHandler::handleNormalKeyPressEvent(unsigned char aKey, int aMouseXPos, int aMouseYPos) {
    switch (aKey) {
		case 'a':
			setOperationMode(ActionHandler::OperationMode::ANIMATING);
			break;
		case 'r':
			setOperationMode(ActionHandler::OperationMode::RIGGING);
			break;
		case 'v':
			setOperationMode(ActionHandler::OperationMode::VIEWING);
			break;
    }


 return true;
}

//
//void processNormalKeys(unsigned char key, int x, int y) {
//
//    switch (key) {
//			case '+':
//				zoom /= 0.75;
//				glutPostRedisplay();
//				break;
//			case '-':
//				zoom *= 0.75;
//				glutPostRedisplay();
//				break;
//    }
//
//}
//
//void processSpecialKeys(int key, int x, int y) {
//
//	switch(key) {
//		case GLUT_KEY_F1 :
//				red = 1.0;
//				green = 0.0;
//				blue = 0.0; break;
//		case GLUT_KEY_F2 :
//				red = 0.0;
//				green = 1.0;
//				blue = 0.0; break;
//		case GLUT_KEY_F3 :
//				red = 0.0;
//				green = 0.0;
//				blue = 1.0; break;
//	}
//}
//
}
