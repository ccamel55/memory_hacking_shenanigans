#include "InputManager.h"

void C_InputManager::onWndProcStart(UINT uMsg, WPARAM wParam, LPARAM lParam) {

	uCurKey = 0;
	bCurState = false;

	switch (uMsg)
	{
	case WM_MOUSEMOVE: {

		pMousePos.x = GET_X_LPARAM(lParam);
		pMousePos.y = GET_Y_LPARAM(lParam);

		return;
	}
	case WM_MOUSEWHEEL: {

		iScrollState = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		return;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: {

		if (wParam < 256U) {

			uCurKey = static_cast<UINT>(wParam);
			bCurState = true;
		}

		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP: {

		if (wParam < 256U) {

			uCurKey = static_cast<UINT>(wParam);
			bCurState = false;
		}

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP: {

		uCurKey = VK_LBUTTON;
		bCurState = uMsg == WM_LBUTTONUP ? false : true;

		break;
	}
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP: {

		uCurKey = VK_RBUTTON;
		bCurState = uMsg == WM_RBUTTONUP ? false : true;

		break;
	}
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP: {

		uCurKey = VK_MBUTTON;
		bCurState = uMsg == WM_MBUTTONUP ? false : true;

		break;
	}
	default:
		break;
	}

	if (uCurKey != 0)
		aKeyState.at(uCurKey) = bCurState;
}

void C_InputManager::onWndProcEnd() {

	if (uCurKey != 0)
		aPrevKeyState.at(uCurKey) = aKeyState.at(uCurKey);

	iScrollState = 0;
}

bool C_InputManager::isKeyDown(UINT vkCode) {
	
	return aKeyState.at(vkCode) == true;
}

bool C_InputManager::isKeyPressed(UINT vkCode) {

	return aKeyState.at(vkCode) == true && aPrevKeyState.at(vkCode) == false;
}

bool C_InputManager::isKeyReleased(UINT vkCode) {

	return aKeyState.at(vkCode) == false && aPrevKeyState.at(vkCode) == true;
}