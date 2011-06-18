#include <QApplication>
#include "MetaWindow.hpp"

int main(int argc, char* argv[]){
  QApplication app(argc, argv);
  app.setApplicationName("Udj");
  app.setQuitOnLastWindowClosed(true);
  MetaWindow window;
  window.show();
  return app.exec();
}
