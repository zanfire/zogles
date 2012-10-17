#include "zOGLESRuntime.h"


zMutex* zOGLESRuntime::_singleton_mtx = new zMutex();
zOGLESRuntime* zOGLESRuntime::_singleton = NULL;


zOGLESRuntime::zOGLESRuntime(void)  {
}


zOGLESRuntime::~zOGLESRuntime(void) {
}


zOGLESRuntime* zOGLESRuntime::get_instance(void) {
  zScopeMutex scope(_singleton_mtx);
  if (_singleton == NULL) {
    _singleton = new zOGLESRuntime();
  }
  return _singleton;
}
  

void zOGLESRuntime::shutdown(void) {
  if (_singleton != NULL) delete _singleton;
  delete _singleton_mtx;
}  
