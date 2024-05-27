#pragma once
#include <QObject>
#include "core/module_interface.h"

namespace vrd {

 /**
* More scene selection modules are used to remind users that other scenes are still under development and waiting to be launched
* Interfaces in this class are not implemented
*/
class SceneSelectModule : public QObject, public IModule {
	Q_OBJECT

signals:
	void sigSceneSelectReturn();

public:
	static void addThis();

private:
	SceneSelectModule();

public:
	~SceneSelectModule();

public:
	void open() override;
	void close() override;

public:
	void quit(bool error = false);
};

}  // namespace vrd

