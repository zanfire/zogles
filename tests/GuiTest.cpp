#include "GuiTest.h"

#include "zGuiStack.h"
#include "zGuiText.h"
#include "zGuiRect.h"
#include "zGuiAbsLayout.h"

GuiTest::GuiTest(void) {
}


GuiTest::~GuiTest(void) {
}


zGuiObject* GuiTest::on_create(zWin* win) {
  /*
  zGuiStack* root = new zGuiStack(win);

  for (int i = 0; i < 2; i++) {
    zGuiObject* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o);
    o->release_reference();
  }
  
  for (int i = 0; i < 1; i++) {
    zGuiText* o = new zGuiText(win);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o);
    o->release_reference();
  }

  for (int i = 0; i < 2; i++) {
    zGuiObject* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o);
    o->release_reference();
  }

  root->set_orientation(zGuiStack::ORIENTATION_HORIZONTAL);
  */

  zGuiAbsLayout* root = new zGuiAbsLayout(win);

  {
    zGuiText* o = new zGuiText(win);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_LEFT_TOP);
    o->release_reference();
  }
  {
    zGuiText* o = new zGuiText(win);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_RIGHT_TOP);
    o->release_reference();
  }
  {
    zGuiText* o = new zGuiText(win);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_RIGHT_BOTTOM);
    o->release_reference();
  }
  {
    zGuiText* o = new zGuiText(win);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_LEFT_BOTTOM);
    o->release_reference();
  }

  {
    zGuiRect* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_LEFT_TOP);
    o->release_reference();
  }
  {
    zGuiRect* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_RIGHT_TOP);
    o->release_reference();
  }
  {
    zGuiRect* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_RIGHT_BOTTOM);
    o->release_reference();
  }
  {
    zGuiRect* o = new zGuiRect(win);
    o->set_width(100);
    o->set_height(100);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.0f, 0.0f, zGuiAbsLayout::EDGE_LEFT_BOTTOM);
    o->release_reference();
  }

  {
    zGuiText* o = new zGuiText(win);
    o->set_text("ON KORO KORO SENDARI MATOGI SOWAKA.");
    o->set_vertical_align(zGuiObject::VERTICAL_ALIGN_CENTER);
    o->set_horizontal_align(zGuiObject::HORIZONTAL_ALIGN_CENTER);
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    root->add(o, 0.5f, 0.5f, zGuiAbsLayout::EDGE_LEFT_TOP);
    o->release_reference();
  }

  return root;
}
