#ifndef @PROJNAMEUPPER@_H
#define @PROJNAMEUPPER@_H

#include <Wt/WApplication.h>

#include "ui_@PROJNAMELOWER@.h"

class @PROJNAME@ : public Wt::WApplication
{

public:
	@PROJNAME@(const Wt::WEnvironment& env);
	~@PROJNAME@();

	// declare slots here

private:
	Ui_@PROJNAME@ *ui;
	
};

#endif // @PROJNAMEUPPER@_H
