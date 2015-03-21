#ifndef PLUGINS_H
#define PLUGINS_H

class QString;

namespace Plugins
{
	void setUp();
  void tearDown();
	void runScript(QString &fileName);
};

#endif
