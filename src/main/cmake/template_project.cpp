#include "@PROJNAMELOWER@.h"

@PROJNAME@::@PROJNAME@(const Wt::WEnvironment& env) : Wt::WApplication(env), ui(new Ui_@PROJNAME@)
{
	ui->setupUi(root());

}

@PROJNAME@::~@PROJNAME@()
{
	delete ui;
}
