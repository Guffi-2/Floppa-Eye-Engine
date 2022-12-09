#pragma once


bool CheckHresult(HRESULT hr, std::string errorText) {
	if (hr != S_OK) {
		MessageBox(NULL, errorText.c_str(), "FloppaEye Error", MB_ICONERROR);
		return false;
	}
	return true;
}