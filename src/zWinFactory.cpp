#include "zWinFactory.h"

#include "zWin_windows.h"

zMutex* zWinFactory::_singleton_mtx = new zMutex();
zWinFactory* zWinFactory::_singleton = NULL;


zWinFactory::zWinFactory(void) : _windows(NO, 32) {
}


zWinFactory::~zWinFactory(void) {
}


zWinFactory* zWinFactory::get_instance(void) {
  zScopeMutex scope(_singleton_mtx);
  if (_singleton == NULL) {
    _singleton = new zWinFactory();
  }
  return _singleton;
}
  

void zWinFactory::shutdown(void) {
  if (_singleton != NULL) delete _singleton;
  delete _singleton_mtx;
}
  


zWin* zWinFactory::create(zRect const& pos, zString const& name) {
  zScopeMutex scope(_singleton_mtx);
#if defined(_WIN32)
  zWin* win = new zWin_windows(this);
#endif
  _windows.append(win);
  win->start();
  // Should be nice wait for the creation completed.

  return win;
}


void zWinFactory::on_window_destroy(zWin* win) {
  zScopeMutex scope(_singleton_mtx);
  for (int i = 0; i < _windows.get_count(); i++) {
    zWin* w = NULL;
    if (_windows.get(i, &w)) {
      if (win == w) {
        _windows.remove(i, NULL);
        break;
      }
    }
  }
  _event_close.signal();
}


zWin* zWinFactory::get_by_id(ZWIN_ID id) {
  zScopeMutex scope(_singleton_mtx);
  for (int i = 0; i < _windows.get_count(); i++) {
    zWin* win = NULL;
    if (_windows.get(i, &win)) {
      if (win->get_id() == id) {
        return win;
      }
    }
  }
  return NULL;
}


bool zWinFactory::wait_all_windows_close(void) {
  while (_windows.get_count() > 0) {
    _event_close.wait();
  }
  return true;
}
