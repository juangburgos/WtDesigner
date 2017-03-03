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

#include "helperfunctions.h"
#include "myglobals.h"
#include "mixedclasses.h"
#include "mainpage.h"

// ------------------------------------------------------ HELPER FUNCTIONS --------------------------------------------------------

QByteArray GetAutoGenCpp(SignalEmiter *rootobj, QString strProjName)
{
	QMetaProperty metaproperty;
	QByteArray    name;
	QByteArray    value;
	QByteArray    config = "";

	// print start conditional include
	config += "#ifndef UI_" + strProjName.toUpper() + "_H\n";
	config += "#define UI_" + strProjName.toUpper() + "_H\n\n";
	config += QString("// Autogenerated on: %1 \n").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:MM:ss")).toUtf8();
	// print all necessary Wt headers (Wt_className property)
	QStringList strListIncludes;
	ListWtIncludes(rootobj, strListIncludes);
	for (int i = 0; i < strListIncludes.count(); i++)
	{
		config += QString("#include <Wt/%1>\n").arg(strListIncludes.at(i));
	}
	// print promoted widgets includes
	ListUserIncludes(rootobj, config);

	// append theme (TODO : make user-definable)
	config += QString("#include <Wt/WBootstrapTheme>\n");


	// print ui class definition
	config += "\nclass Ui_" + strProjName + "\n{\n\npublic:\n\n";
	// print all objects delcarations (Cpp_declare property)
	DeclAllWidgets(rootobj, config);

	// print setupui signature
	config += "\n\tvoid setupUi(Wt::WContainerWidget *PageRoot)\n\t{\n\n";

	// print theme
	if (rootobj->m_strWtTheme.compare(g_strThemeBootstrat3) == 0)
	{
		//Wt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();
		//p_wtTheme->setVersion(Wt::WBootstrapTheme::Version3);
		//setTheme(p_wtTheme);
		config += "\t\tWt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();\n";
		config += "\t\tp_wtTheme->setVersion(Wt::WBootstrapTheme::Version3);\n";
		config += "\t\tWt::WApplication::instance()->setTheme(p_wtTheme);\n";
	}
	else if (rootobj->m_strWtTheme.compare(g_strThemeBootstrat2) == 0)
	{
		//Wt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();
		//p_wtTheme->setVersion(Wt::WBootstrapTheme::Version2);
		//setTheme(p_wtTheme);
		config += "\t\tWt::WBootstrapTheme * p_wtTheme = new Wt::WBootstrapTheme();\n";
		config += "\t\tp_wtTheme->setVersion(Wt::WBootstrapTheme::Version2);\n";
		config += "\t\tWt::WApplication::instance()->setTheme(p_wtTheme);\n";
	}
	else if (rootobj->m_strWtTheme.compare(g_strThemePolished) == 0)
	{
		//setCssTheme("polished");
		config += "\t\tWt::WApplication::instance()->setCssTheme(\"polished\");\n";
	}
	else if (rootobj->m_strWtTheme.compare(g_strThemeNone) == 0)
	{
		//setCssTheme("");
		config += "\t\tWt::WApplication::instance()->setCssTheme(\"\");\n";
	}

	// print title
	config += "\t\tWt::WApplication::instance()->setTitle(\"" + rootobj->m_strWtTitle + "\");\n";

	//config += "\t\tPageRoot->setId(\"wt_root\");\n";
	// call stylesheets function
	config += "\n\t\taddAllStyleSheets();\n\n";
	// configure all widgets recursively
	ConfigAllWidgets(rootobj, config, "\t");
	// call connections function
	config += "\n\t\tconnectAllSignals();\n";
	// call stylesheets function
	config += "\n\t\taddAllJavaScripts();\n";
	// close setupUi
	config += "\n\t}\n\n";

	// print addAllStyleSheets signature
	config += "\n\tvoid addAllStyleSheets()\n\t{\n";
	// print all stylesheets
	PrintAllStyleSheets(rootobj, config);
	// close addAllStyleSheets
	config += "\n\t}\n\n";

	// print addAllJavaScripts signature
	config += "\n\tvoid addAllJavaScripts()\n\t{\n";
	// print all javascripts
	PrintAllJavaScripts(rootobj, config);
	// close addAllJavaScripts
	config += "\n\t}\n\n";

	// print connectAllSignals signature
	config += "\n\tvoid connectAllSignals()\n\t{\n";
	// print all stylesheets
	PrintAllConnections(rootobj, config);
	// close connectAllSignals
	config += "\n\t}\n\n";

	// print end class and conditional include
	config += "\n};\n\n#endif // UI_" + strProjName.toUpper() + "_H";

	return config;
}


void ListWtIncludes(QObject *object, QStringList &strListIncludes)
{
	QString strClassName = object->property("Wt_className").toString();
	// append helpers
	if (!strListIncludes.contains("WFileResource") && !strClassName.isEmpty()) { strListIncludes.append("WFileResource"); }
	// append to list if not there already
	if (!strListIncludes.contains(strClassName)      && 
		!strClassName.isEmpty()                      &&
		strClassName.compare("WTabItem")        != 0 &&  // ignore dummy class
		strClassName.compare("WPopupItem")      != 0 &&  // ignore dummy class
		strClassName.compare("WPromotedWidget") != 0 )   // ignore dummy class
	{
		// append class
		strListIncludes.append(strClassName);
		// WMenu needs WStackedWidget
		if (strClassName.compare("WMenu") == 0 && !strListIncludes.contains("WStackedWidget"))
		{
			strListIncludes.append("WStackedWidget");
		}
		// WRadioButton needs WButtonGroup
		if (strClassName.compare("WRadioButton") == 0 && !strListIncludes.contains("WButtonGroup"))
		{
			strListIncludes.append("WButtonGroup");
		}
	}
	// loop all children
	QObjectList objList = object->children();
	for (int i = 0; i < objList.size(); i++)
	{
		ListWtIncludes(objList.at(i), strListIncludes);
	}
}


void ListUserIncludes(QObject *object, QByteArray &config)
{
	QString strClassName = object->property("Wt_className").toString();

	// Check if WtQtPromotedWidget, append include and early exit (no children)
	if (strClassName.compare("WPromotedWidget") == 0)
	{
		WtQtPromotedWidget * promotedWidget = qobject_cast<WtQtPromotedWidget*>(object);
		if (promotedWidget->Wt_isGlobalInclude().toUInt())
		{
			config += QString("#include <%1>\n").arg(promotedWidget->Wt_headerLocation());
		}
		else
		{
			config += QString("#include \"%1\"\n").arg(promotedWidget->Wt_headerLocation());
		}
	}
	else
	{
		// loop all children
		QObjectList objList = object->children();
		for (int i = 0; i < objList.size(); i++)
		{
			ListUserIncludes(objList.at(i), config);
		}
	}

}


void DeclAllWidgets(QObject *object, QByteArray &config)
{
	// declare this child
	if (!object->property("Cpp_declare").toString().isEmpty())
	{
		config += "\t" + object->property("Cpp_declare").toString() + "\n";
	}
	// loop all children
	QObjectList objList = object->children();
	for (int i = 0; i < objList.size(); i++)
	{
		// loop children of child
		DeclAllWidgets(objList.at(i), config);
	}
}


void ConfigAllWidgets(QObject *object, QByteArray &config, QByteArray tabs /*= ""*/, QByteArray parentid /*= ""*/)
{
	qobject_cast<WtQtObject*>(object)->Cpp_internalTabs = tabs + "\t";
	if (!object->property("Cpp_instantiate").toString().isEmpty())
	{
		// instantiate each widget
		//config += "\t" + tabs + object->property("Cpp_instantiate").toString() + parentid + ");\n";
		config += "\t" + tabs + object->property("Cpp_instantiate").toString();
		// configure each property
		const QMetaObject *metaobject = object->metaObject();
		QMetaProperty metaproperty;
		QString       name;
		QByteArray    value;
		int count = metaobject->propertyCount();
		for (int i = 1; i < count; i++)
		{
			metaproperty = metaobject->property(i);
			name = QString::fromLatin1(metaproperty.name()).toUtf8();
			if (name.contains("Cpp_") && name.compare("Cpp_instantiate") != 0 && name.compare("Cpp_declare") != 0 )
			{
				//
				qobject_cast<WtQtObject*>(object)->Cpp_internalTabs += "\t";
				//
				value = object->property(name.toStdString().c_str()).toString().toUtf8();
				// [FIX] .toLatin1() for .toUtf8(); IN ALL PROJECT
				if (!value.isEmpty())
				{
					config += "\t" + tabs + value + "\n";
				}
			}
		}
	}

	// config all children (enclose in code block {...} for readibility)
	QObjectList objList = object->children();
	QByteArray parid = object->property("Wt_id").toString().toUtf8();
	if (parid.isEmpty()) { parid = parentid; }
	if (objList.size() > 0)
	{
		if(parid != parentid) { config += tabs + "\t" + "{\n"; }
		for (int i = 0; i < objList.size(); i++)
		{
			ConfigAllWidgets(objList.at(i), config, tabs + "\t" , parid);
		}
		if(parid != parentid) { config += tabs + "\t"  + "}\n"; }
	}
}


void PrintAllStyleSheets(SignalEmiter *rootobj, QByteArray &config)
{
	// print all stylesheets in rootobject list (populated in MainPage::AddStyleSheetFile)
	config += "\n";
	for (int i = 0; i < rootobj->m_strCssFilesList.size(); i++)
	{
		config += "\t\tWt::WApplication::instance()->useStyleSheet(Wt::WLink(\"" + rootobj->m_strCssFilesList.at(i) + "\"));\n";
	}
}


void PrintAllJavaScripts(SignalEmiter *rootobj, QByteArray &config)
{
	// print all javascripts in rootobject list (populated in MainPage::AddJavaScriptFile)
	config += "\n";
	for (int i = 0; i < rootobj->m_strJsFilesList.size(); i++)
	{
		config += "\t\tWt::WApplication::instance()->require(std::string(\"" + rootobj->m_strJsFilesList.at(i) + "\"));\n";
	}
}

void PrintAllConnections(SignalEmiter *rootobj, QByteArray &config)
{
	// print connections function
	config += "\n";
	WtQtWidget::Wt_Slots_ wslot;
	for (int i = 0; i < rootobj->m_listConnections.count(); i++)
	{
		wslot = WtQtWidget::GetSlotEnumByName(rootobj->m_listConnections.at(i)->m_strslot);
		switch (wslot)
		{
		case WtQtWidget::Wt_Slots_setStyleClass   :
		case WtQtWidget::Wt_Slots_addStyleClass   :		
		case WtQtWidget::Wt_Slots_removeStyleClass:
			// return  wsender->wsignal().connect(std::bind([=]() { wreceiver->removeStyleClass("..."); }));
			config += "\t\t" + rootobj->m_listConnections.at(i)->m_strsender    + "->"
				             + rootobj->m_listConnections.at(i)->m_strsignal    + "().connect(std::bind([=]() { "
				             + rootobj->m_listConnections.at(i)->m_strreceiver  + "->"
				             + rootobj->m_listConnections.at(i)->m_strslot      + " (\""
				             + rootobj->m_listConnections.at(i)->m_strparameter + "\"); }));\n";
		case WtQtWidget::Wt_Slots_toggleStyleClass:
			// return  wsender->wsignal().connect(std::bind([=]() { wreceiver->toggleStyleClass("...", !wreceiver->hasStyleClass("...")); }));
			config += "\t\t" + rootobj->m_listConnections.at(i)->m_strsender    + "->"
				             + rootobj->m_listConnections.at(i)->m_strsignal    + "().connect(std::bind([=]() { "
				             + rootobj->m_listConnections.at(i)->m_strreceiver  + "->"
				             + rootobj->m_listConnections.at(i)->m_strslot      + " (\""
				             + rootobj->m_listConnections.at(i)->m_strparameter + "\", !"
							 + rootobj->m_listConnections.at(i)->m_strreceiver  + "->hasStyleClass(\""
							 + rootobj->m_listConnections.at(i)->m_strparameter + "\")); }));\n";
		default:
			break;
		} 

	}
}

QMetaObject GetMetaObjectByClassName(QString strClassName)
{
	QMetaObject metaObj;
	if (strClassName.compare("WContainerWidget") == 0)
	{
		metaObj = WtQtContainerWidget::staticMetaObject;
	}
	else if (strClassName.compare("WAnchor") == 0)
	{
		metaObj = WtQtAnchor::staticMetaObject;
	}
	else if (strClassName.compare("WText") == 0)
	{
		metaObj = WtQtText::staticMetaObject;
	}
	else if (strClassName.compare("WLineEdit") == 0)
	{
		metaObj = WtQtLineEdit::staticMetaObject;
	}
	else if (strClassName.compare("WPushButton") == 0)
	{
		metaObj = WtQtPushButton::staticMetaObject;
	}
	else if (strClassName.compare("WImage") == 0)
	{
		metaObj = WtQtImage::staticMetaObject;
	}
	else if (strClassName.compare("WTemplate") == 0)
	{
		metaObj = WtQtTemplate::staticMetaObject;
	}
	else if (strClassName.compare("WSplitButton") == 0)
	{
		metaObj = WtQtSplitButton::staticMetaObject;
	}
	else if (strClassName.compare("WRadioButton") == 0)
	{
		metaObj = WtQtRadioButton::staticMetaObject;
	}
	else if (strClassName.compare("WCheckBox") == 0)
	{
		metaObj = WtQtCheckBox::staticMetaObject;
	}
	else if (strClassName.compare("WComboBox") == 0)
	{
		metaObj = WtQtComboBox::staticMetaObject;
	}
	else if (strClassName.compare("WInPlaceEdit") == 0)
	{
		metaObj = WtQtInPlaceEdit::staticMetaObject;
	}
	else if (strClassName.compare("WTextArea") == 0)
	{
		metaObj = WtQtTextArea::staticMetaObject;
	}
	else if (strClassName.compare("WSelectionBox") == 0)
	{
		metaObj = WtQtSelectionBox::staticMetaObject;
	}
	else if (strClassName.compare("WSpinBox") == 0)
	{
		metaObj = WtQtSpinBox::staticMetaObject;
	}
	else if (strClassName.compare("WDoubleSpinBox") == 0)
	{
		metaObj = WtQtDoubleSpinBox::staticMetaObject;
	}
	else if (strClassName.compare("WTimeEdit") == 0)
	{
		metaObj = WtQtTimeEdit::staticMetaObject;
	}
	else if (strClassName.compare("WDateEdit") == 0)
	{
		metaObj = WtQtDateEdit::staticMetaObject;
	}
	else if (strClassName.compare("WCalendar") == 0)
	{
		metaObj = WtQtCalendar::staticMetaObject;
	}
	else if (strClassName.compare("WSlider") == 0)
	{
		metaObj = WtQtSlider::staticMetaObject;
	}
	else if (strClassName.compare("WFileUpload") == 0)
	{
		metaObj = WtQtFileUpload::staticMetaObject;
	}
	else if (strClassName.compare("WProgressBar") == 0)
	{
		metaObj = WtQtProgressBar::staticMetaObject;
	}
	else if (strClassName.compare("WGroupBox") == 0)
	{
		metaObj = WtQtGroupBox::staticMetaObject;
	}
	else if (strClassName.compare("WPanel") == 0)
	{
		metaObj = WtQtPanel::staticMetaObject;
	}
	else if (strClassName.compare("WStackedWidget") == 0)
	{
		metaObj = WtQtStackedWidget::staticMetaObject;
	}
	else if (strClassName.compare("WTabWidget") == 0)
	{
		metaObj = WtQtTabWidget::staticMetaObject;
	}
	else if (strClassName.compare("WTabItem") == 0)
	{
		metaObj = WtQtTabItem::staticMetaObject;
	}
	else if (strClassName.compare("WMenu") == 0)
	{
		metaObj = WtQtMenu::staticMetaObject;
	}
	else if (strClassName.compare("WMenuItem") == 0)
	{
		metaObj = WtQtMenuItem::staticMetaObject;
	}
	else if (strClassName.compare("WPopupMenu") == 0)
	{
		metaObj = WtQtPopupMenu::staticMetaObject;
	}
	else if (strClassName.compare("WPopupItem") == 0)
	{
		metaObj = WtQtPopupItem::staticMetaObject;
	}
	else if (strClassName.compare("WTable") == 0)
	{
		metaObj = WtQtTable::staticMetaObject;
	}
	else if (strClassName.compare("WTree") == 0)
	{
		metaObj = WtQtTree::staticMetaObject;
	}
	else if (strClassName.compare("WTreeTable") == 0)
	{
		metaObj = WtQtTreeTable::staticMetaObject;
	}
	else if (strClassName.compare("WNavigationBar") == 0)
	{
		metaObj = WtQtNavigationBar::staticMetaObject;
	}
	else if (strClassName.compare("WPromotedWidget") == 0)
	{
		metaObj = WtQtPromotedWidget::staticMetaObject;
	}

	return metaObj;
}

void SetIconColor(QString svgres, QIcon &svgicon, QString svgcolor)
{
	// open svg resource load contents to qbytearray
	QFile file(svgres);
	file.open(QIODevice::ReadOnly);
	QByteArray baData = file.readAll();
	// load svg contents to xml document and edit contents
	QDomDocument doc;
	doc.setContent(baData);
	// recurivelly change color
    QDomElement elemTemp = doc.documentElement();
    SetAttrRecur(elemTemp, "path", "fill", svgcolor);
	// create svg renderer with edited contents
	QSvgRenderer svgRenderer(doc.toByteArray());
	// create pixmap target
	QPixmap pix(svgRenderer.defaultSize());
	pix.fill(Qt::transparent);
	// create painter to act over pixmap
	QPainter pixPainter(&pix);
	// use renderer to render over painter which paints on pixmap
	svgRenderer.render(&pixPainter);
	// load rendered pixmap on icon //QIcon myicon(pix);
	svgicon = QIcon(pix);
}

void SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
	// if it has the tagname then overwritte desired attribute
	if (elem.tagName().compare(strtagname) == 0)
	{
		elem.setAttribute(strattr, strattrval);
	}
	// loop all children
	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (!elem.childNodes().at(i).isElement())
		{
			continue;
		}
        QDomElement elemTemp = elem.childNodes().at(i).toElement();
        SetAttrRecur(elemTemp, strtagname, strattr, strattrval);
	}
}

bool CheckValidProjPath(const QString &strProjPath)
{
	// check if contains a *.wui file if not show error and cancel
	QDir m_Path(strProjPath);
	QDirIterator it(m_Path.absolutePath().toUtf8(), QStringList() << "*.wui", QDir::Files, QDirIterator::NoIteratorFlags);
	if (!it.hasNext())
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(QObject::tr("Error! The path %1 does not contain any project file *.wui").arg(strProjPath));
		msgBox.exec();
		return false;
	}
	// check if contains a ./resources/ folder if not show warning that styles will not be loaded
	if (!QDir(strProjPath+g_strResourcesPathConst).exists())
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(QObject::tr("Warning! The path %1 does not contain a 'resources' subdirectory, no resources will loaded.").arg(strProjPath));
		msgBox.exec();
	}
	return true;
}

QString GetProjFileName(const QString &strProjPath)
{
	QDir m_Path(strProjPath);
	QDirIterator it(m_Path.absolutePath().toUtf8(), QStringList() << "*.wui", QDir::Files, QDirIterator::NoIteratorFlags);
	it.next();
	return it.fileName();
}

bool CopyAllFilesInPath(QString strSourcePath, QString strTargetPath, QStringList strListTypes)
{
	bool b_retVal = true;
	// create target path if it doesnt exist, if it does, delete it
	if (!QDir(strTargetPath).exists())
	{
		if (!QDir().mkdir(strTargetPath))
		{
			b_retVal = false;
			qDebug() << "[ERROR] Could not create path " << strTargetPath << " in helperfunctions::CopyAllFilesInPath";
		}
	}
	QDir m_Path(strSourcePath);
	QDirIterator it(m_Path.absolutePath().toUtf8(), strListTypes, QDir::Files, QDirIterator::Subdirectories);
	// create local sub folders and copy files
	QString     strFilePath;
	QString     strTempPath;
	QStringList strPathParts;
	while (it.hasNext())
	{
		// get next file
		it.next();
		// get relative path wrt m_Path
		strFilePath = m_Path.relativeFilePath(it.filePath());
		// get all relative folders
		strPathParts = strFilePath.split("/");
		// test if is not in base folder
		if (strPathParts.size() > 1)
		{
			strTempPath.clear();
			for (int i = 0; i < strPathParts.size() - 1; i++)
			{
				strTempPath += strPathParts.at(i) + "/";
				// create local sub folder
				if (!QDir(strTargetPath + strTempPath).exists())
				{
					QDir().mkdir(strTargetPath + strTempPath);
				}
			}
		}
		// create local copy of files
		if (QFile::exists(strTargetPath + strFilePath))
		{
			QFile::remove(strTargetPath + strFilePath);
		}
		while (!QFile::copy(it.filePath(), strTargetPath + strFilePath))
		{
			b_retVal = false;
			qDebug() << QString("[ERROR] : Could not copy file : '" + it.filePath() + "' into '" + strTargetPath + strFilePath + "' in helperfunctions::CopyAllFilesInPath");
		}
	}
	// return success or not
	return b_retVal;
}

QByteArray GetAllProperties(QObject *rootobj)
{
	QDomDocument doc("AllProperties");
	QDomElement elem = doc.createElement("DocElement");
	doc.appendChild(elem);
	MapAllProperties(&doc, rootobj);
	return doc.toByteArray();
}

void MapAllProperties(QDomDocument *doc, QObject *parent) // from Wt to Qt
{
	if (parent)
	{
		// load properties to wtqtobjects
		const QMetaObject *metaobject = parent->metaObject();
		QString            name;
		QMetaProperty      metaproperty;
		QString            value;
		QString            id = parent->property("Wt_id").toString();
		if (id.isEmpty()) { return; }
		for (int i = 0; i < metaobject->propertyCount(); i++)
		{
			metaproperty = metaobject->property(i);
			name = QString::fromLatin1(metaproperty.name());
			if (!name.contains("Wt_")            ) { continue; } // ignore non Wt_properties
			if (name.compare("Wt_className") == 0) { continue; } // ignore Wt_className
			if (name.compare("Wt_id")        == 0) { continue; } // ignore Wt_id
			value = metaproperty.read(parent).toString();

			// [FIX] from Wt to Qt
			value = EncodeTextXML(value);

			// append new element for property
			QDomElement elem = doc->createElement("property");
			elem.setAttribute("id"   , id   );
			elem.setAttribute("name" , name );
			elem.setAttribute("value", value);
			doc->documentElement().appendChild(elem);
			// reset otherwise Wt for some reason unsets them
			if (name.compare("Wt_htmlTagName") == 0) { continue; } // ignore Wt_htmlTagName to avoid crash

			// [FIX] from Qt to Wt
			value = DecodeTextXML(value);

			parent->setProperty(name.toStdString().c_str(), value); 
		}
		// iterate through children
		for (int i = 0; i < parent->children().count(); i++)
		{
			MapAllProperties(doc, parent->children().at(i));
		}
	}
}


void HelpUpdateAllProperties(QObject *rootobj) // from Qt to Wt
{
	if (rootobj)
	{
		// load properties to wtqtobjects
		const QMetaObject *metaobject = rootobj->metaObject();
		QString            name;
		QMetaProperty      metaproperty;
		QString            value;
		QString            id = rootobj->property("Wt_id").toString();
		if (id.isEmpty()) { return; }
		for (int i = 0; i < metaobject->propertyCount(); i++)
		{
			metaproperty = metaobject->property(i);
			name = QString::fromLatin1(metaproperty.name());
			if (!name.contains("Wt_"         )     ) { continue; } // ignore non Wt_properties
			if (name.compare("Wt_className"  ) == 0) { continue; } // ignore Wt_className
			if (name.compare("Wt_id"         ) == 0) { continue; } // ignore Wt_id
			if (name.compare("Wt_htmlTagName") == 0) { continue; } // ignore Wt_htmlTagName to avoid crash
			value = metaproperty.read(rootobj).toString();

			// [FIX] from Qt to Wt
			value = DecodeTextXML(value);

			// reset otherwise Wt for some reason unsets them
			rootobj->setProperty(name.toStdString().c_str(), value);
		}
		// iterate through children
		for (int i = 0; i < rootobj->children().count(); i++)
		{
			HelpUpdateAllProperties(rootobj->children().at(i));
		}
	}
}


QObject * GetWtQtObjById(QObject *parent, QString strId)
{
	QObject * pobj;
	QString id = parent->property("Wt_id").toString();
	if (id.compare(strId, Qt::CaseInsensitive) == 0)
	{
		return parent;
	}
	else
	{
		for (int i = 0; i < parent->children().count(); i++)
		{
			pobj = GetWtQtObjById(parent->children().at(i), strId);
			if (pobj != NULL)
			{
				return pobj;
			}
		}
	}

	return NULL;
}


QObject * FindWtQtRoot(QObject *child)
{
	if (!child)
	{
		return NULL;
	}
	SignalEmiter *wtqtroot = dynamic_cast<SignalEmiter*>(child);
	if (wtqtroot)
	{
		return wtqtroot;
	}
	else
	{
		return FindWtQtRoot(child->parent());
	}
}

QMap<QString, QObject*> GetWtQtSiblings(QObject *sibling)
{
	QMap<QString, QObject*> map;
	QObject * parent = sibling->parent();
	QString strId;
	for (int i = 0; i < parent->children().count(); i++)
	{
		strId = parent->children().at(i)->property("Wt_id").toString();
		if (!strId.isEmpty() && parent->children().at(i) != sibling)
		{
			map.insert(strId, parent->children().at(i));
		}
	}
	return map;
}

Wt::WContainerWidget * GetWContainerParent(QObject *qparent)
{
	QString strParentType;
	Wt::WContainerWidget *wobject = NULL;
	if (qparent->property("Wt_className").toString().compare("WGroupBox") == 0)
	{
		wobject = dynamic_cast<Wt::WGroupBox*>(dynamic_cast<WtQtGroupBox*>(qparent));               strParentType = "WGroupBox";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WStackedWidget") == 0)		    
	{																							    
		wobject = dynamic_cast<Wt::WStackedWidget*>(dynamic_cast<WtQtStackedWidget*>(qparent));     strParentType = "WStackedWidget";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WPanel") == 0)				    
	{																							    
		wobject = dynamic_cast<WtQtPanel*>(qparent)->getCentralContainer();                         strParentType = "WPanel";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WTabWidget") == 0)			    
	{																							    
		wobject = dynamic_cast<WtQtTabWidget*>(qparent)->getWtParent();                             strParentType = "WTabWidget";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WMenu") == 0)				    
	{																							    
		wobject = dynamic_cast<WtQtMenu*>(qparent)->getWtParent();                                  strParentType = "WMenu";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WPopupMenu") == 0)			    
	{																							    
		wobject = dynamic_cast<WtQtPopupMenu*>(qparent)->getWtParent();                             strParentType = "WPopupMenu";
	}																							    
	else if (qparent->property("Wt_className").toString().compare("WNavigationBar") == 0)		    
	{																							    
		wobject = dynamic_cast<WtQtNavigationBar*>(qparent)->getWtParent();                         strParentType = "WNavigationBar";
	}
	else if (qparent->property("Wt_className").toString().compare("WAnchor") == 0)
	{
		wobject = dynamic_cast<Wt::WAnchor*>(dynamic_cast<WtQtAnchor*>(qparent));                   strParentType = "WAnchor";
	}
	else
	{
		wobject = dynamic_cast<Wt::WContainerWidget*>(dynamic_cast<WtQtContainerWidget*>(qparent)); strParentType = "WContainerWidget";
	}

	if (!wobject)
	{ // last try
		wobject = dynamic_cast<SignalEmiter*>(qparent)->getWtRoot(); strParentType = "WRoot";
	}

	//qDebug() << "[INFO] Container parent is of type " << strParentType;

	if (!wobject)
	{
		qDebug() << "[ERROR] : Could not cast to WContainerWidget at helperfunctions::GetWContainerParent";
		qDebug() << "[ERROR] : Could not cast to Wt::WContainerWidget Wt_id        = : " << qparent->property("Wt_id").toString();
		qDebug() << "[ERROR] : Could not cast to Wt::WContainerWidget Wt_className = : " << qparent->property("Wt_className").toString();
	}
	return wobject;
}

QString FindWtIncludeDir(QString strRefDir /*= ""*/, int level/* = 0*/)
{
	// check level
	if (level >= 2)
	{
		return "";
	}
	// fix
	if( !strRefDir.isEmpty() && !strRefDir.endsWith("/") && !strRefDir.endsWith("\\") ) { strRefDir += "/"; }
	// make a list of paths to search
	QStringList strListSearchPaths;
	strListSearchPaths.append(strRefDir);
	if (level == 0)
	{
#ifdef Q_OS_WIN
		strListSearchPaths.append(qgetenv("SYSTEMDRIVE") + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES")).absolutePath() + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES(x86)")).absolutePath() + "/");
#endif
//#ifdef Q_OS_UNIX 
//
//#endif
//#ifdef Q_OS_OSX 
//
//#endif 
	}
	strListSearchPaths.removeAll(""); // clean
	// look in subdirectories
	QString     strPossibleDir;
	for (int i = 0; i < strListSearchPaths.count(); i++)
	{
		QDir m_Path(strListSearchPaths.at(i));
		QStringList strListSubDirs = m_Path.entryList(QDir::AllDirs | QDir::NoDotAndDotDot); //
		for (int j = 0; j < strListSubDirs.count(); j++)
		{
			// get next file
			strPossibleDir = strListSubDirs.at(j);
			if (ValidateWtIncludeDir(strListSearchPaths.at(i) + strPossibleDir))
			{
				return strListSearchPaths.at(i) + strPossibleDir + "/";
			}
			else
			{
				strPossibleDir = FindWtIncludeDir(strListSearchPaths.at(i) + strPossibleDir, level+1);
				if (!strPossibleDir.isEmpty())
				{
					return strPossibleDir;
				}
			}
		}
		
	}
	return "";
}

QString FindWtLibraryDir(QString strRefDir /*= ""*/, int level/* = 0*/)
{
	// check level
	if (level >= 2)
	{
		return "";
	}
	// fix
	if (!strRefDir.isEmpty() && !strRefDir.endsWith("/") && !strRefDir.endsWith("\\")) { strRefDir += "/"; }
	// make a list of paths to search
	QStringList strListSearchPaths;
	strListSearchPaths.append(strRefDir);
	if (level == 0)
	{
#ifdef Q_OS_WIN
		strListSearchPaths.append(qgetenv("SYSTEMDRIVE") + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES")).absolutePath() + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES(x86)")).absolutePath() + "/");
#endif
//#ifdef Q_OS_UNIX
//
//#endif
//#ifdef Q_OS_OSX 
//
//#endif 
}
	strListSearchPaths.removeAll(""); // clean
	// look in subdirectories
	QString     strPossibleDir;
	for (int i = 0; i < strListSearchPaths.count(); i++)
	{
		QDir m_Path(strListSearchPaths.at(i));
		QStringList strListSubDirs = m_Path.entryList(QDir::AllDirs | QDir::NoDotAndDotDot); //
		for (int j = 0; j < strListSubDirs.count(); j++)
		{
			// get next file
			strPossibleDir = strListSubDirs.at(j);
			if (ValidateWtLibraryDir(strListSearchPaths.at(i) + strPossibleDir))
			{
				return strListSearchPaths.at(i) + strPossibleDir + "/";
			}
			else
			{
				strPossibleDir = FindWtLibraryDir(strListSearchPaths.at(i) + strPossibleDir, level + 1);
				if (!strPossibleDir.isEmpty())
				{
					return strPossibleDir;
				}
			}
		}

	}
	return "";
}

QString FindWtBinaryDir(QString strRefDir /*= ""*/, int level/* = 0*/)
{
	// check level
	if (level >= 2)
	{
		return "";
	}
	// fix
	if (!strRefDir.isEmpty() && !strRefDir.endsWith("/") && !strRefDir.endsWith("\\")) { strRefDir += "/"; }
	// make a list of paths to search
	QStringList strListSearchPaths;
	strListSearchPaths.append(strRefDir);
	if (level == 0)
	{
#ifdef Q_OS_WIN
		strListSearchPaths.append(qgetenv("SYSTEMDRIVE") + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES")).absolutePath() + "/");
		strListSearchPaths.append(QDir(qgetenv("PROGRAMFILES(x86)")).absolutePath() + "/");
#endif
//#ifdef Q_OS_UNIX 
//
//#endif
//#ifdef Q_OS_OSX 
//
//#endif 
}
	strListSearchPaths.removeAll(""); // clean
	// look in subdirectories
	QString     strPossibleDir;
	for (int i = 0; i < strListSearchPaths.count(); i++)
	{
		QDir m_Path(strListSearchPaths.at(i));
		QStringList strListSubDirs = m_Path.entryList(QDir::AllDirs | QDir::NoDotAndDotDot); //
		for (int j = 0; j < strListSubDirs.count(); j++)
		{
			// get next file
			strPossibleDir = strListSubDirs.at(j);
			if (ValidateWtBinaryDir(strListSearchPaths.at(i) + strPossibleDir))
			{
				return strListSearchPaths.at(i) + strPossibleDir + "/";
			}
			else
			{
				strPossibleDir = FindWtBinaryDir(strListSearchPaths.at(i) + strPossibleDir, level + 1);
				if (!strPossibleDir.isEmpty())
				{
					return strPossibleDir;
				}
			}
		}

	}
	return "";
}

bool ValidateWtIncludeDir(QString strIncludeDir)
{
	// 1) find ./Wt subdir
	QDir::setSearchPaths("IncludeDir", QStringList(strIncludeDir));
	QDir dirWtInclude("IncludeDir:/Wt");
	if (!dirWtInclude.exists())
	{
		return false;
	}
	// 2) look for some of the header files inside ./Wt subdir
	QStringList strListOfDesired(QStringList() << "WAnchor" 
		                                       << "WApplication"
											   << "WComboBox" 
											   << "WGroupBox" 
											   << "WImage" 
											   << "WLabel" 
											   << "WMenu" 
											   << "WPanel" 
											   << "WServer" 
											   << "WString");
	QStringList strListOfFiles = dirWtInclude.entryList();
	for (int i = 0; i < strListOfDesired.count(); i++)
	{
		if (!strListOfFiles.contains(strListOfDesired.at(i)))
		{
			return false;
		}
	}
	return true;
}

bool ValidateWtLibraryDir(QString strLibraryDir)
{
	// directly look for *.lib / .a of interest
	QStringList strListOfDesired(QStringList() << "wt"
		                                       << "wtd"
		                                       << "wthttp"
		                                       << "wthttpd");
	QStringList strListOfFiles = QDir(strLibraryDir).entryList();
	for (int i = 0; i < strListOfDesired.count(); i++)
	{
		if (!strListOfFiles.contains(strListOfDesired.at(i)+".lib") && 
			!strListOfFiles.contains(strListOfDesired.at(i)+".a")  )
		{
			return false;
		}
	}
	return true;
}

bool ValidateWtBinaryDir(QString strBinaryDir)
{
	// directly look for *.dll / .so of interest
	QStringList strListOfDesired(QStringList() << "wt"
		                                       << "wtd"
		                                       << "wthttp"
		                                       << "wthttpd");
	QStringList strListOfFiles = QDir(strBinaryDir).entryList();
	for (int i = 0; i < strListOfDesired.count(); i++)
	{
		if (!strListOfFiles.contains(strListOfDesired.at(i) + ".dll") &&
			!strListOfFiles.contains(strListOfDesired.at(i) + ".so"))
		{
			return false;
		}
	}
	return true;
}

QStringList FindWtLibFiles(QString strLibDir, QStringList listOfLibNames)
{
	QStringList listOfLibFiles;
	QStringList listOfMatched;
	QStringList strListOfFiles = QDir(strLibDir).entryList();
	// possiible extensions
	QString strExtensions = "lib|a";
	// find the library files
	QString strRegex;
	for (int i = 0; i < listOfLibNames.count(); i++)
	{
		//strRegex = (\w)*LIBNAME(.)*[^(EXT1|EXT2)\s]+(EXT1|EXT2)
		strRegex = QString("(\w)*%1(.)*[^(%2)\s]+(%2)").arg(listOfLibNames.at(i)).arg(strExtensions);
		listOfMatched = strListOfFiles.filter(QRegExp(strRegex));
		// found previous but not current = FAIL
		if (listOfMatched.isEmpty() && i > 0)
		{
			return QStringList();
		}
		// all good
		listOfLibFiles.append(listOfMatched);
	}
	return listOfLibFiles;
}

QString ProcessCMakeTemplate(QString strInputFile, QString strOutputFile, QList<QPair<QString, QString>> listStrPairs)
{
	// open input file and load contents
	QFile fileInput(strInputFile);
	QString strInputOutput;
	if (fileInput.open(QIODevice::ReadOnly))
	{
		// read data from file
		strInputOutput = fileInput.readAll();
	}
	else
	{
		return QObject::tr("Could not open file ") + strInputFile;
	}
	fileInput.close();

	// replace tokens by their values
	for (int i = 0; i < listStrPairs.count(); i++)
	{
		strInputOutput.replace(listStrPairs.at(i).first, listStrPairs.at(i).second);
	}
	// TODO : check first if exists

	// open output file and write modified contents
	QFile fileOutput(strOutputFile);
	if (fileOutput.open(QIODevice::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&fileOutput);
		stream << strInputOutput;
	}
	else
	{
		return QObject::tr("Could not create file ") + strOutputFile;
	}
	fileOutput.close();

	// return no error
	return QString();
}

QString EncodeTextXML(QString strInputRaw)
{
	// QUrl::toPercentEncoding   : Returns an encoded copy of input. input is first converted to UTF-8, and all ASCII-characters that are not in the unreserved group are percent encoded.
	// http://doc.qt.io/qt-5/qurl.html#toPercentEncoding
	// To prevent characters from being percent encoded pass them to second parameter 
	return QUrl::toPercentEncoding(strInputRaw, " #:/");
}

QString DecodeTextXML(QString strInputEncoded)
{
	// QUrl::fromPercentEncoding : Returns a decoded copy of input. input is first decoded from percent encoding, then converted from UTF-8 to unicode.
	// http://doc.qt.io/qt-5/qurl.html#fromPercentEncoding
	return QUrl::fromPercentEncoding(strInputEncoded.toUtf8());
}

