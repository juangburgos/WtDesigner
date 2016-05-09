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

#include <QMetaEnum>
#include <QDebug>

#include <functional> // [LINUX]

#include "mixedclasses.h"
#include "mainpage.h"
#include "helperfunctions.h"
#include "myglobals.h"

QString WtQtObject::Cpp_internalTabs = "";

Wt::Signals::connection WtQtInteractWidget::Wt_Connect_Step1(Wt::WInteractWidget *wsender, Wt_Signals_ wsignal, Wt::WWidget *wreceiver, Wt_Slots_ wslot, QStringList &wparameter)
{
	switch (wsignal)
	{
	case WtQtInteractWidget::Wt_Signals_clicked:
		return Wt_Connect_Step2(wsender->clicked(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_doubleClicked:
		return Wt_Connect_Step2(wsender->doubleClicked(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_mouseWentDown:
		return Wt_Connect_Step2(wsender->mouseWentDown(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_mouseWentUp:
		return Wt_Connect_Step2(wsender->mouseWentUp(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_mouseWentOut:
		return Wt_Connect_Step2(wsender->mouseWentOut(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_mouseWentOver:
		return Wt_Connect_Step2(wsender->mouseWentOver(), wreceiver, wslot, wparameter);
	case WtQtInteractWidget::Wt_Signals_mouseMoved:
		return Wt_Connect_Step2(wsender->mouseMoved(), wreceiver, wslot, wparameter);
	default:
		break;
	}
	return Wt::Signals::connection();
}


Wt::Signals::connection WtQtInteractWidget::Wt_Connect_Step2(Wt::EventSignal< Wt::WMouseEvent > &wsignalfun, Wt::WWidget *wreceiver, Wt_Slots_ wslot, QStringList &wparameter)
{
    //try
    //{
		switch (wslot)
		{
		case WtQtWidget::Wt_Slots_setStyleClass:
			return  wsignalfun.connect(std::bind([=]() { wreceiver->setStyleClass(Wt::WString::fromUTF8(wparameter.at(0).toStdString())); }));
		case WtQtWidget::Wt_Slots_addStyleClass:
			return  wsignalfun.connect(std::bind([=]() { wreceiver->addStyleClass(Wt::WString::fromUTF8(wparameter.at(0).toStdString())); }));
		case WtQtWidget::Wt_Slots_removeStyleClass:
			return  wsignalfun.connect(std::bind([=]() { wreceiver->removeStyleClass(Wt::WString::fromUTF8(wparameter.at(0).toStdString())); }));
		case WtQtWidget::Wt_Slots_toggleStyleClass:
			return  wsignalfun.connect(std::bind([=]() { wreceiver->toggleStyleClass(Wt::WString::fromUTF8(wparameter.at(0).toStdString()), !wreceiver->hasStyleClass(wparameter.at(0).toStdString())); }));
		default:
			break;
		}
    /*}
    catch (Wt::Dbo::Exception* e)
	{
		qDebug() << "[ERROR] : Exception occurred in WtQtInteractWidget::Wt_Connect_Step2";
	}
    */
	return Wt::Signals::connection();

		// http://www.webtoolkit.eu/wt/doc/reference/html/namespaceWt_1_1Signals.html

}


WtQtInteractWidget::Wt_Signals_ WtQtInteractWidget::GetSignalEnumByName(QString name)
{
	QMetaObject metaobj = WtQtInteractWidget::staticMetaObject;
	for (int i = metaobj.enumeratorOffset(); i < metaobj.enumeratorCount(); ++i)
	{
		QMetaEnum metaenum = metaobj.enumerator(i);
		QString strEnumName = QString::fromLatin1( metaenum.name()  );
		// check correct enum
		if (strEnumName.compare("Wt_Signals_", Qt::CaseInsensitive) != 0)
		{
			continue;
		}
		// find correct value
		for (int j = 0; j < metaenum.keyCount(); ++j)
		{
			// check if name matches key ([WARNING] without prepended string)
			if (QString::fromLatin1(metaenum.valueToKey(j)).remove(strEnumName).compare(name, Qt::CaseInsensitive) == 0)
			{
				return (WtQtInteractWidget::Wt_Signals_)j;
			}
		}
	}
	return Wt_Signals_invalid;
}

WtQtWidget::Wt_Slots_ WtQtWidget::GetSlotEnumByName(QString name)
{
	QMetaObject metaobj = WtQtWidget::staticMetaObject;
	for (int i = metaobj.enumeratorOffset(); i < metaobj.enumeratorCount(); ++i)
	{
		QMetaEnum metaenum = metaobj.enumerator(i);
		QString strEnumName = QString::fromLatin1(metaenum.name());
		// check correct enum
		if (strEnumName.compare("Wt_Slots_", Qt::CaseInsensitive) != 0)
		{
			continue;
		}
		// find correct value
		for (int j = 0; j < metaenum.keyCount(); ++j)
		{
			// check if name matches key ([WARNING] without prepended string)
			if (QString::fromLatin1(metaenum.valueToKey(j)).remove(strEnumName).compare(name, Qt::CaseInsensitive) == 0)
			{
				return (WtQtWidget::Wt_Slots_)j;
			}
		}
	}
	return Wt_Slots_invalid;
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtContainerWidget             *******************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtContainerWidget::WtQtContainerWidget(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WContainerWidget(wparent), WtQtInteractWidget(qparent)
{

}

WtQtContainerWidget::WtQtContainerWidget(const WtQtContainerWidget& other)
{
	Q_UNUSED(other)
}

WtQtContainerWidget::~WtQtContainerWidget()
{

}

QString WtQtContainerWidget::Wt_className()
{
	return "WContainerWidget";
}

QString WtQtContainerWidget::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtContainerWidget::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtContainerWidget::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtContainerWidget::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtContainerWidget::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtContainerWidget::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

QString WtQtContainerWidget::Wt_htmlTagName()
{
	return QString::fromStdString(htmlTagName());
}

void WtQtContainerWidget::Wt_setHtmlTagName(QString tagname)
{
	if (tagname.isEmpty()) { return; }
	setHtmlTagName(tagname.toStdString());
}


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************************             WtQtAnchor             **********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtAnchor::WtQtAnchor(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WAnchor(wparent), WtQtContainerWidget(wparent, qparent)
{

}

WtQtAnchor::WtQtAnchor(const WtQtAnchor& other)
{
	Q_UNUSED(other)
}

WtQtAnchor::~WtQtAnchor()
{

}

QString WtQtAnchor::Wt_className()
{
	return "WAnchor";
}

QString WtQtAnchor::Wt_id()
{
	return QString::fromStdString(Wt::WAnchor::id());
}

void WtQtAnchor::Wt_setId(QString id)
{
	Wt::WAnchor::setId(id.toStdString());
}

QString WtQtAnchor::Wt_styleClass()
{
	return QString::fromStdString(Wt::WAnchor::styleClass().toUTF8());
}

void WtQtAnchor::Wt_setStyleClass(QString styleclass)
{
	Wt::WAnchor::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtAnchor::Wt_isInline()
{
	return QString("%1").arg(Wt::WAnchor::isInline());
}

void WtQtAnchor::Wt_setInline(QString isinline)
{
	Wt::WAnchor::setInline(isinline.toUInt());
}

//QString WtQtAnchor::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WAnchor::htmlTagName());
//}
//
//void WtQtAnchor::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WAnchor::setHtmlTagName(tagname.toStdString());
//}

QString WtQtAnchor::Wt_link()
{
	return QString::fromStdString(link().url());
}

void WtQtAnchor::Wt_setLink(QString link)
{
	setLink(Wt::WLink(link.toStdString()));
}

QString WtQtAnchor::Wt_target()
{
	if (target() == Wt::TargetNewWindow) { return "TargetNewWindow"; }
	else if (target() == Wt::TargetThisWindow) { return "TargetThisWindow"; }
	else                                       { return "TargetSelf"; }
}

void WtQtAnchor::Wt_setTarget(QString target)
{
	if (target.compare("TargetNewWindow", Qt::CaseInsensitive) == 0) { setTarget(Wt::TargetNewWindow); }
	else if (target.compare("TargetThisWindow", Qt::CaseInsensitive) == 0) { setTarget(Wt::TargetThisWindow); }
	else  /*(target.compare("TargetSelf", Qt::CaseInsensitive) == 0)*/     { setTarget(Wt::TargetSelf); }
}

QString WtQtAnchor::Wt_text()
{
	QString htext = QString::fromStdString(text().toUTF8());

	if (htext.compare(m_strText) != 0)
	{
		setText(Wt::WString::fromUTF8(m_strText.toStdString()));
		htext = QString::fromStdString(text().toUTF8());
	}

	return htext;

	//htext.replace("<", "&lt;");
	//htext.replace(">", "&gt;");
	//htext.replace('"', "'");
	//htext.replace("\n", "&#xA;"); // [NOTE] This must be better done by the user if desired, otherwise it corrupts html
}

void WtQtAnchor::Wt_setText(QString text)
{

	setText(Wt::WString::fromUTF8(text.toStdString()));
	m_strText = text;
}

QString WtQtAnchor::Wt_textFormat()
{
	// XHTMLText
	// Format text as XSS - safe XHTML markup'ed text.
	// XHTMLUnsafeText
	// Format text as XHTML markup'ed text.
	// PlainText
	// Format text as plain text.
	//
	// * The default format is Wt::XHTMLText.
	return QString("%1").arg((int)textFormat());
}

void WtQtAnchor::Wt_setTextFormat(QString textFormat)
{
	setTextFormat((Wt::TextFormat)textFormat.toInt());
	// refresh
	setText(Wt::WString::fromUTF8(m_strText.toStdString()));
}


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************************             WtQtText             ************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtText::WtQtText(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WText(wparent), WtQtInteractWidget(qparent)
{

}

WtQtText::WtQtText(const WtQtText& other)
{
	Q_UNUSED(other)
}

WtQtText::~WtQtText()
{

}

QString WtQtText::Wt_className()
{
	return "WText";
}

QString WtQtText::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtText::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtText::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtText::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtText::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtText::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtText::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtText::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtText::Wt_text()
{

	QString htext = QString::fromStdString(text().toUTF8());

	if (htext.compare(m_strText) != 0)
	{
		setText(Wt::WString::fromUTF8(m_strText.toStdString()));
		htext = QString::fromStdString(text().toUTF8());
	}

	return htext;

	//htext.replace("<", "&lt;");
	//htext.replace(">", "&gt;");
	//htext.replace('"', "'");
	//htext.replace("\n", "&#xA;"); // [NOTE] This must be better done by the user if desired, otherwise it corrupts html
}



void WtQtText::Wt_setText(QString text)
{

	setText(Wt::WString::fromUTF8(text.toStdString()));
	m_strText = text;



	//// Strapdown (working : download from https://github.com/arturadib/strapdown), and use iframe : (http://www.w3schools.com/tags/att_iframe_srcdoc.asp)
	//// Then paste the following text:
	//<iframe srcdoc="<title>Hello Strapdown</title>
	//<xmp theme=&quot;amelia&quot; style=&quot;display:none;&quot;>
	//# Markdown text goes in here
	//## Chapter 1
	//Lorem ipsum dolor sit amet, consectetur adipisicing elit. 
	//</xmp>
	//<script src=&quot;http://strapdownjs.com/v/0.2/strapdown.js &quot;></script>" style="height: 600px; width: 400px;"></iframe>

	//// ICE (working : download from: https://github.com/ajaxorg/ace-builds/)
	//setTextFormat(Wt::XHTMLUnsafeText); // Or '1' IMPORTANT
	//this->resize(500, 300);
	//QString strStrapDownJs = QString(g_strLocalResourcesPath + "ace-builds/src-noconflict/ace.js");
	//Wt::WApplication::instance()->require(strStrapDownJs.toStdString());
	//std::string editor_ref = this->jsRef(); // is a text string that will be the element when executed in JS
	//std::string command =
	//	editor_ref + ".editor = ace.edit(" + editor_ref + ");" +
	//	editor_ref + ".editor.setTheme(\"ace/theme/monokai\");" +
	//	editor_ref + ".editor.getSession().setMode(\"ace/mode/markdown\");";
	//this->doJavaScript(command);

}


QString WtQtText::Wt_textFormat()
{
	// XHTMLText
	// Format text as XSS - safe XHTML markup'ed text.
	// XHTMLUnsafeText
	// Format text as XHTML markup'ed text.
	// PlainText
	// Format text as plain text.
	//
	// * The default format is Wt::XHTMLText.
	return QString("%1").arg((int)textFormat());
}

void WtQtText::Wt_setTextFormat(QString textFormat)
{
	setTextFormat((Wt::TextFormat)textFormat.toInt());
	// refresh
	setText(Wt::WString::fromUTF8(m_strText.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
***********************************************************             WtQtLineEdit             **********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtLineEdit::WtQtLineEdit(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WLineEdit(wparent), WtQtFormWidget(qparent)
{
	m_boolIsSearch = false;
	// check if parent is navbar
	WtQtNavigationBar * navbar = qobject_cast<WtQtNavigationBar*>(qparent);
	if (navbar)
	{
		navbar->addSearch(this, Wt::AlignRight);
		m_boolIsSearch = true;
	}
}

WtQtLineEdit::WtQtLineEdit(const WtQtLineEdit& other)
{
	Q_UNUSED(other)
}

WtQtLineEdit::~WtQtLineEdit()
{

}

QString WtQtLineEdit::Wt_className()
{
	return "WLineEdit";
}

QString WtQtLineEdit::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtLineEdit::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtLineEdit::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtLineEdit::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtLineEdit::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtLineEdit::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtLineEdit::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtLineEdit::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtLineEdit::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtLineEdit::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}




/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*********************************************************             WtQtPushButton             **********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtPushButton::WtQtPushButton(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WPushButton(wparent), WtQtFormWidget(qparent)
{

}

WtQtPushButton::WtQtPushButton(const WtQtPushButton& other)
{
	Q_UNUSED(other)
}

WtQtPushButton::~WtQtPushButton()
{

}

QString WtQtPushButton::Wt_className()
{
	return "WPushButton";
}

QString WtQtPushButton::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtPushButton::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtPushButton::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtPushButton::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtPushButton::Wt_isInline()
{
	QString strRetVal = QString("%1").arg(Wt::WPushButton::isInline());
	return strRetVal;
}

void WtQtPushButton::Wt_setInline(QString isinline)
{
	Wt::WPushButton::setInline(isinline.toUInt());
}

//QString WtQtPushButton::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtPushButton::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

// QUrl::fromPercentEncoding : Returns a decoded copy of input. input is first decoded from percent encoding, then converted from UTF-8 to unicode.
// http://doc.qt.io/qt-5/qurl.html#fromPercentEncoding
// QUrl::toPercentEncoding   : Returns an encoded copy of input. input is first converted to UTF-8, and all ASCII-characters that are not in the unreserved group are percent encoded.
// http://doc.qt.io/qt-5/qurl.html#toPercentEncoding

QString WtQtPushButton::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtPushButton::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtPushButton::Wt_text()
{
	// [FIX] Now the problem is that this feeds both Qt side and autogencpp
	//       Now it works fine on the Qt site, but not autogencpp side
	//       Need to find an easy way to make both work
	return QString::fromStdString(text().toUTF8());
	//return QString::fromStdWString(text().value()).toUtf8();
}

void WtQtPushButton::Wt_setText(QString text)	
{
	setText(Wt::WString::fromUTF8(text.toUtf8().toStdString()));
}

QString WtQtPushButton::Wt_link()
{
	return QString::fromStdString(link().url());
}

void WtQtPushButton::Wt_setLink(QString link)
{
	setLink(Wt::WLink(link.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
**********************************************************             WtQtImage             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtImage::WtQtImage(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WImage(wparent), WtQtInteractWidget(qparent)
{
	strImageLink = "";
}

WtQtImage::WtQtImage(const WtQtImage& other)
{
	Q_UNUSED(other)
}

WtQtImage::~WtQtImage()
{

}

QString WtQtImage::Wt_className()
{
	return "WImage";
}

QString WtQtImage::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtImage::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtImage::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtImage::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtImage::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtImage::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtImage::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtImage::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtImage::Wt_imageLink()
{
	return strImageLink;
}

void WtQtImage::Wt_setImageLink(QString imagelink)
{
	setImageLink(Wt::WLink(imagelink.toStdString()));
	strImageLink = imagelink;
}


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtTemplate             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTemplate::WtQtTemplate(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTemplate(wparent), WtQtInteractWidget(qparent)
{
	m_textFormat = Wt::XHTMLText;
}

WtQtTemplate::WtQtTemplate(const WtQtTemplate& other)
{
	Q_UNUSED(other)
}

WtQtTemplate::~WtQtTemplate()
{

}

QString WtQtTemplate::Wt_className()
{
	return "WTemplate";
}

QString WtQtTemplate::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtTemplate::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtTemplate::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtTemplate::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTemplate::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtTemplate::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtTemplate::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtTemplate::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtTemplate::Wt_templateText()
{
	QString htext = QString::fromStdString(templateText().toUTF8());

	if (htext.compare(m_strText) != 0)
	{
		setTemplateText(Wt::WString::fromUTF8(m_strText.toStdString()), m_textFormat);
		htext = QString::fromStdString(templateText().toUTF8());
	}

	return htext;
	//htext.replace("<", "&lt;");
	//htext.replace(">", "&gt;");
	//htext.replace('"', "'");
	//htext.replace("\n", "&#xA;"); // [NOTE] This must be better done by the user if desired, otherwise it corrupts html

}

void WtQtTemplate::Wt_setTemplateText(QString templatetext)
{
	setTemplateText(Wt::WString::fromUTF8(templatetext.toStdString()), m_textFormat); // TODO : set as property and AUOTGENCPP
	m_strText = templatetext;
}


QString WtQtTemplate::Wt_textFormat()
{
	// XHTMLText
	// Format text as XSS - safe XHTML markup'ed text.
	// XHTMLUnsafeText
	// Format text as XHTML markup'ed text.
	// PlainText
	// Format text as plain text.
	//
	// * The default format is Wt::XHTMLText.
	return QString("%1").arg((int)m_textFormat);
}

void WtQtTemplate::Wt_setTextFormat(QString textFormat)
{
	m_textFormat = (Wt::TextFormat)textFormat.toInt();
	// refresh
	setTemplateText(Wt::WString::fromUTF8(m_strText.toStdString()), m_textFormat);
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtSplitButton             ***********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtSplitButton::WtQtSplitButton(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WSplitButton(wparent), WtQtCompositeWidget(qparent)
{

}

WtQtSplitButton::WtQtSplitButton(const WtQtSplitButton& other)
{
	Q_UNUSED(other)
}

WtQtSplitButton::~WtQtSplitButton()
{

}

QString WtQtSplitButton::Wt_className()
{
	return "WSplitButton";
}

QString WtQtSplitButton::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtSplitButton::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtSplitButton::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtSplitButton::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtSplitButton::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtSplitButton::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

QString WtQtSplitButton::Wt_text()
{
	return QString::fromStdString(actionButton()->text().toUTF8());
}

void WtQtSplitButton::Wt_setText(QString text)
{
	actionButton()->setText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtSplitButton::Wt_allItems()
{
	if (!menu()) { return ""; }
	QString strRetList = "";

	for (int i = 0; i < menu()->count(); i++)
	{
		// append item
		strRetList += QString::fromStdString(menu()->itemAt(i)->text().toUTF8());
		// add separator if necessary
		if(i != menu()->count())
		{
			strRetList += "\\n"; 
		}
	}
	return strRetList;
}

void WtQtSplitButton::Wt_setAllItems(QString strAllItems)
{
	// first remove all (book-keeping is too much effort)
	if (menu())
	{
		while (menu()->count() > 0)
		{
			menu()->removeItem(menu()->itemAt(0));
		}
		delete menu();
	}
	// check if any
	QStringList strListItems = strAllItems.split("\\n"); // this works
	if (strAllItems.count() <= 0) { return; }
	// separate by newline and add all
	Wt::WPopupMenu *popup = new Wt::WPopupMenu();
	for (int i = 0; i < strListItems.count(); i++)
	{
		popup->addItem(strListItems.at(i).toStdString());
	}
	//setMenu(popup);
	dropDownButton()->setMenu(popup);
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtRadioButton             ***********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtRadioButton::WtQtRadioButton(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WRadioButton("Some Text", wparent), WtQtAbstractToggleButton(qparent)
{
	m_printed = false;
	m_qtparent = dynamic_cast<WtQtWidget*>(qparent);
	if (!m_qtparent)
	{
		qDebug() << "[ERROR]";
	}
	QVector<WtQtRadioButton*> siblings = getSiblings();
	Wt::WButtonGroup * buttongroup = NULL;
	int i = 0;
	if (siblings.size() > 0)
	{
		while (!buttongroup && i < siblings.size())
		{
			buttongroup = siblings.at(i)->group();
		}
	}
	if (!buttongroup)
	{
		buttongroup = new Wt::WButtonGroup();
	}
	buttongroup->addButton(this, -1);
}

WtQtRadioButton::WtQtRadioButton(const WtQtRadioButton& other)
{
	Q_UNUSED(other)
}

WtQtRadioButton::~WtQtRadioButton()
{
	if (!m_qtparent) { return; }
	QVector<WtQtRadioButton*> siblings = getSiblings();
	Wt::WButtonGroup * buttongroup = group();
	if (!buttongroup) { return; }
	buttongroup->removeButton(this);
	if (siblings.size() <= 0)
	{
		delete buttongroup;
	}
}

QString WtQtRadioButton::Wt_className()
{
	return "WRadioButton";
}

QString WtQtRadioButton::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtRadioButton::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtRadioButton::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtRadioButton::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtRadioButton::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtRadioButton::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

QString WtQtRadioButton::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtRadioButton::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

//QString WtQtRadioButton::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtRadioButton::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtRadioButton::Wt_text()
{
	return QString::fromStdString(text().toUTF8());
}

void WtQtRadioButton::Wt_setText(QString text)
{
	setText(Wt::WString::fromUTF8(text.toStdString()));
}

QVector<WtQtRadioButton*> WtQtRadioButton::getSiblings()
{
	QVector<WtQtRadioButton*> retVector   ;
	WtQtRadioButton         * pRadioButton;
	QObject                 * childObj    ;
	if (!m_qtparent) { return retVector; }
	for (int j = 0; j < m_qtparent->children().count(); j++)
	{
		// check that is a valid button
		childObj = m_qtparent->children().at(j);
		if (!childObj) { continue; }
		if (childObj->property("Wt_className").toString().compare("WRadioButton", Qt::CaseInsensitive) == 0)
		{
			pRadioButton = dynamic_cast<WtQtRadioButton*>(m_qtparent->children().at(j));
			if (pRadioButton != this)
			{
				retVector.append(pRadioButton);
			}
		}
	}
	return retVector;
}


bool WtQtRadioButton::isFirstToPrint()
{
	bool boolIsFirst = true; // [NOTE] needs to be true by default to handle the case where there are no siblings
	QVector<WtQtRadioButton*> siblings = getSiblings();
	for(int i = 0; i < siblings.count(); i++)
	{
		if (!siblings.at(i)->m_printed)
		{
			boolIsFirst = true;
		}
		else
		{
			boolIsFirst = false;
			break;
		}
	}
	return boolIsFirst;
}

bool WtQtRadioButton::isLastToPrint()
{
	bool boolIsLast = false;
	QVector<WtQtRadioButton*> siblings = getSiblings();
	for (int i = 0; i < siblings.count(); i++)
	{
		if (siblings.at(i)->m_printed)
		{
			boolIsLast = true;
		}
		else
		{
			boolIsLast = false;
			break;
		}
	}
	return boolIsLast;
}

void WtQtRadioButton::clearAllPrinted()
{
	// clear all siblings flags and this flag
	QVector<WtQtRadioButton*> siblings = getSiblings();
	for (int i = 0; i < siblings.count(); i++)
	{
		siblings.at(i)->m_printed = false;
	}
	m_printed = false;
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
**********************************************************             WtQtCheckBox             ***********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtCheckBox::WtQtCheckBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WCheckBox("Some Text", wparent), WtQtAbstractToggleButton(qparent)
{

}

WtQtCheckBox::WtQtCheckBox(const WtQtCheckBox& other)
{
	Q_UNUSED(other)
}

WtQtCheckBox::~WtQtCheckBox()
{

}

QString WtQtCheckBox::Wt_className()
{
	return "WCheckBox";
}

QString WtQtCheckBox::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtCheckBox::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtCheckBox::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtCheckBox::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtCheckBox::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtCheckBox::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtCheckBox::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtCheckBox::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtCheckBox::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtCheckBox::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtCheckBox::Wt_text()
{
	return QString::fromStdString(text().toUTF8());
}

void WtQtCheckBox::Wt_setText(QString text)
{
	setText(Wt::WString::fromUTF8(text.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
**********************************************************             WtQtComboBox             ***********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtComboBox::WtQtComboBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WComboBox(wparent), WtQtFormWidget(qparent)
{

}

WtQtComboBox::WtQtComboBox(const WtQtComboBox& other)
{
	Q_UNUSED(other)
}

WtQtComboBox::~WtQtComboBox()
{

}

QString WtQtComboBox::Wt_className()
{
	return "WComboBox";
}

QString WtQtComboBox::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtComboBox::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtComboBox::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtComboBox::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtComboBox::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtComboBox::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtComboBox::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtComboBox::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtComboBox::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtComboBox::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtComboBox::Wt_allItems()
{
	QString strRetList = "";
	for (int i = 0; i < count(); i++)
	{
		// append item
		strRetList += QString::fromStdString(itemText(i).toUTF8());
		// add separator if necessary
		if (i != count())
		{
			strRetList += "\\n";
		}
	}
	return strRetList;
}

void WtQtComboBox::Wt_setAllItems(QString strAllItems)
{
	// first remove all (book-keeping is too much effort)
	while (count() > 0)
	{
		removeItem(0);
	}
	// check if any
	QStringList strListItems = strAllItems.split("\\n"); // this works
	if (strAllItems.count() <= 0) { return; }
	// separate by newline and add all
	for (int i = 0; i < strListItems.count(); i++)
	{
		addItem(strListItems.at(i).toStdString());
	}
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtInPlaceEdit             ***********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/


WtQtInPlaceEdit::WtQtInPlaceEdit(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WInPlaceEdit("This is editable text", wparent), WtQtCompositeWidget(qparent)
{
	m_boolButtonsEnabled = true;
}

WtQtInPlaceEdit::WtQtInPlaceEdit(const WtQtInPlaceEdit& other)
{
	Q_UNUSED(other)
}

WtQtInPlaceEdit::~WtQtInPlaceEdit()
{

}

QString WtQtInPlaceEdit::Wt_className()
{
	return "WInPlaceEdit";
}

QString WtQtInPlaceEdit::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtInPlaceEdit::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtInPlaceEdit::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtInPlaceEdit::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtInPlaceEdit::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtInPlaceEdit::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

QString WtQtInPlaceEdit::Wt_text()
{
	return QString::fromStdString(text().toUTF8());
}

void WtQtInPlaceEdit::Wt_setText(QString text)
{
	setText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtInPlaceEdit::Wt_placeholderText()
{
	return QString::fromStdString(placeholderText().toUTF8());
}

void WtQtInPlaceEdit::Wt_setPlaceholderText(QString text)
{
	setPlaceholderText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtInPlaceEdit::Wt_buttonsEnabled()
{
	return QString("%1").arg(m_boolButtonsEnabled);
}

void WtQtInPlaceEdit::Wt_setButtonsEnabled(QString isinline)
{
	m_boolButtonsEnabled = isinline.toUInt(); setButtonsEnabled(m_boolButtonsEnabled);
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtTextArea             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTextArea::WtQtTextArea(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTextArea(wparent), WtQtFormWidget(qparent)
{

}

WtQtTextArea::WtQtTextArea(const WtQtTextArea& other)
{
	Q_UNUSED(other)
}

WtQtTextArea::~WtQtTextArea()
{

}

QString WtQtTextArea::Wt_className()
{
	return "WTextArea";
}

QString WtQtTextArea::Wt_id()
{
	return QString::fromStdString(Wt::WTextArea::id());
}

void WtQtTextArea::Wt_setId(QString id)
{
	Wt::WTextArea::setId(id.toStdString());
}

QString WtQtTextArea::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTextArea::styleClass().toUTF8());
}

void WtQtTextArea::Wt_setStyleClass(QString styleclass)
{
	Wt::WTextArea::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTextArea::Wt_isInline()
{
	return QString("%1").arg(Wt::WTextArea::isInline());
}

void WtQtTextArea::Wt_setInline(QString isinline)
{
	Wt::WTextArea::setInline(isinline.toUInt());
}

//QString WtQtTextArea::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WTextArea::htmlTagName());
//}
//
//void WtQtTextArea::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WTextArea::setHtmlTagName(tagname.toStdString());
//}

QString WtQtTextArea::Wt_emptyText()
{
	return QString::fromStdString(Wt::WTextArea::emptyText().toUTF8());
}

void WtQtTextArea::Wt_setEmptyText(QString emptytext)
{
	Wt::WTextArea::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtTextArea::Wt_text()
{
	return QString::fromStdString(Wt::WTextArea::text().toUTF8());
}

void WtQtTextArea::Wt_setText(QString text)
{
	Wt::WTextArea::setText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtTextArea::Wt_columns()
{
	return QString("%1").arg(Wt::WTextArea::columns());
}

void WtQtTextArea::Wt_setColumns(QString columns)
{
	Wt::WTextArea::setColumns(columns.toInt());
}

QString WtQtTextArea::Wt_rows()
{
	return QString("%1").arg(Wt::WTextArea::rows());
}

void WtQtTextArea::Wt_setRows(QString rows)
{
	Wt::WTextArea::setRows(rows.toInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtSelectionBox             **********************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtSelectionBox::WtQtSelectionBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WSelectionBox(wparent), WtQtComboBox(wparent, qparent)
{
	WtQtComboBox::setHidden(true);
}

WtQtSelectionBox::WtQtSelectionBox(const WtQtSelectionBox& other)
{
	Q_UNUSED(other)
}

WtQtSelectionBox::~WtQtSelectionBox()
{

}

QString WtQtSelectionBox::Wt_className()
{
	return "WSelectionBox";
}

QString WtQtSelectionBox::Wt_id()
{
	return QString::fromStdString(Wt::WSelectionBox::id());
}

void WtQtSelectionBox::Wt_setId(QString id)
{
	Wt::WSelectionBox::setId(id.toStdString());
}

QString WtQtSelectionBox::Wt_styleClass()
{
	return QString::fromStdString(Wt::WSelectionBox::styleClass().toUTF8());
}

void WtQtSelectionBox::Wt_setStyleClass(QString styleclass)
{
	Wt::WSelectionBox::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtSelectionBox::Wt_isInline()
{
	return QString("%1").arg(Wt::WSelectionBox::isInline());
}

void WtQtSelectionBox::Wt_setInline(QString isinline)
{
	Wt::WSelectionBox::setInline(isinline.toUInt());
}

//QString WtQtSelectionBox::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WSelectionBox::htmlTagName());
//}
//
//void WtQtSelectionBox::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WSelectionBox::setHtmlTagName(tagname.toStdString());
//}

QString WtQtSelectionBox::Wt_emptyText()
{
	return QString::fromStdString(Wt::WSelectionBox::emptyText().toUTF8());
}

void WtQtSelectionBox::Wt_setEmptyText(QString emptytext)
{
	Wt::WSelectionBox::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtSelectionBox::Wt_allItems()
{
	QString strRetList = "";
	for (int i = 0; i < Wt::WSelectionBox::count(); i++)
	{
		// append item
		strRetList += QString::fromStdString(Wt::WSelectionBox::itemText(i).toUTF8());
		// add separator if necessary
		if (i != Wt::WSelectionBox::count())
		{
			strRetList += "\\n";
		}
	}
	return strRetList;
}

void WtQtSelectionBox::Wt_setAllItems(QString strAllItems)
{
	// first remove all (book-keeping is too much effort)
	while (Wt::WSelectionBox::count() > 0)
	{
		Wt::WSelectionBox::removeItem(0);
	}
	// check if any
	QStringList strListItems = strAllItems.split("\\n"); // this works
	if (strAllItems.count() <= 0) { return; }
	// separate by newline and add all
	for (int i = 0; i < strListItems.count(); i++)
	{
		Wt::WSelectionBox::addItem(strListItems.at(i).toStdString());
	}
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtSpinBox             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtSpinBox::WtQtSpinBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WSpinBox(wparent), WtQtLineEdit(wparent, qparent)
{
	WtQtLineEdit::setHidden(true);
	// TODO : locale error
	// TODO : set disabled?
}

WtQtSpinBox::WtQtSpinBox(const WtQtSpinBox& other)
{
	Q_UNUSED(other)
}

WtQtSpinBox::~WtQtSpinBox()
{

}

QString WtQtSpinBox::Wt_className()
{
	return "WSpinBox";
}

QString WtQtSpinBox::Wt_id()
{
	return QString::fromStdString(Wt::WSpinBox::id());
}

void WtQtSpinBox::Wt_setId(QString id)
{
	Wt::WSpinBox::setId(id.toStdString());
}

QString WtQtSpinBox::Wt_styleClass()
{
	return QString::fromStdString(Wt::WSpinBox::styleClass().toUTF8());
}

void WtQtSpinBox::Wt_setStyleClass(QString styleclass)
{
	Wt::WSpinBox::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtSpinBox::Wt_isInline()
{
	return QString("%1").arg(Wt::WSpinBox::isInline());
}

void WtQtSpinBox::Wt_setInline(QString isinline)
{
	Wt::WSpinBox::setInline(isinline.toUInt());
}

//QString WtQtSpinBox::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WSpinBox::htmlTagName());
//}
//
//void WtQtSpinBox::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WSpinBox::setHtmlTagName(tagname.toStdString());
//}

QString WtQtSpinBox::Wt_emptyText()
{
	return QString::fromStdString(Wt::WSpinBox::emptyText().toUTF8());
}

void WtQtSpinBox::Wt_setEmptyText(QString emptytext)
{
	Wt::WSpinBox::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtSpinBox::Wt_minimum()
{
	return QString("%1").arg(Wt::WSpinBox::minimum());
}

void WtQtSpinBox::Wt_setMinimum(QString minimum)
{
	Wt::WSpinBox::setMinimum(minimum.toInt());
}

QString WtQtSpinBox::Wt_maximum()
{
	return QString("%1").arg(Wt::WSpinBox::maximum());
}

void WtQtSpinBox::Wt_setMaximum(QString maximum)
{
	Wt::WSpinBox::setMaximum(maximum.toInt());
}

QString WtQtSpinBox::Wt_value()
{
	return QString("%1").arg(Wt::WSpinBox::value());
}

void WtQtSpinBox::Wt_setValue(QString value)
{
	Wt::WSpinBox::setValue(value.toInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************             WtQtDoubleSpinBox             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtDoubleSpinBox::WtQtDoubleSpinBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WDoubleSpinBox(wparent), WtQtLineEdit(wparent, qparent)
{
	WtQtLineEdit::setHidden(true);
	// TODO : locale error
	// TODO : set disabled?
}

WtQtDoubleSpinBox::WtQtDoubleSpinBox(const WtQtDoubleSpinBox& other)
{
	Q_UNUSED(other)
}

WtQtDoubleSpinBox::~WtQtDoubleSpinBox()
{

}

QString WtQtDoubleSpinBox::Wt_className()
{
	return "WDoubleSpinBox";
}

QString WtQtDoubleSpinBox::Wt_id()
{
	return QString::fromStdString(Wt::WDoubleSpinBox::id());
}

void WtQtDoubleSpinBox::Wt_setId(QString id)
{
	Wt::WDoubleSpinBox::setId(id.toStdString());
}

QString WtQtDoubleSpinBox::Wt_styleClass()
{
	return QString::fromStdString(Wt::WDoubleSpinBox::styleClass().toUTF8());
}

void WtQtDoubleSpinBox::Wt_setStyleClass(QString styleclass)
{
	Wt::WDoubleSpinBox::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtDoubleSpinBox::Wt_isInline()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::isInline());
}

void WtQtDoubleSpinBox::Wt_setInline(QString isinline)
{
	Wt::WDoubleSpinBox::setInline(isinline.toUInt());
}

//QString WtQtDoubleSpinBox::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WDoubleSpinBox::htmlTagName());
//}
//
//void WtQtDoubleSpinBox::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WDoubleSpinBox::setHtmlTagName(tagname.toStdString());
//}

QString WtQtDoubleSpinBox::Wt_emptyText()
{
	return QString::fromStdString(Wt::WDoubleSpinBox::emptyText().toUTF8());
}

void WtQtDoubleSpinBox::Wt_setEmptyText(QString emptytext)
{
	Wt::WDoubleSpinBox::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtDoubleSpinBox::Wt_minimum()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::minimum());
}

void WtQtDoubleSpinBox::Wt_setMinimum(QString minimum)
{
	Wt::WDoubleSpinBox::setMinimum(minimum.toDouble());
}

QString WtQtDoubleSpinBox::Wt_maximum()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::maximum());
}

void WtQtDoubleSpinBox::Wt_setMaximum(QString maximum)
{
	Wt::WDoubleSpinBox::setMaximum(maximum.toDouble());
}

QString WtQtDoubleSpinBox::Wt_value()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::value());
}

void WtQtDoubleSpinBox::Wt_setValue(QString value)
{
	Wt::WDoubleSpinBox::setValue(value.toDouble());
}

QString WtQtDoubleSpinBox::Wt_singleStep()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::singleStep());
}

void WtQtDoubleSpinBox::Wt_setSingleStep(QString step)
{
	Wt::WDoubleSpinBox::setSingleStep(step.toDouble());
}

QString WtQtDoubleSpinBox::Wt_decimals()
{
	return QString("%1").arg(Wt::WDoubleSpinBox::decimals());
}

void WtQtDoubleSpinBox::Wt_setDecimals(QString decimals)
{
	Wt::WDoubleSpinBox::setDecimals(decimals.toInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
******************************************************             WtQtTimeEdit             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTimeEdit::WtQtTimeEdit(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTimeEdit(wparent), WtQtLineEdit(wparent, qparent)
{
	WtQtLineEdit::setHidden(true);
	setTime(Wt::WTime::currentServerTime());
	// TODO : locale error
	// TODO : set disabled?
}

WtQtTimeEdit::WtQtTimeEdit(const WtQtTimeEdit& other)
{
	Q_UNUSED(other)
}

WtQtTimeEdit::~WtQtTimeEdit()
{

}

QString WtQtTimeEdit::Wt_className()
{
	return "WTimeEdit";
}

QString WtQtTimeEdit::Wt_id()
{
	return QString::fromStdString(Wt::WTimeEdit::id());
}

void WtQtTimeEdit::Wt_setId(QString id)
{
	Wt::WTimeEdit::setId(id.toStdString());
}

QString WtQtTimeEdit::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTimeEdit::styleClass().toUTF8());
}

void WtQtTimeEdit::Wt_setStyleClass(QString styleclass)
{
	Wt::WTimeEdit::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTimeEdit::Wt_isInline()
{
	return QString("%1").arg(Wt::WTimeEdit::isInline());
}

void WtQtTimeEdit::Wt_setInline(QString isinline)
{
	Wt::WTimeEdit::setInline(isinline.toUInt());
}

//QString WtQtTimeEdit::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WTimeEdit::htmlTagName());
//}
//
//void WtQtTimeEdit::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WTimeEdit::setHtmlTagName(tagname.toStdString());
//}

QString WtQtTimeEdit::Wt_emptyText()
{
	return QString::fromStdString(Wt::WTimeEdit::emptyText().toUTF8());
}

void WtQtTimeEdit::Wt_setEmptyText(QString emptytext)
{
	Wt::WTimeEdit::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtTimeEdit::Wt_minuteStep()
{
	return QString("%1").arg(Wt::WTimeEdit::minuteStep());
}

void WtQtTimeEdit::Wt_setMinuteStep(QString step)
{
	Wt::WTimeEdit::setMinuteStep(step.toInt());
}

QString WtQtTimeEdit::Wt_format()
{
	return QString::fromStdString(Wt::WTimeEdit::format().toUTF8());
}

void WtQtTimeEdit::Wt_setFormat(QString format)
{
	Wt::WTimeEdit::setFormat(Wt::WString::fromUTF8(format.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
******************************************************             WtQtDateEdit             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtDateEdit::WtQtDateEdit(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WDateEdit(wparent), WtQtLineEdit(wparent, qparent)
{
	WtQtLineEdit::setHidden(true);
	setDate(Wt::WDate::currentServerDate());
	// TODO : locale error
	// TODO : set disabled?
}

WtQtDateEdit::WtQtDateEdit(const WtQtDateEdit& other)
{
	Q_UNUSED(other)
}

WtQtDateEdit::~WtQtDateEdit()
{

}

QString WtQtDateEdit::Wt_className()
{
	return "WDateEdit";
}

QString WtQtDateEdit::Wt_id()
{
	return QString::fromStdString(Wt::WDateEdit::id());
}

void WtQtDateEdit::Wt_setId(QString id)
{
	Wt::WDateEdit::setId(id.toStdString());
}

QString WtQtDateEdit::Wt_styleClass()
{
	return QString::fromStdString(Wt::WDateEdit::styleClass().toUTF8());
}

void WtQtDateEdit::Wt_setStyleClass(QString styleclass)
{
	Wt::WDateEdit::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtDateEdit::Wt_isInline()
{
	return QString("%1").arg(Wt::WDateEdit::isInline());
}

void WtQtDateEdit::Wt_setInline(QString isinline)
{
	Wt::WDateEdit::setInline(isinline.toUInt());
}

//QString WtQtDateEdit::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WDateEdit::htmlTagName());
//}
//
//void WtQtDateEdit::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WDateEdit::setHtmlTagName(tagname.toStdString());
//}

QString WtQtDateEdit::Wt_emptyText()
{
	return QString::fromStdString(Wt::WDateEdit::emptyText().toUTF8());
}

void WtQtDateEdit::Wt_setEmptyText(QString emptytext)
{
	Wt::WDateEdit::setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtDateEdit::Wt_format()
{
	return QString::fromStdString(Wt::WDateEdit::format().toUTF8());
}

void WtQtDateEdit::Wt_setFormat(QString format)
{
	Wt::WDateEdit::setFormat(Wt::WString::fromUTF8(format.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
******************************************************             WtQtWCalendar             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtWCalendar::WtQtWCalendar(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WCalendar(wparent), WtQtCompositeWidget(qparent)
{

}

WtQtWCalendar::WtQtWCalendar(const WtQtWCalendar& other)
{
	Q_UNUSED(other)
}

WtQtWCalendar::~WtQtWCalendar()
{

}

QString WtQtWCalendar::Wt_className()
{
	return "WCalendar";
}

QString WtQtWCalendar::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtWCalendar::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtWCalendar::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtWCalendar::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtWCalendar::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtWCalendar::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*********************************************************             WtQtSlider             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtSlider::WtQtSlider(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WSlider(wparent), WtQtFormWidget(qparent)
{

}

WtQtSlider::WtQtSlider(const WtQtSlider& other)
{
	Q_UNUSED(other)
}

WtQtSlider::~WtQtSlider()
{

}

QString WtQtSlider::Wt_className()
{
	return "WSlider";
}

QString WtQtSlider::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtSlider::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtSlider::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtSlider::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtSlider::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtSlider::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtSlider::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtSlider::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtSlider::Wt_emptyText()
{
	return QString::fromStdString(emptyText().toUTF8());
}

void WtQtSlider::Wt_setEmptyText(QString emptytext)
{
	setEmptyText(Wt::WString::fromUTF8(emptytext.toStdString()));
}

QString WtQtSlider::Wt_minimum()
{
	return QString("%1").arg(Wt::WSlider::minimum());
}

void WtQtSlider::Wt_setMinimum(QString minimum)
{
	Wt::WSlider::setMinimum(minimum.toInt());
}

QString WtQtSlider::Wt_maximum()
{
	return QString("%1").arg(Wt::WSlider::maximum());
}

void WtQtSlider::Wt_setMaximum(QString maximum)
{
	Wt::WSlider::setMaximum(maximum.toInt());
}

QString WtQtSlider::Wt_value()
{
	return QString("%1").arg(Wt::WSlider::value());
}

void WtQtSlider::Wt_setValue(QString value)
{
	Wt::WSlider::setValue(value.toInt());
}

QString WtQtSlider::Wt_tickInterval()
{
	return QString("%1").arg(Wt::WSlider::tickInterval());
}

void WtQtSlider::Wt_setTickInterval(QString interval)
{
	Wt::WSlider::setTickInterval(interval.toInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtFileUpload             **************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtFileUpload::WtQtFileUpload(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WFileUpload(wparent), WtQtWebWidget(qparent)
{

}

WtQtFileUpload::WtQtFileUpload(const WtQtFileUpload& other)
{
	Q_UNUSED(other)
}

WtQtFileUpload::~WtQtFileUpload()
{

}

QString WtQtFileUpload::Wt_className()
{
	return "WFileUpload";
}

QString WtQtFileUpload::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtFileUpload::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtFileUpload::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtFileUpload::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtFileUpload::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtFileUpload::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtFileUpload::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtFileUpload::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtProgressBar             *************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtProgressBar::WtQtProgressBar(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WProgressBar(wparent), WtQtInteractWidget(qparent)
{

}

WtQtProgressBar::WtQtProgressBar(const WtQtProgressBar& other)
{
	Q_UNUSED(other)
}

WtQtProgressBar::~WtQtProgressBar()
{

}

QString WtQtProgressBar::Wt_className()
{
	return "WProgressBar";
}

QString WtQtProgressBar::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtProgressBar::Wt_setId(QString id)
{
	setId(id.toStdString());
}

QString WtQtProgressBar::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtProgressBar::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtProgressBar::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtProgressBar::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

//QString WtQtProgressBar::Wt_htmlTagName()
//{
//	return QString::fromStdString(htmlTagName());
//}
//
//void WtQtProgressBar::Wt_setHtmlTagName(QString tagname)
//{
//	setHtmlTagName(tagname.toStdString());
//}

QString WtQtProgressBar::Wt_minimum()
{
	return QString("%1").arg(Wt::WProgressBar::minimum());
}

void WtQtProgressBar::Wt_setMinimum(QString minimum)
{
	Wt::WProgressBar::setMinimum(minimum.toDouble());
}

QString WtQtProgressBar::Wt_maximum()
{
	return QString("%1").arg(Wt::WProgressBar::maximum());
}

void WtQtProgressBar::Wt_setMaximum(QString maximum)
{
	Wt::WProgressBar::setMaximum(maximum.toDouble());
}

QString WtQtProgressBar::Wt_value()
{
	return QString("%1").arg(Wt::WProgressBar::value());
}

void WtQtProgressBar::Wt_setValue(QString value)
{
	Wt::WProgressBar::setValue(value.toDouble());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtGroupBox             ****************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtGroupBox::WtQtGroupBox(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WGroupBox(wparent), WtQtContainerWidget(wparent, qparent)
{

}

WtQtGroupBox::WtQtGroupBox(const WtQtGroupBox& other)
{
	Q_UNUSED(other)
}

WtQtGroupBox::~WtQtGroupBox()
{

}

QString WtQtGroupBox::Wt_className()
{
	return "WGroupBox";
}

QString WtQtGroupBox::Wt_id()
{
	return QString::fromStdString(Wt::WGroupBox::id());
}

void WtQtGroupBox::Wt_setId(QString id)
{
	Wt::WGroupBox::setId(id.toStdString());
}

QString WtQtGroupBox::Wt_styleClass()
{
	return QString::fromStdString(Wt::WGroupBox::styleClass().toUTF8());
}

void WtQtGroupBox::Wt_setStyleClass(QString styleclass)
{
	Wt::WGroupBox::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtGroupBox::Wt_isInline()
{
	return QString("%1").arg(Wt::WGroupBox::isInline());
}

void WtQtGroupBox::Wt_setInline(QString isinline)
{
	Wt::WGroupBox::setInline(isinline.toUInt());
}

QString WtQtGroupBox::Wt_htmlTagName()
{
	return QString::fromStdString(Wt::WGroupBox::htmlTagName());
}

void WtQtGroupBox::Wt_setHtmlTagName(QString tagname)
{
	//Wt::WGroupBox::setHtmlTagName(tagname.toStdString());
}

QString WtQtGroupBox::Wt_title()
{
	return QString::fromStdString(Wt::WGroupBox::title().toUTF8());
}

void WtQtGroupBox::Wt_setTitle(QString text)
{
	Wt::WGroupBox::setTitle(Wt::WString::fromUTF8(text.toStdString()));
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtPanel             *****************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtPanel::WtQtPanel(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WPanel(wparent), WtQtCompositeWidget(qparent)
{
	if (wparent == NULL)
	{
		wparent = GetWContainerParent(qparent);
	}
	if (!wparent)
	{
		qDebug() << "[ERROR] Invalid Wt parent in WtQtPanel::WtQtPanel";
	}
	m_centralWidget = new Wt::WContainerWidget(wparent); // TODO : will this work with null wparent?
	setCentralWidget(m_centralWidget);
}

WtQtPanel::WtQtPanel(const WtQtPanel& other)
{
	Q_UNUSED(other)
}

WtQtPanel::~WtQtPanel()
{

}

QString WtQtPanel::Wt_className()
{
	return "WPanel";
}

QString WtQtPanel::Wt_id()
{
	return QString::fromStdString(id());
}

void WtQtPanel::Wt_setId(QString id)
{
	setId(id.toStdString());
	m_strCentralWidgetId = "cw_" + id;
}

QString WtQtPanel::Wt_styleClass()
{
	return QString::fromStdString(styleClass().toUTF8());
}

void WtQtPanel::Wt_setStyleClass(QString styleclass)
{
	setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtPanel::Wt_isInline()
{
	return QString("%1").arg(isInline());
}

void WtQtPanel::Wt_setInline(QString isinline)
{
	setInline(isinline.toUInt());
}

QString WtQtPanel::Wt_title()
{
	return QString::fromStdString(title().toUTF8());
}

void WtQtPanel::Wt_setTitle(QString text)
{
	setTitle(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtPanel::Wt_isCollapsible()
{
	return QString("%1").arg(isCollapsible());
}

void WtQtPanel::Wt_setCollapsible(QString collapsible)
{
	setCollapsible(collapsible.toUInt());
}

Wt::WContainerWidget* WtQtPanel::getCentralContainer()
{
	return m_centralWidget;
}

QString WtQtPanel::getCentralWidgetId()
{
	return m_strCentralWidgetId;
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************             WtQtStackedWidget             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtStackedWidget::WtQtStackedWidget(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WStackedWidget(wparent), WtQtContainerWidget(wparent, qparent)
{

}

WtQtStackedWidget::WtQtStackedWidget(const WtQtStackedWidget& other)
{
	Q_UNUSED(other)
}

WtQtStackedWidget::~WtQtStackedWidget()
{

}

QString WtQtStackedWidget::Wt_className()
{
	return "WStackedWidget";
}

QString WtQtStackedWidget::Wt_id()
{
	return QString::fromStdString(Wt::WStackedWidget::id());
}

void WtQtStackedWidget::Wt_setId(QString id)
{
	Wt::WStackedWidget::setId(id.toStdString());
}

QString WtQtStackedWidget::Wt_styleClass()
{
	return QString::fromStdString(Wt::WStackedWidget::styleClass().toUTF8());
}

void WtQtStackedWidget::Wt_setStyleClass(QString styleclass)
{
	Wt::WStackedWidget::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtStackedWidget::Wt_isInline()
{
	return QString("%1").arg(Wt::WStackedWidget::isInline());
}

void WtQtStackedWidget::Wt_setInline(QString isinline)
{
	Wt::WStackedWidget::setInline(isinline.toUInt());
}

QString WtQtStackedWidget::Wt_htmlTagName()
{
	return QString::fromStdString(Wt::WStackedWidget::htmlTagName());
}

void WtQtStackedWidget::Wt_setHtmlTagName(QString tagname)
{
	//Wt::WStackedWidget::setHtmlTagName(tagname.toStdString());
}

QString WtQtStackedWidget::Wt_currentIndex()
{
	return QString("%1").arg(currentIndex());
}

void WtQtStackedWidget::Wt_setCurrentIndex(QString index)
{
	int intIdx = index.toUInt();
	if (intIdx < QObject::children().count())
	{
		setCurrentIndex(intIdx);
	}
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************             WtQtWTabWidget             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTabWidget::WtQtTabWidget(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTabWidget(wparent), WtQtCompositeWidget(qparent)
{
	m_beingDeleted = false;
	m_wparent = NULL;
	if (wparent)
	{
		m_wparent = wparent;
	}
	else
	{
		m_wparent = GetWContainerParent(qparent);
		if (!m_wparent)
		{
			qDebug() << "[ERROR] Invalid Wt parent in WtQtWTabWidget::WtQtWTabWidget";
		}
	}
}

WtQtTabWidget::WtQtTabWidget(const WtQtTabWidget& other)
{
	Q_UNUSED(other)
}

WtQtTabWidget::~WtQtTabWidget()
{
	m_beingDeleted = true;
}

QString WtQtTabWidget::Wt_className()
{
	return "WTabWidget";
}

QString WtQtTabWidget::Wt_id()
{
	return QString::fromStdString(Wt::WTabWidget::id());
}

void WtQtTabWidget::Wt_setId(QString id)
{
	Wt::WTabWidget::setId(id.toStdString());
}

QString WtQtTabWidget::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTabWidget::styleClass().toUTF8());
}

void WtQtTabWidget::Wt_setStyleClass(QString styleclass)
{
	Wt::WTabWidget::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTabWidget::Wt_isInline()
{
	return QString("%1").arg(Wt::WTabWidget::isInline());
}

void WtQtTabWidget::Wt_setInline(QString isinline)
{
	Wt::WTabWidget::setInline(isinline.toUInt());
}

Wt::WContainerWidget * WtQtTabWidget::getWtParent()
{
	return m_wparent;
}

bool WtQtTabWidget::isBeingDeleted()
{
	return m_beingDeleted;
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************             WtQtTabItem             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/


WtQtTabItem::WtQtTabItem(WtQtTabWidget *qparent) : WtQtContainerWidget(qparent->getWtParent(), qparent)
{
	m_qparent     = qparent;
	m_childWidget = dynamic_cast<Wt::WContainerWidget*>(this);
	m_qparent->addTab(m_childWidget, Wt::WString("Label"));
}

WtQtTabItem::WtQtTabItem(const WtQtTabItem& other)
{
	Q_UNUSED(other)
}

WtQtTabItem::~WtQtTabItem()
{
	if (!m_qparent->isBeingDeleted())
	{
		m_qparent->removeTab(m_childWidget);
	}
}

QString WtQtTabItem::Wt_className()
{
	return "WTabItem"; // [NOTE] Added exception in helperfunctions::ListAllIncludes for Cpp autogen
}

QString WtQtTabItem::Wt_id()
{
	return QString::fromStdString(m_childWidget->id());
}

void WtQtTabItem::Wt_setId(QString id)
{
	m_childWidget->setId(id.toStdString());
}

QString WtQtTabItem::Wt_styleClass()
{
	return QString::fromStdString(m_childWidget->styleClass().toUTF8());
}

void WtQtTabItem::Wt_setStyleClass(QString styleclass)
{
	m_childWidget->setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTabItem::Wt_isInline()
{
	return QString("%1").arg(m_childWidget->isInline());
}

void WtQtTabItem::Wt_setInline(QString isinline)
{
	m_childWidget->setInline(isinline.toUInt());
}

QString WtQtTabItem::Wt_htmlTagName()
{
	return QString::fromStdString(m_childWidget->htmlTagName());
}

void WtQtTabItem::Wt_setHtmlTagName(QString tagname)
{
	//m_childWidget->setHtmlTagName(tagname.toStdString());
}

QString WtQtTabItem::Wt_tabText()
{
	return QString::fromStdString(m_qparent->tabText(m_qparent->indexOf(m_childWidget)).toUTF8());
}

void WtQtTabItem::Wt_setTabText(QString tabtext)
{
	m_qparent->setTabText(m_qparent->indexOf(m_childWidget), Wt::WString::fromUTF8(tabtext.toStdString()));
}

QString WtQtTabItem::Wt_isTabEnabled()
{
	return QString("%1").arg(m_qparent->isTabEnabled(m_qparent->indexOf(m_childWidget)));
}

void WtQtTabItem::Wt_setTabEnabled(QString benabled)
{
	m_qparent->setTabEnabled(m_qparent->indexOf(m_childWidget), benabled.toUInt());
}

QString WtQtTabItem::Wt_isTabHidden()
{
	return QString("%1").arg(m_qparent->isTabHidden(m_qparent->indexOf(m_childWidget)));
}

void WtQtTabItem::Wt_setTabHidden(QString bhidden)
{
	m_qparent->setTabHidden(m_qparent->indexOf(m_childWidget), bhidden.toUInt());
}

QString WtQtTabItem::Wt_isTabCloseable()
{
	return QString("%1").arg(m_qparent->isTabCloseable(m_qparent->indexOf(m_childWidget)));
}

void WtQtTabItem::Wt_setTabCloseable(QString bclosable)
{
	m_qparent->setTabCloseable(m_qparent->indexOf(m_childWidget), bclosable.toUInt());
}


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtWMenu             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// TODO : overload cpp_instantiate to acomodate wstackedwidget
WtQtMenu::WtQtMenu(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WMenu(new Wt::WStackedWidget(), wparent), WtQtCompositeWidget(qparent)
{
	m_isLeftWMenu  = false;
	m_beingDeleted = false;
	m_wparent = NULL;
	if (wparent)
	{
		m_wparent = wparent;
	}
	else
	{
		m_wparent = GetWContainerParent(qparent);
	}
	if (m_wparent)
	{
		// add stacked widget to same menu parent
		m_wparent->addWidget(Wt::WMenu::contentsStack());
	}
	else
	{
		qDebug() << "[ERROR] Invalid Wt parent in WtQtMenu::WtQtMenu";
	}
	// check if parent is navbar
	WtQtNavigationBar * navbar = qobject_cast<WtQtNavigationBar*>(qparent);
	if (navbar)
	{
		navbar->addMenu(this, Wt::AlignLeft);
		m_isLeftWMenu = true;
	}
}
WtQtMenu::WtQtMenu(const WtQtMenu& other)
{
	Q_UNUSED(other)
}

WtQtMenu::~WtQtMenu()
{
	m_beingDeleted = true;
}

QString WtQtMenu::Wt_className()
{
	return "WMenu";
}

QString WtQtMenu::Wt_id()
{
	return QString::fromStdString(Wt::WMenu::id());
}

void WtQtMenu::Wt_setId(QString id)
{
	Wt::WMenu::setId(id.toStdString());
	// also change stacked id
	QString strStackId = id + "_stacked";
	Wt::WMenu::contentsStack()->setId(strStackId.toStdString());
}

QString WtQtMenu::Wt_styleClass()
{
	return QString::fromStdString(Wt::WMenu::styleClass().toUTF8());
}

void WtQtMenu::Wt_setStyleClass(QString styleclass)
{
	Wt::WMenu::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtMenu::Wt_isInline()
{
	return QString("%1").arg(Wt::WMenu::isInline());
}

void WtQtMenu::Wt_setInline(QString isinline)
{
	Wt::WMenu::setInline(isinline.toUInt());
}

Wt::WContainerWidget * WtQtMenu::getWtParent()
{
	return m_wparent;
}

bool WtQtMenu::isBeingDeleted()
{
	return m_beingDeleted;
}



/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*************************************************             WtQtMenuItem             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtMenuItem::WtQtMenuItem(WtQtMenu *qparent) : WtQtContainerWidget(qparent->getWtParent(), qparent)
{
	m_qparent       = qparent;
	m_contentwidget = dynamic_cast<Wt::WContainerWidget*>(this);
	m_menuitem      = m_qparent->addItem(Wt::WString("Label"), m_contentwidget);
}

WtQtMenuItem::WtQtMenuItem(const WtQtMenuItem& other)
{
	Q_UNUSED(other)
}

WtQtMenuItem::~WtQtMenuItem()
{
	if (!m_qparent->isBeingDeleted())
	{
		m_qparent->removeItem(m_menuitem);
	}
}

QString WtQtMenuItem::Wt_className()
{
	return "WMenuItem";
}

QString WtQtMenuItem::Wt_id()
{
	return QString::fromStdString(m_contentwidget->id());
}

void WtQtMenuItem::Wt_setId(QString id)
{
	m_contentwidget->setId(id.toStdString());
}

QString WtQtMenuItem::Wt_styleClass()
{
	return QString::fromStdString(m_contentwidget->styleClass().toUTF8());
}

void WtQtMenuItem::Wt_setStyleClass(QString styleclass)
{
	m_contentwidget->setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtMenuItem::Wt_isInline()
{
	return QString("%1").arg(m_contentwidget->isInline());
}

void WtQtMenuItem::Wt_setInline(QString isinline)
{
	m_contentwidget->setInline(isinline.toUInt());
}

QString WtQtMenuItem::Wt_htmlTagName()
{
	return QString::fromStdString(m_contentwidget->htmlTagName());
}

void WtQtMenuItem::Wt_setHtmlTagName(QString tagname)
{
	//m_contentwidget->setHtmlTagName(tagname.toStdString());
}

QString WtQtMenuItem::Wt_text()
{
	return QString::fromStdString(m_menuitem->text().toUTF8());
}

void WtQtMenuItem::Wt_setText(QString text)
{
	m_menuitem->setText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtMenuItem::Wt_isItemDisabled()
{
	return QString("%1").arg(m_qparent->isItemDisabled(m_menuitem));
}

void WtQtMenuItem::Wt_setItemDisabled(QString disabled)
{
	m_qparent->setItemDisabled(m_menuitem, disabled.toUInt());
}

QString WtQtMenuItem::Wt_isItemHidden()
{
	return QString("%1").arg(m_qparent->isItemHidden(m_menuitem));
}

void WtQtMenuItem::Wt_setItemHidden(QString hidden)
{
	m_qparent->setItemHidden(m_menuitem, hidden.toUInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtWPopupMenu             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtPopupMenu::WtQtPopupMenu(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : WtQtContainerWidget(0, qparent)
{
	m_isRightWMenu = false;
    QString strTemp = qparent->property("Wt_className").toString();
    m_isSubmenu = !strTemp.compare("WPopupMenu");
	WtQtContainerWidget::setHidden(true);
	m_beingDeleted = false;
	m_wparent      = NULL;
	// add wmenu to same menu parent
	if (!wparent)
	{
		wparent = GetWContainerParent(qparent);
	}
	if (wparent)
	{
		m_wparent    = wparent;
		m_wmenu      = new Wt::WPopupMenu();
		// check if parent is navbar
		WtQtNavigationBar * navbar = qobject_cast<WtQtNavigationBar*>(qparent);
		m_rightMenu = NULL;
		if (m_isSubmenu)
		{
			m_pushbutton  = NULL;
			WtQtPopupMenu * parentMenu = qobject_cast<WtQtPopupMenu*>(qparent);
			m_submenuitem = parentMenu->getWPopupMenu()->addMenu("SubMenu", m_wmenu);
		} 
		else if (navbar)
		{
			// Setup a Right-aligned menu.
			m_rightMenu = new Wt::WMenu();
			navbar->addMenu(m_rightMenu, Wt::AlignRight);
			m_submenuitem = new Wt::WMenuItem("Help");
			m_submenuitem->setMenu(m_wmenu);
			m_rightMenu->addItem(m_submenuitem);
			m_wmenu->setAutoHide(true);
			//
			m_wparent = navbar->getWtParent();
			m_isRightWMenu = true;
		}
		else
		{
			m_submenuitem = NULL;
			m_pushbutton  = new Wt::WPushButton(wparent);
			m_pushbutton->setMenu(m_wmenu);
		}
	}
	else
	{
		qDebug() << "[ERROR] Invalid Wt parent in WtQtMenu::WtQtMenu";
		this->deleteLater();
	}
}

WtQtPopupMenu::WtQtPopupMenu(const WtQtPopupMenu& other)
{
	Q_UNUSED(other)
}

WtQtPopupMenu::~WtQtPopupMenu()
{
	if (m_isRightWMenu)
	{
		//delete m_rightMenu; // NOPE ?
		return;
	}
	else if (!m_isSubmenu && !m_isRightWMenu)
	{
		m_beingDeleted = true;
		//delete m_pushbutton; // NOPE EITHER ?
	}
	else
	{
		WtQtPopupMenu * parentMenu = qobject_cast<WtQtPopupMenu*>(QObject::parent());
		if (parentMenu)
		{
			parentMenu->getWPopupMenu()->removeItem(m_submenuitem);
		}
	}
}

QString WtQtPopupMenu::Wt_className()
{
	return "WPopupMenu";
}


QString WtQtPopupMenu::Wt_id()
{
	return QString::fromStdString(m_wmenu->id());
}

void WtQtPopupMenu::Wt_setId(QString id)
{
	m_wmenu->setId(id.toStdString());
	if (m_rightMenu) 
	{
		QString strPopupId = id + "_menu";
		m_rightMenu->setId(strPopupId.toStdString());
		return;
	}
	if (!m_isSubmenu)
	{
		QString strPopupId = id + "_button";
		m_pushbutton->setId(strPopupId.toStdString());
	}
}

QString WtQtPopupMenu::Wt_styleClass()
{
	return QString::fromStdString(m_wmenu->styleClass().toUTF8());
}

void WtQtPopupMenu::Wt_setStyleClass(QString styleclass)
{
	m_wmenu->setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtPopupMenu::Wt_isInline()
{
	if (m_rightMenu) 
	{
		return QString("%1").arg(m_rightMenu->isInline());
	}
	if (!m_isSubmenu)
	{ // ONLY for pushbutton (reimplement for cpp)
		return QString("%1").arg(m_pushbutton->isInline());
	}
	else
	{
		return "";
	}
}

void WtQtPopupMenu::Wt_setInline(QString isinline)
{
	if (m_rightMenu) 
	{
		m_rightMenu->setInline(isinline.toUInt());
		return;
	}
	if (!m_isSubmenu)
	{ // ONLY for pushbutton (reimplement for cpp)
		m_pushbutton->setInline(isinline.toUInt());
	}
}

//QString WtQtPopupMenu::Wt_htmlTagName()
//{
//	if (m_rightMenu)
//	{
//		return "";
//	}
//	if (!m_isSubmenu)
//	{ // ONLY for pushbutton (reimplement for cpp)
//		return QString::fromStdString(m_pushbutton->htmlTagName());
//	}
//	else
//	{
//		return "";
//	}
//}
//
//void WtQtPopupMenu::Wt_setHtmlTagName(QString tagname)
//{
//	if (m_rightMenu)
//	{
//		return;
//	}
//	if (!m_isSubmenu)
//	{ // ONLY for pushbutton (reimplement for cpp)
//		if (tagname.compare("button") != 0) // if we overwritte with button it fucks up the style somehow
//		{
//			m_pushbutton->setHtmlTagName(tagname.toStdString());
//		}
//	}
//	else
//	{
//		return;
//	}
//}

QString WtQtPopupMenu::Wt_text() // ONLY for submenu (reimplement instantiate for cpp)
{
	if (m_rightMenu) 
	{
		return QString::fromStdString(m_submenuitem->text().toUTF8());
	}
	if (!m_isSubmenu) 
	{ 
		return ""; 
	}
	else
	{
		return QString::fromStdString(m_submenuitem->text().toUTF8());
	}

}

void WtQtPopupMenu::Wt_setText(QString text) // ONLY for submenu (reimplement instantiate for cpp)
{
	if (m_rightMenu)
	{
		m_submenuitem->setText(Wt::WString::fromUTF8(text.toStdString()));
	}
	if (!m_isSubmenu) 
	{ 
		return ; 
	}
	else
	{
		m_submenuitem->setText(Wt::WString::fromUTF8(text.toStdString()));
	}

}

bool WtQtPopupMenu::isBeingDeleted()
{
	if (!m_isRightWMenu)
	{
		return m_beingDeleted;
	}
	return true;
}

Wt::WContainerWidget * WtQtPopupMenu::getWtParent()
{
	return m_wparent;
}


Wt::WWidget * WtQtPopupMenu::getInternalWWidget()
{
	if (m_rightMenu)
	{
		return m_rightMenu;
		
	}
	return m_pushbutton;
}

Wt::WPopupMenu * WtQtPopupMenu::getWPopupMenu()
{
	return m_wmenu;
}


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtPopupItem             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtPopupItem::WtQtPopupItem(WtQtPopupMenu *qparent) : WtQtContainerWidget(0, qparent)
{
	Wt::WContainerWidget::setHidden(true);
	m_qparent  = qparent;
	m_menuitem = m_qparent->getWPopupMenu()->addItem(Wt::WString("Label"));
}

WtQtPopupItem::WtQtPopupItem(const WtQtPopupItem& other)
{
	Q_UNUSED(other)
}

WtQtPopupItem::~WtQtPopupItem()
{
	if (!m_qparent->isBeingDeleted())
	{
		m_qparent->getWPopupMenu()->removeItem(m_menuitem);
	}
}

QString WtQtPopupItem::Wt_className()
{
	return "WPopupItem";
}

QString WtQtPopupItem::Wt_id()
{
	return QString::fromStdString(m_menuitem->id());
}

void WtQtPopupItem::Wt_setId(QString id)
{
	m_menuitem->setId(id.toStdString());
}

QString WtQtPopupItem::Wt_styleClass()
{
	return QString::fromStdString(m_menuitem->styleClass().toUTF8());
}

void WtQtPopupItem::Wt_setStyleClass(QString styleclass)
{
	m_menuitem->setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtPopupItem::Wt_isInline()
{
	return QString("%1").arg(m_menuitem->isInline());
}

void WtQtPopupItem::Wt_setInline(QString isinline)
{
	m_menuitem->setInline(isinline.toUInt());
}

QString WtQtPopupItem::Wt_htmlTagName()
{
	return QString::fromStdString(m_menuitem->htmlTagName());
}

void WtQtPopupItem::Wt_setHtmlTagName(QString tagname)
{
	//m_menuitem->setHtmlTagName(tagname.toStdString());
}

QString WtQtPopupItem::Wt_text()
{
	return QString::fromStdString(m_menuitem->text().toUTF8());
}

void WtQtPopupItem::Wt_setText(QString text)
{
	m_menuitem->setText(Wt::WString::fromUTF8(text.toStdString()));
}

QString WtQtPopupItem::Wt_icon()
{
	return QString::fromStdString(m_menuitem->icon());
}

void WtQtPopupItem::Wt_setIcon(QString iconpath)
{
	m_menuitem->setIcon(iconpath.toStdString());
}

QString WtQtPopupItem::Wt_isCheckable()
{
	return QString("%1").arg(m_menuitem->isCheckable());
}

void WtQtPopupItem::Wt_setCheckable(QString checkable)
{
	m_menuitem->setCheckable(checkable.toUInt());
}

QString WtQtPopupItem::Wt_isChecked()
{
	return QString("%1").arg(m_menuitem->isChecked());
}

void WtQtPopupItem::Wt_setChecked(QString checked)
{
	m_menuitem->setChecked(checked.toUInt());
}

QString WtQtPopupItem::Wt_isCloseable()
{
	return QString("%1").arg(m_menuitem->isCloseable());
}

void WtQtPopupItem::Wt_setCloseable(QString closable)
{
	m_menuitem->setCloseable(closable.toUInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*******************************************************             WtQtTable             *****************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTable::WtQtTable(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTable(wparent), WtQtInteractWidget(qparent)
{
	// Just some entries for display
	this->elementAt(0, 1)->addWidget(new Wt::WText("Column 1"));
	this->elementAt(0, 2)->addWidget(new Wt::WText("Column 2"));
	this->elementAt(1, 0)->addWidget(new Wt::WText("Row 1"));
	this->elementAt(2, 0)->addWidget(new Wt::WText("Row 1"));
}

WtQtTable::WtQtTable(const WtQtTable& other)
{
	Q_UNUSED(other)
}

WtQtTable::~WtQtTable()
{

}

QString WtQtTable::Wt_className()
{
	return "WTable";
}

QString WtQtTable::Wt_id()
{
	return QString::fromStdString(Wt::WTable::id());
}

void WtQtTable::Wt_setId(QString id)
{
	Wt::WTable::setId(id.toStdString());
}

QString WtQtTable::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTable::styleClass().toUTF8());
}

void WtQtTable::Wt_setStyleClass(QString styleclass)
{
	Wt::WTable::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTable::Wt_isInline()
{
	return QString("%1").arg(Wt::WTable::isInline());
}

void WtQtTable::Wt_setInline(QString isinline)
{
	Wt::WTable::setInline(isinline.toUInt());
}

//QString WtQtTable::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WTable::htmlTagName());
//}
//
//void WtQtTable::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WTable::setHtmlTagName(tagname.toStdString());
//}

QString WtQtTable::Wt_headerCountHorizontal()
{
	return QString("%1").arg(Wt::WTable::headerCount(Wt::Orientation::Horizontal));
}

void WtQtTable::Wt_setHeaderCountHorizontal(QString headcount)
{
	Wt::WTable::setHeaderCount(headcount.toInt(), Wt::Orientation::Horizontal);
}

QString WtQtTable::Wt_headerCountVertical()
{
	return QString("%1").arg(Wt::WTable::headerCount(Wt::Orientation::Vertical));
}

void WtQtTable::Wt_setHeaderCountVertical(QString headcount)
{
	Wt::WTable::setHeaderCount(headcount.toInt(), Wt::Orientation::Vertical);
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
********************************************************             WtQtTree             *****************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTree::WtQtTree(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTree(wparent), WtQtCompositeWidget(qparent)
{
	Wt::WTreeNode *node = new Wt::WTreeNode("Furniture");

	this->setTreeRoot(node);
	node->label()->setTextFormat(Wt::PlainText);
	node->setLoadPolicy(Wt::WTreeNode::NextLevelLoading);
	node->addChildNode(new Wt::WTreeNode("Table"));
	node->addChildNode(new Wt::WTreeNode("Cupboard"));

	Wt::WTreeNode *three = new Wt::WTreeNode("Chair");
	node->addChildNode(three);
	node->addChildNode(new Wt::WTreeNode("Coach"));
	node->expand();

	three->addChildNode(new Wt::WTreeNode("Doc"));
	three->addChildNode(new Wt::WTreeNode("Grumpy"));
	three->addChildNode(new Wt::WTreeNode("Happy"));
	three->addChildNode(new Wt::WTreeNode("Sneezy"));
	three->addChildNode(new Wt::WTreeNode("Dopey"));
	three->addChildNode(new Wt::WTreeNode("Bashful"));
	three->addChildNode(new Wt::WTreeNode("Sleepy"));
}

WtQtTree::WtQtTree(const WtQtTree& other)
{
	Q_UNUSED(other)
}

WtQtTree::~WtQtTree()
{
	
}

QString WtQtTree::Wt_className()
{
	return "WTree";
}

QString WtQtTree::Wt_id()
{
	return QString::fromStdString(Wt::WTree::id());
}

void WtQtTree::Wt_setId(QString id)
{
	Wt::WTree::setId(id.toStdString());
}

QString WtQtTree::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTree::styleClass().toUTF8());
}

void WtQtTree::Wt_setStyleClass(QString styleclass)
{
	Wt::WTree::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTree::Wt_isInline()
{
	return QString("%1").arg(Wt::WTree::isInline());
}

void WtQtTree::Wt_setInline(QString isinline)
{
	Wt::WTree::setInline(isinline.toUInt());
}

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtTreeTable             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/

WtQtTreeTable::WtQtTreeTable(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/) : Wt::WTreeTable(wparent), WtQtCompositeWidget(qparent)
{
	//resize(650, 200);
	tree()->setSelectionMode(Wt::ExtendedSelection);
	addColumn("Yuppie Factor", 125);
	addColumn("# Holidays", 125);
	addColumn("Favorite Item", 125);

	Wt::WTreeTableNode *root = new Wt::WTreeTableNode("All Personnel");
	setTreeRoot(root, "Emweb Organigram");

	Wt::WTreeTableNode *group;

	group = new Wt::WTreeTableNode("Upper Management", 0, root);
	addNode(group, "Chief Anything Officer", "-2.8", "20", "Scepter");
	addNode(group, "Vice President of Parties", "13.57", "365", "Flag");
	addNode(group, "Vice President of Staplery", "3.42", "27", "Perforator");

	group = new Wt::WTreeTableNode("Middle management", 0, root);
	addNode(group, "Boss of the house", "9.78", "35", "Happy Animals");
	addNode(group, "Xena caretaker", "8.66", "10", "Yellow bag");

	group = new Wt::WTreeTableNode("Actual Workforce", 0, root);
	addNode(group, "The Dork", "9.78", "22", "Mojito");
	addNode(group, "The Stud", "8.66", "46", "Toothbrush");
	addNode(group, "The Ugly", "13.0", "25", "Paper bag");

	root->expand();
}

WtQtTreeTable::WtQtTreeTable(const WtQtTreeTable& other)
{
	Q_UNUSED(other)
}

WtQtTreeTable::~WtQtTreeTable()
{
	
}

QString WtQtTreeTable::Wt_className()
{
	return "WTreeTable";
}

QString WtQtTreeTable::Wt_id()
{
	return QString::fromStdString(Wt::WTreeTable::id());
}

void WtQtTreeTable::Wt_setId(QString id)
{
	Wt::WTreeTable::setId(id.toStdString());
}

QString WtQtTreeTable::Wt_styleClass()
{
	return QString::fromStdString(Wt::WTreeTable::styleClass().toUTF8());
}

void WtQtTreeTable::Wt_setStyleClass(QString styleclass)
{
	Wt::WTreeTable::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtTreeTable::Wt_isInline()
{
	return QString("%1").arg(Wt::WTreeTable::isInline());
}

void WtQtTreeTable::Wt_setInline(QString isinline)
{
	Wt::WTreeTable::setInline(isinline.toUInt());
}

Wt::WTreeTableNode * WtQtTreeTable::addNode(Wt::WTreeTableNode *parent, const char *name, const char *yuppie, const char *holidays, const char *favorite)
{
	Wt::WTreeTableNode *node = new Wt::WTreeTableNode(name, 0, parent);
	node->setColumnWidget(1, new Wt::WText(yuppie));
	node->setColumnWidget(2, new Wt::WText(holidays));
	node->setColumnWidget(3, new Wt::WText(favorite));
	return node;
}



/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*****************************************************             WtQtNavigationBar             ***************************************************************
-----------------------------------------------------------------------------------------------------------------------------------------------------------
*/



WtQtNavigationBar::WtQtNavigationBar(Wt::WContainerWidget *wparent /*= 0*/, QObject *qparent /*= 0*/)
	: Wt::WNavigationBar(wparent), WtQtInteractWidget(qparent)
{
	m_wparent = NULL;
	if (wparent)
	{
		m_wparent = wparent;
	}
	else
	{
		m_wparent = GetWContainerParent(qparent);
	}
	m_strParentId = qparent->property("Wt_id").toString();
	// init as not responsive
	m_strResponsive = "0";
}

WtQtNavigationBar::WtQtNavigationBar(const WtQtNavigationBar& other)
{
	Q_UNUSED(other)
}

WtQtNavigationBar::~WtQtNavigationBar()
{

}


QString WtQtNavigationBar::Wt_className()
{
	return "WNavigationBar";
}

QString WtQtNavigationBar::Wt_id()
{
	return QString::fromStdString(Wt::WNavigationBar::id());
}

void WtQtNavigationBar::Wt_setId(QString id)
{
	Wt::WNavigationBar::setId(id.toStdString());
}

QString WtQtNavigationBar::Wt_styleClass()
{
	return QString::fromStdString(Wt::WNavigationBar::styleClass().toUTF8());
}

void WtQtNavigationBar::Wt_setStyleClass(QString styleclass)
{
	Wt::WNavigationBar::setStyleClass(Wt::WString::fromUTF8(styleclass.toStdString()));
}

QString WtQtNavigationBar::Wt_isInline()
{
	return QString("%1").arg(Wt::WNavigationBar::isInline());
}

void WtQtNavigationBar::Wt_setInline(QString isinline)
{
	Wt::WNavigationBar::setInline(isinline.toUInt());
}

//QString WtQtNavigationBar::Wt_htmlTagName()
//{
//	return QString::fromStdString(Wt::WNavigationBar::htmlTagName());
//}
//
//void WtQtNavigationBar::Wt_setHtmlTagName(QString tagname)
//{
//	Wt::WNavigationBar::setHtmlTagName(tagname.toStdString());
//}

QString WtQtNavigationBar::Wt_title()
{
	return m_strTitle;
}

void WtQtNavigationBar::Wt_setTitle(QString text)
{
	Wt::WNavigationBar::setTitle(text.toStdString());
	m_strTitle = text;
}

QString WtQtNavigationBar::Wt_responsive()
{
	return m_strResponsive;
}

void WtQtNavigationBar::Wt_setResponsive(QString isresponsive)
{
	Wt::WNavigationBar::setResponsive(isresponsive.toUInt());
	m_strResponsive = isresponsive;
}

Wt::WContainerWidget * WtQtNavigationBar::getWtParent()
{
	return m_wparent;
}

QString WtQtNavigationBar::getParentWidgetId()
{
	return m_strParentId;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////   CPP    /////////////////   CPP    //////////////////   CPP    /////////////////////   CPP    //////////////////////   CPP    ///////////////////   CPP    ///////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QString WtQtWidget::Cpp_declare()
{
	return "Wt::" + Wt_className() + " *" + Wt_id() + ";";
}

QString WtQtWidget::Cpp_instantiate()
{

	return Wt_id() + " = new Wt::" + Wt_className() + "(" + Cpp_getCorrectParentId() + ");\n";
}

QString WtQtWidget::Cpp_styleClass()
{
	return Wt_id() + "->" + "setStyleClass(Wt::WString::fromUTF8(\"" + Wt_styleClass() + "\"));";
}

QString WtQtWidget::Cpp_isInline()
{
	return Wt_id() + "->" + "setInline(" + Wt_isInline() + ");";
}

QString WtQtWidget::Cpp_getCorrectParentId()
{
	WtQtWidget * wtqtparent = qobject_cast<WtQtWidget*>(QObject::parent());
	if (!wtqtparent) { qDebug() << "[ERROR] Could not cast parent in WtQtWidget::Cpp_instantiate"; return ""; }

	if (wtqtparent->Wt_className().compare("WPanel", Qt::CaseInsensitive) == 0)
	{
		return qobject_cast<WtQtPanel*>(wtqtparent)->getCentralWidgetId();
	}
	else if (wtqtparent->Wt_className().compare("WNavigationBar", Qt::CaseInsensitive) == 0)
	{
		return qobject_cast<WtQtNavigationBar*>(wtqtparent)->getParentWidgetId();
	}

	return wtqtparent->Wt_id();
}

QString WtQtWidget::Cpp_id()
{
	return Wt_id() + "->" + "setId(\"" + Wt_id() + "\");";
}

QString WtQtLineEdit::Cpp_instantiate()
{
	QString strRetVal = WtQtWidget::Cpp_instantiate();
	if (m_boolIsSearch)
	{ //navbar->addSearch(this, Wt::AlignRight);
		strRetVal += Cpp_internalTabs + QObject::parent()->property("Wt_id").toString() // navbar id
			      + "->addSearch(" + Wt_id() + ", Wt::AlignRight);\n";
	}
	return strRetVal;
}

//QString WtQtWebWidget::Cpp_htmlTagName()
//{
//	return Wt_id() + "->" + "setHtmlTagName(\"" + Wt_htmlTagName() + "\");";
//}

QString WtQtContainerWidget::Cpp_htmlTagName()
{
	return Wt_id() + "->" + "setHtmlTagName(\"" + Wt_htmlTagName() + "\");";
}

QString WtQtAnchor::Cpp_link()
{
	return Wt_id() + "->" + "setLink(Wt::WLink(\"" + Wt_link() + "\"));";
}

QString WtQtAnchor::Cpp_target()
{
	return Wt_id() + "->" + "setTarget(Wt::" + Wt_target() + ");";
}

QString WtQtAnchor::Cpp_text()
{
	return Wt_id() + "->" + "setTextFormat(Wt::PlainText); " + Wt_id() + "->" + "setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}



//QString WtQtAnchor::Cpp_htmlTagName()
//{
//	return "";
//}



QString WtQtText::Cpp_text()
{
	QString htext = Wt_text();
	htext.replace("&lt;", "<");
	htext.replace("&gt;", ">");
	htext.replace("\"", "\\\"");
	htext.replace("\n", "\\n\\\n");
	//
	return Wt_id() + "->" + "setText(Wt::WString::fromUTF8(\"" + htext + "\"));";
}

QString WtQtText::Cpp_textFormat()
{
	return Wt_id() + "->" + "setTextFormat((Wt::TextFormat)" + Wt_textFormat() + ");";
}


QString WtQtAnchor::Cpp_textFormat()
{
	return Wt_id() + "->" + "setTextFormat((Wt::TextFormat)" + Wt_textFormat() + ");";
}



QString WtQtPushButton::Cpp_text()
{
	QString strTemp = Wt_text();
	// [FIX] Here they go out corretcly
	return Wt_id() + "->" + "setText(Wt::WString::fromUTF8(\"" + strTemp.toUtf8() + "\"));";

	//return Wt_id() + "->" + "setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}

QString WtQtPushButton::Cpp_link()
{
	return Wt_id() + "->" + "setLink(Wt::WLink(\"" + Wt_link() + "\"));";
}



QString WtQtImage::Cpp_imageLink()
{
	return Wt_id() + "->" + "setImageLink(Wt::WLink(\"" + Wt_imageLink() + "\"));";
}



QString WtQtTemplate::Cpp_templateText()
{
	QString htext = Wt_templateText();
	htext.replace("&lt;", "<");
	htext.replace("&gt;", ">");
	htext.replace("\"", "\\\"");
	htext.replace("\n", "\\n\\\n");
	return Wt_id() + "->" + "setTemplateText(Wt::WString::fromUTF8(\"" + htext + "\"), (Wt::TextFormat)" + Wt_textFormat() + ");";
}


QString WtQtFormWidget::Cpp_emptyText()
{
	return Wt_id() + "->" + "setEmptyText(Wt::WString::fromUTF8(\"" + Wt_emptyText() + "\"));";
}


QString WtQtSplitButton::Cpp_text()
{
	return Wt_id() + "->actionButton()->setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}

QString WtQtSplitButton::Cpp_allItems()
{
	QStringList strListItems = Wt_allItems().split("\\n");
	if (strListItems.count() <= 0) { return ""; }
	// create popup menu
	QString strPopName = "p_" + Wt_id() + "_popup";
	QString strRetVal = "Wt::WPopupMenu *" + strPopName + " = new Wt::WPopupMenu(); \n";
	// add all items
	for (int i = 0; i < strListItems.count(); i++)
	{
		if (strListItems.at(i).isEmpty()) { continue; }
		strRetVal += Cpp_internalTabs + strPopName + "->addItem(Wt::WString::fromUTF8(\"" + strListItems.at(i) + +"\"));";
		if (i != strListItems.count() - 1)
		{
			strRetVal += " \n";
		}
	}
	// set popup menu
	strRetVal += Cpp_internalTabs + Wt_id() + "->dropDownButton()->setMenu(" + strPopName + ");";
	return strRetVal;
}


QString WtQtAbstractToggleButton::Cpp_text()
{
	return Wt_id() + "->setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}


QString WtQtRadioButton::Cpp_instantiate()
{
	return Wt_id() + " = new Wt::" + Wt_className() + "(\"" + Wt_text() + "\", " + Cpp_getCorrectParentId() + ");\n";
}

QString WtQtRadioButton::Cpp_setGroup()
{
	QString strRetVal  = "";
	QString strGroupName;
	// create group name based on parent
	strGroupName = Cpp_getCorrectParentId() + "_btngrp";
	// create group if first to print
	if (isFirstToPrint())
	{
		strRetVal += "Wt::WButtonGroup *" + strGroupName + " = new Wt::WButtonGroup(" + Cpp_getCorrectParentId() + ");\n" + Cpp_internalTabs; // TODO : fix tabs
	}
	// add to group
	strRetVal += strGroupName + "->addButton(" + Wt_id() + ", -1);";
	// if last to print reset all print states
	m_printed = true;
	if (isLastToPrint())
	{
		clearAllPrinted();
	}
	return strRetVal;
}

QString WtQtRadioButton::Cpp_text()
{
	return ""; // Return empty
}




QString WtQtComboBox::Cpp_allItems()
{
	QString strRetVal = "";
	QStringList strListItems = Wt_allItems().split("\\n");
	if (strListItems.count() <= 0) { return ""; }
	// add all items
	for (int i = 0; i < strListItems.count(); i++)
	{
		if (strListItems.at(i).isEmpty()) { continue; }
		strRetVal += Wt_id() + "->addItem(Wt::WString::fromUTF8(\"" + strListItems.at(i) + +"\"));";
		if (i != strListItems.count() - 1)
		{
			strRetVal += " \n" + Cpp_internalTabs;
		}
	}
	return strRetVal;
}



QString WtQtInPlaceEdit::Cpp_text()
{
	return Wt_id() + "->setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}

QString WtQtInPlaceEdit::Cpp_placeholderText()
{
	return Wt_id() + "->setPlaceholderText(Wt::WString::fromUTF8(\"" + Wt_placeholderText() + "\"));";
}

QString WtQtInPlaceEdit::Cpp_buttonsEnabled()
{
	return Wt_id() + "->" + "setButtonsEnabled(" + Wt_buttonsEnabled() + ");";
}




QString WtQtTextArea::Cpp_text()
{
	return Wt_id() + "->setText(Wt::WString::fromUTF8(\"" + Wt_text() + "\"));";
}

QString WtQtTextArea::Cpp_columns()
{
	return Wt_id() + "->setColumns(" + Wt_columns() + ");";
}

QString WtQtTextArea::Cpp_rows()
{
	return Wt_id() + "->setRows(" + Wt_rows() + ");";
}



QString WtQtSelectionBox::Cpp_allItems()
{
	QString strRetVal = "";
	QStringList strListItems = WtQtSelectionBox::Wt_allItems().split("\\n");
	if (strListItems.count() <= 0) { return ""; }
	// add all items
	for (int i = 0; i < strListItems.count(); i++)
	{
		if (strListItems.at(i).isEmpty()) { continue; }
		strRetVal += Wt_id() + "->addItem(Wt::WString::fromUTF8(\"" + strListItems.at(i) + +"\"));";
		if (i != strListItems.count() - 1)
		{
			strRetVal += " \n" + Cpp_internalTabs;
		}
	}
	return strRetVal;
}



QString WtQtSpinBox::Cpp_minimum()
{
	return Wt_id() + "->setMinimum(" + Wt_minimum() + ");";
}

QString WtQtSpinBox::Cpp_maximum()
{
	return Wt_id() + "->setMaximum(" + Wt_maximum() + ");";
}

QString WtQtSpinBox::Cpp_value()
{
	return Wt_id() + "->setValue(" + Wt_value() + ");";
}



QString WtQtDoubleSpinBox::Cpp_minimum()
{
	return Wt_id() + "->setMinimum(" + Wt_minimum() + ");";
}

QString WtQtDoubleSpinBox::Cpp_maximum()
{
	return Wt_id() + "->setMaximum(" + Wt_maximum() + ");";
}

QString WtQtDoubleSpinBox::Cpp_value()
{
	return Wt_id() + "->setValue(" + Wt_value() + ");";
}

QString WtQtDoubleSpinBox::Cpp_singleStep()
{
	return Wt_id() + "->setSingleStep(" + Wt_singleStep() + ");";
}

QString WtQtDoubleSpinBox::Cpp_decimals()
{
	return Wt_id() + "->setDecimals(" + Wt_decimals() + ");";
}



QString WtQtTimeEdit::Cpp_minuteStep()
{
	return Wt_id() + "->setMinuteStep(" + Wt_minuteStep() + ");";
}

QString WtQtTimeEdit::Cpp_format()
{
	return Wt_id() + "->setFormat(\"" + Wt_format() + "\");";
}



QString WtQtDateEdit::Cpp_format()
{
	return Wt_id() + "->setFormat(\"" + Wt_format() + "\");";
}


QString WtQtSlider::Cpp_minimum()
{
	return Wt_id() + "->setMinimum(" + Wt_minimum() + ");";
}

QString WtQtSlider::Cpp_maximum()
{
	return Wt_id() + "->setMaximum(" + Wt_maximum() + ");";
}

QString WtQtSlider::Cpp_value()
{
	return Wt_id() + "->setValue(" + Wt_value() + ");";
}

QString WtQtSlider::Cpp_tickInterval()
{
	return Wt_id() + "->setTickInterval(" + Wt_tickInterval() + ");";
}



QString WtQtProgressBar::Cpp_minimum()
{
	return Wt_id() + "->setMinimum(" + Wt_minimum() + ");";
}

QString WtQtProgressBar::Cpp_maximum()
{
	return Wt_id() + "->setMaximum(" + Wt_maximum() + ");";
}

QString WtQtProgressBar::Cpp_value()
{
	return Wt_id() + "->setValue(" + Wt_value() + ");";
}



QString WtQtGroupBox::Cpp_title()
{
	return Wt_id() + "->setTitle(" + Wt_title() + ");";
}

QString WtQtGroupBox::Cpp_htmlTagName()
{
	return "";
}


QString WtQtPanel::Cpp_title()
{
	return Wt_id() + "->setTitle(" + Wt_title() + ");";
}

QString WtQtPanel::Cpp_isCollapsible()
{
	return Wt_id() + "->" + "setCollapsible(" + Wt_isCollapsible() + ");";
}

QString WtQtPanel::Cpp_instantiate()
{
	QString strRetVal = "";
	
	strRetVal += "Wt::WContainerWidget *cw_" + Wt_id() + " = new Wt::WContainerWidget(" + Cpp_getCorrectParentId() + ");\n" + Cpp_internalTabs;
	strRetVal += Wt_id() + " = new Wt::" + Wt_className() + "(" + Cpp_getCorrectParentId() + ");\n";
	return strRetVal;
}
QString WtQtPanel::Cpp_centralWidget()
{ 
	return Wt_id() + "->" + "setCentralWidget(" + getCentralWidgetId() + ");";
}

QString WtQtStackedWidget::Cpp_currentIndex()
{
	return Wt_id() + "->setCurrentIndex(" + Wt_currentIndex() + ");";
}

QString WtQtStackedWidget::Cpp_htmlTagName()
{
	return "";
}





QString WtQtTabItem::Cpp_declare()
{
	QString strRetVal = "";
	strRetVal += "Wt::WContainerWidget * " + Wt_id() + ";\n\t" ;
	strRetVal += "Wt::WMenuItem * " + Wt_id() + "_mi;";
	return strRetVal;
}

QString WtQtTabItem::Cpp_instantiate()
{
	QString strRetVal = "";
	strRetVal += Wt_id() + " = new Wt::WContainerWidget(" + m_qparent->Cpp_getCorrectParentId() + ");\n" + Cpp_internalTabs;
	strRetVal += Wt_id() + "_mi = " + m_qparent->Wt_id() + "->addTab(" + Wt_id() + ", Wt::WString(\"" + Wt_tabText() + "\"));\n";
	return strRetVal;
}

QString WtQtTabItem::Cpp_isTabEnabled()
{
	return m_qparent->Wt_id()
		+ "->setTabEnabled("
		+ QString("%1").arg(m_qparent->indexOf(m_childWidget))
		+ ", " + Wt_isTabEnabled() + ");";
}

QString WtQtTabItem::Cpp_isTabHidden()
{
	return m_qparent->Wt_id()
		+ "->setTabHidden("
		+ QString("%1").arg(m_qparent->indexOf(m_childWidget))
		+ ", " + Wt_isTabHidden() + ");";
}

QString WtQtTabItem::Cpp_isTabCloseable()
{
	return m_qparent->Wt_id()
		+ "->setTabCloseable("
		+ QString("%1").arg(m_qparent->indexOf(m_childWidget))
		+ ", " + Wt_isTabCloseable() + ");";
}

QString WtQtTabItem::Cpp_htmlTagName()
{
	return "";
}

QString WtQtMenu::Cpp_instantiate()
{
	QString strRetVal = "";
	strRetVal += Wt_id() + " = new Wt::WMenu(new Wt::WStackedWidget(), " + Cpp_getCorrectParentId() + ");\n" + Cpp_internalTabs;
	strRetVal += Cpp_getCorrectParentId() + "->addWidget(" + Wt_id() + "->contentsStack());\n" + Cpp_internalTabs;
	strRetVal += Wt_id() +"->contentsStack()->setId(\"" + Wt_id() + "_stackedwidget" + "\");\n";
	if (m_isLeftWMenu)
	{ // navbar->addMenu(this, Wt::AlignLeft);
		strRetVal += Cpp_internalTabs + QObject::parent()->property("Wt_id").toString() // navbar id
			      + "->addMenu(" + Wt_id() + ", Wt::AlignLeft);\n";
	}
	return strRetVal;
}

QString WtQtMenuItem::Cpp_declare()
{
	QString strRetVal = "";
	strRetVal += "Wt::WContainerWidget * " + Wt_id() + ";\n\t";
	strRetVal += "Wt::WMenuItem * " + Wt_id() + "_mi;";
	return strRetVal;
}

QString WtQtMenuItem::Cpp_instantiate()
{
	QString strRetVal = "";
	strRetVal += Wt_id() + " = new Wt::WContainerWidget(" + m_qparent->Cpp_getCorrectParentId() + ");\n" + Cpp_internalTabs;
	strRetVal += Wt_id() + "_mi = " + m_qparent->Wt_id() + "->addItem(Wt::WString(\"" + Wt_text() + "\"), " + Wt_id() + ");\n";
	return strRetVal;
}

QString WtQtMenuItem::Cpp_isItemDisabled()
{
	return m_qparent->Wt_id()
		+ "->setItemDisabled("
		+ QString("%1").arg(m_qparent->indexOf(m_menuitem))
		+ ", " + Wt_isItemDisabled() + ");";
}

QString WtQtMenuItem::Cpp_isItemHidden()
{
	return m_qparent->Wt_id()
		+ "->setItemHidden("
		+ QString("%1").arg(m_qparent->indexOf(m_menuitem))
		+ ", " + Wt_isItemHidden() + ");";
}

QString WtQtMenuItem::Cpp_htmlTagName()
{
	return "";
}

QString WtQtPopupMenu::Cpp_declare()
{
	QString strRetVal = "";
	strRetVal += "Wt::WPopupMenu * " + Wt_id() + ";";
	if (m_isSubmenu)
	{
		// nothing to do
	}
	else if (m_isRightWMenu)
	{
		strRetVal += "\n\tWt::WMenu * " + Wt_id() + "_rightmenu;";
		strRetVal += "\n\tWt::WMenuItem * " + Wt_id() + "_rightmenuitem;";
	}
	else
	{
		strRetVal += "\n\tWt::WPushButton * " + Wt_id() + "_button;";
	}
	return strRetVal;
}


QString WtQtPopupMenu::Cpp_instantiate()
{
	QString strRetVal = "";
	strRetVal += Wt_id() + " = new Wt::WPopupMenu();\n";
	if (m_isSubmenu)
	{
		WtQtPopupMenu * parentMenu = qobject_cast<WtQtPopupMenu*>(QObject::parent());
		strRetVal += Cpp_internalTabs + parentMenu->Wt_id() + "->addMenu(\"" + Wt_text() + "\", " + Wt_id() + ");\n";
	}
	else if (m_isRightWMenu)
	{// Setup a Right-aligned menu.
		//m_rightMenu = new Wt::WMenu();
		//navbar->addMenu(m_rightMenu, Wt::AlignRight);
		strRetVal += Cpp_internalTabs + Wt_id() + "_rightmenu = new Wt::WMenu();\n";
		strRetVal += Cpp_internalTabs + QObject::parent()->property("Wt_id").toString() // navbar id
			      + "->addMenu(" + Wt_id() + "_rightmenu, Wt::AlignRight);\n";
		//Wt::WMenuItem *item = new Wt::WMenuItem("Help");
		//item->setMenu(m_wmenu);
		strRetVal += Cpp_internalTabs + Wt_id() + "_rightmenuitem = new Wt::WMenuItem(\"" + Wt_text() + "\");\n";
		strRetVal += Cpp_internalTabs + Wt_id() + "_rightmenuitem->setMenu(" + Wt_id() + ");\n";
		//m_rightMenu->addItem(item);
		//m_wmenu->setAutoHide(true);
		strRetVal += Cpp_internalTabs + Wt_id() + "_rightmenu->addItem(" + Wt_id() + "_rightmenuitem);\n";
		strRetVal += Cpp_internalTabs + Wt_id() + "->setAutoHide(true);\n";
	}
	else
	{
		strRetVal += Cpp_internalTabs + Wt_id() + "_button = new Wt::WPushButton(" + Cpp_getCorrectParentId() + ");\n";
		strRetVal += Cpp_internalTabs + Wt_id() + "_button->setMenu(" + Wt_id() + ");\n";
	}
	return strRetVal;
}

QString WtQtPopupMenu::Cpp_isInline()
{
	if (m_rightMenu)
	{
		return Wt_id() + "_rightmenu->setInline(" + Wt_isInline() + ");";
	}
	if (!m_isSubmenu)
	{
		return Wt_id() + "_button->setInline(" + Wt_isInline() + ");";
	}
	else
	{
		return "";
	}
}

QString WtQtPopupMenu::Cpp_htmlTagName()
{
	if (!m_isSubmenu && Wt_htmlTagName().compare("button") != 0 && !m_isRightWMenu)
	{ // ONLY for pushbutton (reimplement for cpp)
		return Wt_id() + "_button->setHtmlTagName(\"" + Wt_htmlTagName() + "\");";
	}
	else
	{
		return "";
	}
}



QString WtQtPopupItem::Cpp_declare()
{
	return "Wt::WMenuItem * " + Wt_id() + ";";
}

QString WtQtPopupItem::Cpp_instantiate()
{
	QString strRetVal = "";
	strRetVal += Wt_id() + " = " + m_qparent->Wt_id() + "->addItem(Wt::WString(\"" + Wt_text() + "\"));\n";
	return strRetVal;
}

QString WtQtPopupItem::Cpp_icon()
{
	return Wt_id() + "->setIcon(\"" + Wt_icon() + "\");";
}

QString WtQtPopupItem::Cpp_isCheckable()
{
	return Wt_id() + "->setCheckable(" + Wt_isCheckable() + ");";
}

QString WtQtPopupItem::Cpp_isChecked()
{
	return Wt_id() + "->setChecked(" + Wt_isChecked() + ");";
}

QString WtQtPopupItem::Cpp_isCloseable()
{
	return Wt_id() + "->setCloseable(" + Wt_isCloseable() + ");";
}

QString WtQtPopupItem::Cpp_htmlTagName()
{
	return "";
}



QString WtQtTable::Cpp_headerCountHorizontal()
{
	return Wt_id() + "->setHeaderCount(" + Wt_headerCountHorizontal() + ", Wt::Orientation::Horizontal);";
}

QString WtQtTable::Cpp_headerCountVertical()
{
	return Wt_id() + "->setHeaderCount(" + Wt_headerCountVertical() + ", Wt::Orientation::Vertical);";
}


QString WtQtNavigationBar::Cpp_title()
{
	return Wt_id() + "->setTitle(\"" + Wt_title() +  + "\");";
}

QString WtQtNavigationBar::Cpp_responsive()
{
	return Wt_id() + "->setResponsive(\"" + Wt_responsive() +  + "\");";
}
