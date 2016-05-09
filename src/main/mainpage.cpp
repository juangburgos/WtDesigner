// Copyright (c) 2016 Juan Gonzalez Burgos
// 
// This file is part of WtDesigner.
// 
// WtDesigner is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// WtDesigner is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with WtDesigner.  If not, see <http://www.gnu.org/licenses/>.

#include "mainpage.h"
#include "mainwindow.h"
#include "helperfunctions.h"

#include <Wt/WBootstrapTheme>
#include <Wt/WEnvironment>

bool MainPage::qtConnected = false;

MainPage::MainPage(const Wt::WEnvironment& env, MainWindow *mainwindow) :
	Wt::WQApplication(env, true) // [IMPORTANT] : second param has to be set to TRUE
{
	m_mainwindow = mainwindow;
	m_qtroot     = NULL; 
	// necessary to dynamically update webpage
	enableUpdates(true);
	// don't know if necessary
	Wt::WString::setDefaultEncoding(Wt::UTF8);
}

MainPage::~MainPage()
{

}

void MainPage::create()
{
	QEventLoop evtblocker;
	m_qtroot = new SignalEmiter(NULL, this);
	// from mainwindow to mainpage
	QObject::connect(m_mainwindow                 , SIGNAL(SendCurrentConfig(QString))                                      , m_qtroot, SLOT(on_ReceivedCurrentConfig(QString))                                   , Qt::QueuedConnection);
	QObject::connect(m_mainwindow                 , SIGNAL(ElementEliminated(QString))                                      , m_qtroot, SLOT(on_ElementEliminated(QString))                                       , Qt::QueuedConnection);
	QObject::connect(m_mainwindow                 , SIGNAL(PropertyChanged(QString, QString, QString))                      , m_qtroot, SLOT(on_PropertyChanged(QString, QString, QString))                       , Qt::QueuedConnection);
	QObject::connect(m_mainwindow                 , SIGNAL(ConnectionChanged(QString, QString, QString, QString, QString))  , m_qtroot, SLOT(on_ConnectionChanged(QString, QString, QString, QString, QString))   , Qt::QueuedConnection);
	QObject::connect(&m_mainwindow->m_sigslotmodel, SIGNAL(DisconnectOldSignal(QString, QString, QString, QString, QString)), m_qtroot, SLOT(on_DisconnectOldSignal(QString, QString, QString, QString, QString)) , Qt::QueuedConnection);
	QObject::connect(m_mainwindow                 , SIGNAL(InsertConfig(QByteArray, QString, int))                          , m_qtroot, SLOT(on_InsertConfig(QByteArray, QString, int))                           , Qt::QueuedConnection);
	QObject::connect(m_mainwindow                 , SIGNAL(UpdateAllProperties())                                           , m_qtroot, SLOT(on_UpdateAllProperties())                                            , Qt::QueuedConnection);

	// from mainpage to mainwindow
	QObject::connect(m_qtroot, SIGNAL(RequestCurrentConfig())                    , m_mainwindow , SLOT(on_RequestCurrentConfig())                    , Qt::QueuedConnection);
	// special connection for requesting config and waiting for it
	QObject::connect(m_qtroot, SIGNAL(LoadCurrentConfig())                       , &evtblocker  , SLOT(quit())                                       , Qt::QueuedConnection);
	//
	QObject::connect(m_qtroot, SIGNAL(NewClientConneted())                 , m_mainwindow , SLOT(on_NewClientConneted())                      , Qt::QueuedConnection);
	// logic for connecting unique thread/session with Qt gui mainwindow
	thisQtConnected = false;
	if (!MainPage::qtConnected)
	{
		// from mainwindow to mainpage
		QObject::connect(m_mainwindow, SIGNAL(RequestAutoGenCpp(QString))   , m_qtroot, SLOT(on_GenerateAutoGenCpp(QString))   , Qt::QueuedConnection);
		QObject::connect(m_mainwindow, SIGNAL(RequestAllProperties(QString)), m_qtroot, SLOT(on_GenerateAllProperties(QString)), Qt::QueuedConnection);
		// from mainpage to mainwindow
		QObject::connect(m_qtroot, SIGNAL(ReloadPage())                 , m_mainwindow , SLOT(on_ReloadPage())                      , Qt::QueuedConnection);
		QObject::connect(m_qtroot, SIGNAL(SendAutoGenCpp(QByteArray))   , m_mainwindow , SLOT(on_ReceivedAutoGenCpp(QByteArray))    , Qt::QueuedConnection);
		QObject::connect(m_qtroot, SIGNAL(SendAllProperties(QByteArray)), m_mainwindow , SLOT(on_ReceivedAllProperties(QByteArray)) , Qt::QueuedConnection);
		// mark as connected
		qtConnected     = true;
		thisQtConnected = true;
	}

	// request current config
	Q_EMIT m_qtroot->RequestCurrentConfig();
	// wait for it
	evtblocker.exec();
	QObject::disconnect(m_qtroot, SIGNAL(LoadCurrentConfig()), &evtblocker, SLOT(quit()));
	// load configuration in dom document
	QDomDocument doc;
	doc.setContent(m_qtroot->m_currentState);
	// validate root element tagname
	QDomElement  docElem = doc.documentElement();
	if(docElem.tagName().compare("WtCreator", Qt::CaseInsensitive) != 0)
	{
		QMessageBox::critical(0, QObject::tr("XML Error"), QObject::tr("Not a valid *.wui file"));
		return;
	}

	// set CSS Theme
	// (http://www.webtoolkit.eu/widgets/layout/themes)
	// (http://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1WBootstrapTheme.html)
	// By default, the theme will use CSS resources that are shipped together with the Wt distribution, but since the Twitter Bootstrap CSS API is a popular API for custom themes, you can easily replace the CSS with custom-built CSS (by reimplementing styleSheets()).
	m_qtroot->m_strWtTheme = docElem.attribute(g_strThemeAttr); // COPY theme
	if (m_qtroot->m_strWtTheme.compare(g_strThemeBootstrat3) == 0)
	{
		Wt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();
		p_wtTheme->setVersion(Wt::WBootstrapTheme::Version3);
		setTheme(p_wtTheme);
	}
	else if (m_qtroot->m_strWtTheme.compare(g_strThemeBootstrat2) == 0)
	{
		Wt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();
		p_wtTheme->setVersion(Wt::WBootstrapTheme::Version2);
		setTheme(p_wtTheme);
	}
	else if (m_qtroot->m_strWtTheme.compare(g_strThemePolished) == 0)
	{
		setCssTheme("polished");
	}
	else if (m_qtroot->m_strWtTheme.compare(g_strThemeNone) == 0)
	{
		setCssTheme("");
	}

	// set browser tab title
	m_qtroot->m_strWtTitle = docElem.attribute(g_strTitleAttr); // COPY title
	setTitle(m_qtroot->m_strWtTitle.toStdString());
	
	// load tree recursively (qparent==0 on purpose which forces tree to be loaded from scratch)
	LoadRecursiveTree(docElem, NULL, m_qtroot);	
	// perform connections
	for (int i = 0; i < m_qtroot->m_listConnections.count(); i++)
	{
		if(!PerformConnection(m_qtroot->m_listConnections.at(i)))
		{
			qDebug() << "[ERROR] Could not perform connection : ("
				     << m_qtroot->m_listConnections.at(i)->m_strsender    << ", "
					 << m_qtroot->m_listConnections.at(i)->m_strsignal    << ", "
					 << m_qtroot->m_listConnections.at(i)->m_strreceiver  << ", "
					 << m_qtroot->m_listConnections.at(i)->m_strslot      << ", "
					 << m_qtroot->m_listConnections.at(i)->m_strparameter << ")";
		}
	}

	// let know main application that a new client has connected
	Q_EMIT m_qtroot->NewClientConneted();
}

void MainPage::destroy()
{
	// mark wtapp disconnected from mainwindow if connected
	if (thisQtConnected)
	{
		// mark as not connected
		thisQtConnected = false;
		qtConnected = false;
	}
	// clear connections list
	qDeleteAll(m_qtroot->m_listConnections);
	m_qtroot->m_listConnections.clear();
	// delete qt object
	delete m_qtroot;

}
// TODO : FIX BY SEPARATING INTO TWO DIFFFERENT FUNCTIONS
void MainPage::LoadRecursiveTree(QDomElement element, Wt::WContainerWidget *wparent/*=0*/, QObject *qparent/*=0*/, int irow/*=-1*/)
{
	QDomElement elem;
	if (element.tagName().compare(g_strDocumTag, Qt::CaseInsensitive) == 0) // if root
	{
		// first find and load all stylesheets
		QDomNodeList listStlyeElems = element.elementsByTagName(g_strStylesheetTag);
		for (int i = 0; i < listStlyeElems.count(); i++)
		{
			elem = listStlyeElems.at(i).toElement();
            QString strTemp = elem.attribute(g_strStylePathAttr);
            AddStyleSheetFile(strTemp);
		}
		// first find and load all javascripts
		QDomNodeList listJavasElems = element.elementsByTagName(g_strJavascriptTag);
		for (int i = 0; i < listJavasElems.count(); i++)
		{
			elem = listJavasElems.at(i).toElement();
            QString strTemp = elem.attribute(g_strJavasPathAttr);
            AddJavaScriptFile(strTemp);
		}
		// find root and load widget tree configuration recursivelly
		QDomNode node = element.firstChildElement(g_strWRootTag);
		if (!node.isNull()) 
		{
			// recursivelly load configuration
			LoadRecursiveTree(node.toElement(), wparent, qparent);
		}
		else { qDebug() << "[ERROR] Could not find widget root in configuration MainPage::LoadRecursiveTree"; }
		return;
	}

	// create mixed class instance
	QObject *object;
	if (element.tagName().compare(g_strWRootTag, Qt::CaseInsensitive) == 0)
	{ // case parent is wroot
		object = qparent;
	}
	else
	{ // case is wobject
		object = CreateWtQtInstance(&element, irow, wparent, qparent);
	}

	// if not null (valid wt type)
	if (object)
	{
		// load properties to wtqtobjects
		const QMetaObject *metaobject = object->metaObject();
		QString            name;
		QMetaProperty      metaproperty;
		QString            value;
		QString            objvalue;
		for (int i = 1; i < metaobject->propertyCount(); i++)
		{

			metaproperty = metaobject->property(i);
			name = QString::fromLatin1(metaproperty.name());
			if (!name.contains("Wt_")) { continue; } // ignore non Wt_properties
			if (name.compare("Wt_className") == 0) { continue; } // ignore Wt_className 
			//if (name.compare("Wt_htmlTagName") == 0) { continue; } // [NOTE] : app crash bug due to this is handled now both in helperfunctions::MapAllProperties and in helperfunctions::HelpUpdateAllProperties
			value = QString::fromUtf8(element.attribute(name, "").toUtf8());
			if (!value.isEmpty()) 
			{ 
				object->setProperty(metaproperty.name(), value);
			} // if there is nothing to write set default config to wt tree in mianwindow
		}
	}

	//qDebug() << "[INFO] Added item " << object->property("Wt_id").toString()
	//	                             << " (" << object->property("Wt_className").toString()
	//	                             << "), and parent " << qparent->property("Wt_id").toString()
	//	                             << " (" << qparent->property("Wt_className").toString() << ")";

	// iterate through children (if any)
	QDomNode node = element.firstChild();
	while (!node.isNull()) 
	{
		if (node.toElement().tagName().compare(g_strValidNodeTag, Qt::CaseInsensitive) == 0 || element.tagName().compare(g_strWRootTag, Qt::CaseInsensitive) == 0)
		{
			Wt::WContainerWidget *wobject = GetWContainerParent(object);
			if (wobject != NULL)
			{
				LoadRecursiveTree(node.toElement(), wobject, object);
			}
			else 
			{  
				qDebug() << "[ERROR] : Could not add Widget Wt_id        = : " << node.toElement().attribute("Wt_id")        << "due to invalid parent.";
				qDebug() << "[ERROR] : Could not add Widget Wt_className = : " << node.toElement().attribute("Wt_className") << "due to invalid parent.";
			}
		}
		// case is wconnection
		else if (node.toElement().tagName().compare(g_strConnectionTag, Qt::CaseInsensitive) == 0)
		{
			// check if inherits from WInteractWidget so it can have signals
			WtQtInteractWidget  *winteract = dynamic_cast<WtQtInteractWidget*>(object);
			Wt::WInteractWidget *wobject   = dynamic_cast<Wt::WInteractWidget*>(winteract);
			if (wobject != NULL)
			{

				QString wparameter = node.toElement().attribute("Wt_Parameter");
				// fill up connection config
				Wt_ConnectionConfig *conconfig = new Wt_ConnectionConfig();
				conconfig->m_strsender         = element.attribute("Wt_id");
				conconfig->m_strsignal         = node.toElement().attribute("Wt_Signal");
				conconfig->m_strreceiver       = node.toElement().attribute("Wt_Receiver");
				conconfig->m_strslot           = node.toElement().attribute("Wt_Slot");
				conconfig->m_strparameter      = node.toElement().attribute("Wt_Parameter");
				// store in list for later connection
				m_qtroot->m_listConnections.append(conconfig);
			}
			else
			{
				qDebug() << "[ERROR] : Could not cast to Wt::WInteractWidget : " << node.toElement().tagName();
			}
		}
		else
		{
			if (!node.toElement().tagName().isEmpty()) { qDebug() << "[ERROR] : Node type is invalid : " << node.toElement().tagName(); }
		}
		node = node.nextSibling();
	}
	// needed for new Wt::WServer::post -based implementation of SignalEmiter::on_InsertConfig
	//triggerUpdate();
}

void MainPage::CreateConnectionKey(QString &strConnectionId, Wt_ConnectionConfig * conConfig)
{
	// put together all fields
	strConnectionId = conConfig->m_strsender   +
		              conConfig->m_strreceiver +
		              conConfig->m_strsignal   +
		              conConfig->m_strslot     +
		              conConfig->m_strparameter;
	// clean spaces
	strConnectionId = strConnectionId.simplified();
	strConnectionId.replace(" ", "");
}

bool MainPage::PerformConnection(Wt_ConnectionConfig *conConfig)
{
	// validate sender
	WtQtInteractWidget  *wqsender = dynamic_cast<WtQtInteractWidget*>(FindElementById(m_qtroot, conConfig->m_strsender));
	Wt::WInteractWidget *wsender  = dynamic_cast<Wt::WInteractWidget*>(wqsender);
	if (wsender == NULL)
	{
		return false;
	}

	// validate receiver
	WtQtWidget  *wqreceiver = dynamic_cast<WtQtWidget*>(FindElementById(m_qtroot, conConfig->m_strreceiver));
	Wt::WWidget *wreceiver  = dynamic_cast<Wt::WWidget*>(wqreceiver);
	if (wreceiver == NULL)
	{
		return false;
	}

	// validate signal
	WtQtInteractWidget::Wt_Signals_ wsignal = wqsender->GetSignalEnumByName(conConfig->m_strsignal);
	if (wsignal == WtQtInteractWidget::Wt_Signals_invalid)
	{
		return false;
	}

	// validate slot
	WtQtWidget::Wt_Slots_ wslot = wqreceiver->GetSlotEnumByName(conConfig->m_strslot);
	if (wslot == WtQtWidget::Wt_Slots_invalid)
	{
		return false;
	}

	// validate number of parameters
	QStringList strListParams = conConfig->m_strparameter.split(",");
	int intValidSize = 0;
	switch (wslot)
	{
	case WtQtWidget::Wt_Slots_setStyleClass    :
	case WtQtWidget::Wt_Slots_addStyleClass    :
	case WtQtWidget::Wt_Slots_removeStyleClass :
	case WtQtWidget::Wt_Slots_toggleStyleClass :
		intValidSize = 1;
		break;
	default:
		break;
	}
	if (strListParams.size() != intValidSize || conConfig->m_strparameter.isEmpty())
	{
		return false;
	}

	// generate connection id (unique only for this instance of WtQtInteractWidget)
	QString strConnectionId ;
	CreateConnectionKey(strConnectionId, conConfig);

	// check if already exists
	if(wqsender->m_hashconnections.contains(strConnectionId))
	{
		qDebug() << "[WARN] : Signal already exists.";
		return false;
	}

	Wt::Signals::connection myconnection = wqsender->Wt_Connect_Step1(wsender, wsignal, wreceiver, wslot, strListParams);

	wqsender->m_hashconnections.insert(strConnectionId, myconnection);

	return true;

}


bool MainPage::PerformDisconnection(Wt_ConnectionConfig *conConfig)
{
	// generate connection id (unique only for this instance of WtQtInteractWidget)
	QString strConnectionId;
	CreateConnectionKey(strConnectionId, conConfig);

	// check if not exists
	WtQtInteractWidget  *wqsender = dynamic_cast<WtQtInteractWidget*>(FindElementById(m_qtroot, conConfig->m_strsender));
	if (!wqsender->m_hashconnections.contains(strConnectionId))
	{
		qDebug() << "[ERROR] : PerformDisconnection, m_hashconnections does not contain " << strConnectionId;
		return false;
	}
	// take connection form hash (remove form hash)
	Wt::Signals::connection myconnection = wqsender->m_hashconnections.take(strConnectionId);
	// disconnect
	myconnection.disconnect();

	return true;
}

QObject * MainPage::CreateWtQtInstance(QDomElement * element, int irow, Wt::WContainerWidget * wparent, QObject * qparent)
{
	QObject *object = NULL;

	if (!wparent)
	{
		wparent = GetWContainerParent(qparent);
	}

	if (element->attribute("Wt_className").compare("WContainerWidget") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtContainerWidget(NULL, qparent);
			Wt::WContainerWidget *wwobject = dynamic_cast<Wt::WContainerWidget*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtContainerWidget(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WAnchor") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtAnchor(NULL, qparent);
			Wt::WAnchor *wwobject = dynamic_cast<Wt::WAnchor*>(object); // [NOTE] changed casting to wanchor otherwise crash (this is because WtQtAnchor already inerits from a mixed-class)
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtAnchor(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WText") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtText(NULL, qparent);
			Wt::WText *wwobject = dynamic_cast<Wt::WText*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtText(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WLineEdit") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtLineEdit(NULL, qparent);
			Wt::WLineEdit *wwobject = dynamic_cast<Wt::WLineEdit*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtLineEdit(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WImage") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtImage(NULL, qparent);
			Wt::WImage *wwobject = dynamic_cast<Wt::WImage*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtImage(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WPushButton") == 0) // [TODO] first part was commented?
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtPushButton(NULL, qparent);
			Wt::WPushButton *wwobject = dynamic_cast<Wt::WPushButton*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtPushButton(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTemplate") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTemplate(NULL, qparent);
			Wt::WTemplate *wwobject = dynamic_cast<Wt::WTemplate*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTemplate(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WSplitButton") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtSplitButton(NULL, qparent);
			Wt::WSplitButton *wwobject = dynamic_cast<Wt::WSplitButton*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtSplitButton(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WRadioButton") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtRadioButton(NULL, qparent);
			Wt::WRadioButton *wwobject = dynamic_cast<Wt::WRadioButton*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtRadioButton(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WCheckBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtCheckBox(NULL, qparent);
			Wt::WCheckBox *wwobject = dynamic_cast<Wt::WCheckBox*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtCheckBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WComboBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtComboBox(NULL, qparent);
			Wt::WComboBox *wwobject = dynamic_cast<Wt::WComboBox*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtComboBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WInPlaceEdit") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtInPlaceEdit(NULL, qparent);
			Wt::WInPlaceEdit *wwobject = dynamic_cast<Wt::WInPlaceEdit*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtInPlaceEdit(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTextArea") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTextArea(NULL, qparent);
			Wt::WTextArea *wwobject = dynamic_cast<Wt::WTextArea*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTextArea(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WSelectionBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtSelectionBox(NULL, qparent);
			Wt::WSelectionBox *wwobject = dynamic_cast<Wt::WSelectionBox*>(object); 
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtSelectionBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WSpinBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtSpinBox(NULL, qparent);
			Wt::WSpinBox *wwobject = dynamic_cast<Wt::WSpinBox*>(object); 
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtSpinBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WDoubleSpinBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtDoubleSpinBox(NULL, qparent);
			Wt::WDoubleSpinBox *wwobject = dynamic_cast<Wt::WDoubleSpinBox*>(object); 
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtDoubleSpinBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTimeEdit") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTimeEdit(NULL, qparent);
			Wt::WTimeEdit *wwobject = dynamic_cast<Wt::WTimeEdit*>(object); 
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTimeEdit(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WDateEdit") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtDateEdit(NULL, qparent);
			Wt::WDateEdit *wwobject = dynamic_cast<Wt::WDateEdit*>(object); 
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtDateEdit(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WCalendar") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtWCalendar(NULL, qparent);
			Wt::WCalendar *wwobject = dynamic_cast<Wt::WCalendar*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtWCalendar(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WSlider") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtSlider(NULL, qparent);
			Wt::WSlider *wwobject = dynamic_cast<Wt::WSlider*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtSlider(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WFileUpload") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtFileUpload(NULL, qparent);
			Wt::WFileUpload *wwobject = dynamic_cast<Wt::WFileUpload*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtFileUpload(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WProgressBar") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtProgressBar(NULL, qparent);
			Wt::WProgressBar *wwobject = dynamic_cast<Wt::WProgressBar*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtProgressBar(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WGroupBox") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtGroupBox(NULL, qparent);
			Wt::WGroupBox *wwobject = dynamic_cast<Wt::WGroupBox*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtGroupBox(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WPanel") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtPanel(NULL, qparent);
			Wt::WPanel *wwobject = dynamic_cast<Wt::WPanel*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtPanel(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WStackedWidget") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtStackedWidget(NULL, qparent);
			Wt::WStackedWidget *wwobject = dynamic_cast<Wt::WStackedWidget*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtStackedWidget(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTabWidget") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTabWidget(NULL, qparent);
			Wt::WTabWidget *wwobject = dynamic_cast<Wt::WTabWidget*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTabWidget(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTabItem") == 0)
	{
		WtQtTabWidget *newparent = dynamic_cast<WtQtTabWidget*>(qparent);
		if (newparent)
		{
			object = new WtQtTabItem(newparent);
		}
		else
		{
			qDebug() << "[ERROR] : Could not cast WtQtTabItem parent as WtQtWTabWidget.";
			qDebug() << "[ERROR] : WtQtTabItem parent id    = " << qparent->property("Wt_id").toString();
			qDebug() << "[ERROR] : WtQtTabItem parend class = " << qparent->property("Wt_className").toString();
		}
	}
	else if (element->attribute("Wt_className").compare("WMenu") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtMenu(NULL, qparent);
			Wt::WMenu *wwobject = dynamic_cast<Wt::WMenu*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtMenu(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WMenuItem") == 0)
	{
		WtQtMenu *newparent = dynamic_cast<WtQtMenu*>(qparent);
		if (newparent)
		{
			object = new WtQtMenuItem(newparent);
		}
		else
		{
			qDebug() << "[ERROR] : Could not cast WtQtMenuItem parent as WtQtMenu.";
			qDebug() << "[ERROR] : WtQtMenuItem parent id    = " << qparent->property("Wt_id").toString();
			qDebug() << "[ERROR] : WtQtMenuItem parend class = " << qparent->property("Wt_className").toString();
		}
	}
	else if (element->attribute("Wt_className").compare("WPopupMenu") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtPopupMenu(NULL, qparent);
			Wt::WWidget *wwobject = dynamic_cast<WtQtPopupMenu*>(object)->getInternalWWidget();
			if (wwobject)
			{
				wparent->insertWidget(irow, wwobject);
			}
		}
		else
			object = new WtQtPopupMenu(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WPopupItem") == 0)
	{
		WtQtPopupMenu *newparent = dynamic_cast<WtQtPopupMenu*>(qparent);
		if (newparent)
		{
			object = new WtQtPopupItem(newparent);
		}
		else
		{
			qDebug() << "[ERROR] : Could not cast WtQtPopupItem parent as WtQtPopupMenu.";
			qDebug() << "[ERROR] : WtQtPopupItem parent id    = " << qparent->property("Wt_id").toString();
			qDebug() << "[ERROR] : WtQtPopupItem parend class = " << qparent->property("Wt_className").toString();
		}
	}
	else if (element->attribute("Wt_className").compare("WTable") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTable(NULL, qparent);
			Wt::WTable *wwobject = dynamic_cast<Wt::WTable*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTable(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTree") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTree(NULL, qparent);
			Wt::WTree *wwobject = dynamic_cast<Wt::WTree*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTree(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WTreeTable") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtTreeTable(NULL, qparent);
			Wt::WTreeTable *wwobject = dynamic_cast<Wt::WTreeTable*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtTreeTable(wparent, qparent);
	}
	else if (element->attribute("Wt_className").compare("WNavigationBar") == 0)
	{
		if (irow >= 0 && irow < wparent->children().size())
		{
			object = new WtQtNavigationBar(NULL, qparent);
			Wt::WNavigationBar *wwobject = dynamic_cast<Wt::WNavigationBar*>(object);
			wparent->insertWidget(irow, wwobject);
		}
		else
			object = new WtQtNavigationBar(wparent, qparent);
	}
	else
	{
		qDebug() << "[ERROR] : Unknown Wt Element."; // TODO : do something, now it crashes
		qDebug() << "[ERROR] : Unknown element className  = " << element->attribute("Wt_className");
		qDebug() << "[ERROR] : Unknown element lineNumber = " << element->lineNumber();
	}

	return object;
}

void MainPage::AddStyleSheetFile(QString &strPath)
{
	// check if URL
	if (strPath.contains("http") || strPath.contains("https") || strPath.contains("ftp"))
	{
		// can use this stylesheet
		Wt::WLink wlink = Wt::WLink(strPath.toStdString());
		this->useStyleSheet(wlink); // CSS <----------------------------
		m_qtroot->m_strCssFilesList.append(strPath);
	}
	// check if FILE
	else if (!strPath.isEmpty())
	{
		// create css files with unique file name to trick browsers into not using their cached files
		QTemporaryFile tmpFile;
		QFile          cssFile(strPath);
		tmpFile.setAutoRemove(false);
		tmpFile.setFileTemplate(g_strLocalTempPath+".wtdtmpXXXXXX.css");
		if (!tmpFile.open() || !cssFile.open(QIODevice::ReadOnly)) 
		{
			qDebug() << "[ERROR] Could not open the temporary file";
		}
		tmpFile.write(cssFile.readAll());
		tmpFile.flush();
		tmpFile.close();
		cssFile.close();
		// can use this stylesheet
		Wt::WLink wlink(QDir(".").relativeFilePath(tmpFile.fileName()).toStdString());
		this->useStyleSheet(wlink); // CSS <----------------------------
		m_qtroot->m_strCssFilesList.append(strPath);
		
	}
	// some browsers need a good refresh
	refresh();
}

void MainPage::AddJavaScriptFile(QString &strPath)
{
	// check if URL
	if (strPath.contains("http") || strPath.contains("https") || strPath.contains("ftp"))
	{
		// can use this stylesheet
		std::string wlink = strPath.toStdString();
		this->require(wlink); // JS <----------------------------
		m_qtroot->m_strJsFilesList.append(strPath);
	}
	// check if FILE
	else if (!strPath.isEmpty())
	{
		// create css files with unique file name to trick browsers into not using their cached files
		QTemporaryFile tmpFile;
		QFile          jsFile(strPath);
		tmpFile.setAutoRemove(false);
		tmpFile.setFileTemplate(g_strLocalTempPath + ".wtdtmpXXXXXX.js");
		if (!tmpFile.open() || !jsFile.open(QIODevice::ReadOnly))
		{
			qDebug() << "[ERROR] Could not open the temporary file";
		}
		tmpFile.write(jsFile.readAll());
		tmpFile.flush();
		tmpFile.close();
		jsFile.close();
		// can use this stylesheet
		std::string wlink = QDir(".").relativeFilePath(tmpFile.fileName()).toStdString();
		this->require(wlink); // JS <----------------------------
		m_qtroot->m_strJsFilesList.append(strPath);
	}
	// some browsers need a good refresh
	refresh();
}

// Connect signals and slots
//ui->p1One->clicked().connect(this, &MainPage::on_buttonOne_clicked);
//void MainPage::on_buttonOne_clicked()
//{
//	qDebug() << "\\n[QDEBUG] " << QDir::currentPath() << "\\n";
//}
//

// --------------------------------------------------------------------------------------------------------------------------------

QObject* FindElementById(QObject *parent, QString &strid)
{
	QString objid = parent->property(g_strIdAttr.toStdString().c_str()).toString();
	if (objid.compare(strid, Qt::CaseInsensitive) == 0)
	{
		return parent;
	}
	else
	{
		for (int i = 0; i < parent->children().count(); i++)
		{
			QObject *childObj = FindElementById(parent->children().at(i), strid);
			if (childObj != NULL)
			{
				return childObj;
			}
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------

void SignalEmiter::on_ReceivedCurrentConfig(QString currentConfig)
{
	m_currentState    = currentConfig;
	Q_EMIT LoadCurrentConfig();
}

SignalEmiter::SignalEmiter(QObject *parent, MainPage * owner)
  : WtQtInteractWidget(parent)
{
	m_currentState = "";
	if (owner)
	{
		mp_owner = owner;
	}
	else
	{
		mp_owner = NULL;
	}
}

void SignalEmiter::on_PropertyChanged(QString strElemChanged, QString strPropChanged, QString strNewPropVal)
{
	if (!mp_owner) { return; }
	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock) 
	{
		// We now have exclusive access to the application: we can safely modify the widget tree for example.
		QObject* objChanged = FindElementById(this, strElemChanged);

		// [FIX] from Qt to Wt
		strNewPropVal = DecodeTextXML(strNewPropVal);

		objChanged->setProperty(strPropChanged.toStdString().c_str(), strNewPropVal);
		// Push the changes to the browser
		mp_owner->triggerUpdate();
	}
	else
	{
		qDebug() << "[ERROR] : on_PropertyChanged, Could not grab WApplication lock";
	}
}

void SignalEmiter::on_ElementEliminated(QString strElemId)
{
	if (!mp_owner) { return; }
	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock) 
	{
		// We now have exclusive access to the application: we can safely modify the widget tree for example.
		QObject* objChanged = FindElementById(this, strElemId);
		delete objChanged;
		// Push the changes to the browser
		mp_owner->triggerUpdate();
	}
	else
	{
		qDebug() << "[ERROR] : on_ElementEliminated, Could not grab WApplication lock";
	}
}

void SignalEmiter::on_ConnectionChanged(QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter)
{
	if (!mp_owner) { return; }

	Wt_ConnectionConfig *conConfig;
	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock)
	{
		conConfig = new Wt_ConnectionConfig();
		conConfig->m_strsender = strSender;
		conConfig->m_strsignal = strSignal;
		conConfig->m_strreceiver = strReceiver;
		conConfig->m_strslot = strSlot;
		conConfig->m_strparameter = strParameter;
		// We now have exclusive access to the application: we can safely modify the widget tree for example.
		if(!mp_owner->PerformConnection(conConfig))
		{
			qDebug() << "[ERROR] : on_ConnectionChanged, Could not perform connection";
		}
		else
		{
			// store in list
			m_listConnections.append(conConfig);
			// Push the changes to the browser
			mp_owner->triggerUpdate();
		}
		
	}
	else
	{
		qDebug() << "[ERROR] : on_ConnectionChanged, Could not grab WApplication lock";
	}
}

void SignalEmiter::on_DisconnectOldSignal(QString strSender, QString strSignal, QString strReceiver, QString strSlot, QString strParameter)
{
	if (!mp_owner) { return; }
	Wt_ConnectionConfig conConfig;
	conConfig.m_strsender = strSender;
	conConfig.m_strsignal = strSignal;
	conConfig.m_strreceiver = strReceiver;
	conConfig.m_strslot = strSlot;
	conConfig.m_strparameter = strParameter;

	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock)
	{
		// We now have exclusive access to the application: we can safely modify the widget tree for example.
		if (!mp_owner->PerformDisconnection(&conConfig))
		{
			qDebug() << "[ERROR] : on_DisconnectOldSignal, Could not perform disconnection";
		}
		else
		{
			QString strtodel;
			QString strtotest;
			mp_owner->CreateConnectionKey(strtodel, &conConfig);
			// remove from list
			for (int i = 0; i < m_listConnections.size(); i++)
			{
				mp_owner->CreateConnectionKey(strtotest, m_listConnections.at(i));
				if (strtotest.compare(strtodel) == 0)
				{
					Wt_ConnectionConfig *todelete = m_listConnections.takeAt(i);
					delete todelete;
					todelete = NULL;
					break;
				}
			}
			// Push the changes to the browser
			mp_owner->triggerUpdate();
		}
	}
	else
	{
		qDebug() << "[ERROR] : on_ConnectionChanged, Could not grab WApplication lock";
	}
}

void SignalEmiter::on_InsertConfig(QByteArray strConfigFraction, QString strParentName, int irow)
{
	// We now have exclusive access to the application: we can safely modify the widget tree for example.
	QObject              *qparent = NULL;
	Wt::WContainerWidget *wparent = NULL;
	if (!strParentName.isEmpty())
	{
		qparent = FindElementById(this, strParentName);
		wparent = GetWContainerParent(qparent);
	}
	if (wparent == NULL)
	{
		qDebug() << "[ERROR] : Could not add widget in SignalEmiter::on_InsertConfig to to invalid parent " << strParentName;
		return;
	}
	QDomDocument doc;
	doc.setContent(strConfigFraction);
	//
	if (wparent == getWtRoot() && doc.documentElement().childNodes().count() > 0)
	{
		qDebug() << "[ERROR] Wt crashes in this situation";
		return;
	}
	else
	{
		QApplication::processEvents();
		// Update the tree (http://redmine.emweb.be/boards/2/topics/5306)
		//Wt::WServer::instance()->post(mp_owner->sessionId(), boost::bind(&MainPage::LoadRecursiveTree, mp_owner, doc.documentElement(), wparent, qparent, irow));
		Wt::WApplication::UpdateLock lock(mp_owner);
		if (lock)
		{
			mp_owner->LoadRecursiveTree(doc.documentElement(), wparent, qparent, irow);
			mp_owner->triggerUpdate();
		}
	} // TODO : crashes here when enable to load Wt_htmlTagName property in MainPage::LoadRecursiveTree
	//Q_EMIT GenerateAllProperties(strParentName);
}


void SignalEmiter::on_GenerateAutoGenCpp(QString strProjName)
{
	Q_EMIT SendAutoGenCpp(GetAutoGenCpp(mp_owner->m_qtroot, strProjName));
}


void SignalEmiter::on_GenerateAllProperties(QString strParentId)
{
	if (!mp_owner) { return; }

	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock)
	{
		
		// return all tree properties if string is empty
		if (strParentId.isEmpty())
		{
			//Q_EMIT SendAllProperties(GetAllProperties(mp_owner->m_qtroot));
			Q_EMIT SendAllProperties(GetAllProperties(this));
			mp_owner->triggerUpdate(); // (in SendAllProperties setProperty is used for some widgets)
			return;
		}
		// try to get specific newly added object reference
		QObject  * parent = GetWtQtObjById(this, strParentId);
		// return all tree properties if specific object is new
		if (parent == NULL)
		{
			//Q_EMIT SendAllProperties(GetAllProperties(mp_owner->m_qtroot));
			Q_EMIT SendAllProperties(GetAllProperties(this));
		}
		else // return specific object properties as well as downstream objects
		{
			Q_EMIT SendAllProperties(GetAllProperties(parent));
		}
		// Push the changes to the browser
		mp_owner->triggerUpdate(); // (in SendAllProperties setProperty is used for some widgets)
	}
	else
	{
		qDebug() << "[ERROR] : on_PropertyChanged, Could not grab WApplication lock";
	}
}

void SignalEmiter::on_UpdateAllProperties()
{
	if (!mp_owner) { return; }
	// Grab the application lock. It is a scoped lock.
	Wt::WApplication::UpdateLock lock(mp_owner);
	if (lock)
	{
		// Update all properties
		HelpUpdateAllProperties(mp_owner->m_qtroot);
		// Push the changes to the browser
		mp_owner->triggerUpdate();
	}
	else
	{
		qDebug() << "[ERROR] : on_PropertyChanged, Could not grab WApplication lock";
	}
}

Wt::WContainerWidget * SignalEmiter::getWtRoot()
{
	return mp_owner->root();
}

QString SignalEmiter::Wt_className()
{
	return "WContainerWidget";
}

QString SignalEmiter::Wt_id()
{
	return QString::fromStdString(mp_owner->root()->id());
}

void SignalEmiter::Wt_setId(QString id)
{
	mp_owner->root()->setId(id.toStdString());
}

QString SignalEmiter::Wt_styleClass()
{
	return QString::fromStdString(mp_owner->root()->styleClass().toUTF8());
}

void SignalEmiter::Wt_setStyleClass(QString styleclass)
{
	mp_owner->root()->setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString SignalEmiter::Wt_isInline()
{
	return QString("%1").arg(mp_owner->root()->isInline()); 
}

void SignalEmiter::Wt_setInline(QString isinline)
{
	mp_owner->root()->setInline(isinline.toUInt());
}

QString SignalEmiter::Wt_htmlTagName()
{
	return QString::fromStdString(mp_owner->root()->htmlTagName());
}

void SignalEmiter::Wt_setHtmlTagName(QString tagname)
{
	mp_owner->root()->setHtmlTagName(tagname.toStdString());
}



QString SignalEmiter::Cpp_instantiate()
{
	return Wt_id() + " = (PageRoot);\n";
}
