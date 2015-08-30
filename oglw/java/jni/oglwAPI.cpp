#include "oglwAPI.h"
#include "oglwApp.h"

namespace OGLW {

uptr<OGLWApp> app;

void init(float _windowWidth, float _windowHeight) {
	app = uptr<OGLWApp>(new OGLWApp(_windowWidth, _windowHeight));
}

}