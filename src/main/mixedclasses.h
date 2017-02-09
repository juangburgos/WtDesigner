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

#ifndef MIXEDCLASSES_H
#define MIXEDCLASSES_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <QVariant>

#include <Wt/Utils>

#include <Wt/WObject>
#include <Wt/WWidget>
#include <Wt/WWebWidget>
#include <Wt/WInteractWidget>
#include <Wt/WFormWidget>
#include <Wt/WCompositeWidget>
#include <Wt/WAbstractToggleButton>

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WFileResource>
#include <Wt/WTemplate>
#include <Wt/WString>
#include <Wt/WSplitButton>
#include <Wt/WPopupMenu>
#include <Wt/WRadioButton>
#include <Wt/WLabel>
#include <Wt/WButtonGroup>
#include <Wt/WCheckBox>
#include <Wt/WComboBox>
#include <Wt/WInPlaceEdit>
#include <Wt/WTextArea>
#include <Wt/WTextEdit>
#include <Wt/WSelectionBox>
#include <Wt/WSpinBox>
#include <Wt/WDoubleSpinBox>
#include <Wt/WTimeEdit>
#include <Wt/WDateEdit>
#include <Wt/WCalendar>
#include <Wt/WSlider>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WGroupBox>
#include <Wt/WPanel>
#include <Wt/WStackedWidget>
#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WTable>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WTreeTable>
#include <Wt/WTreeTableNode>
#include <Wt/WNavigationBar>

struct Wt_ConnectionConfig
{
    QString m_strsender    ;
    QString m_strsignal    ;
    QString m_strreceiver  ;
    QString m_strslot      ;
    QString m_strparameter ;
};

// ------------------------------------------------- Base Classes ------------------------------------------------------------------------------------------------------------

class WtQtObject : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString     Wt_className         READ Wt_className   )
		// Cpp code generation
		Q_PROPERTY(QString     Cpp_declare          READ Cpp_declare    )
		Q_PROPERTY(QString     Cpp_instantiate      READ Cpp_instantiate)
public:
	WtQtObject(QObject *qparent = 0) : QObject(qparent) {  }

	virtual QString     Wt_className() = 0;

	// Cpp code generation
	virtual QString Cpp_declare()     = 0;
	virtual QString Cpp_instantiate() = 0; // virtual to deal with root

	static  QString Cpp_internalTabs;
};


class WtQtWidget : public WtQtObject
{
	Q_OBJECT
		// Inherited Wt::WWidget
		Q_PROPERTY(QString Wt_id            READ Wt_id             WRITE Wt_setId        )
		Q_PROPERTY(QString Wt_styleClass    READ Wt_styleClass     WRITE Wt_setStyleClass)
		Q_PROPERTY(QString Wt_isInline      READ Wt_isInline       WRITE Wt_setInline    )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_id           READ Cpp_id         )
		Q_PROPERTY(QString Cpp_styleClass   READ Cpp_styleClass )
		Q_PROPERTY(QString Cpp_isInline     READ Cpp_isInline   )

public:
	WtQtWidget(QObject *qparent = 0) : WtQtObject(qparent) {  }

	virtual QString Wt_id() = 0;
	virtual void    Wt_setId(QString id) = 0;

	virtual QString Wt_styleClass() = 0;
	virtual void    Wt_setStyleClass(QString styleclass) = 0;

	virtual QString Wt_isInline() = 0;
	virtual void    Wt_setInline(QString isinline) = 0;

	// Cpp code generation
	QString Cpp_id()         ;
	QString Cpp_declare()    ;
	QString Cpp_instantiate(); 

	virtual QString Cpp_styleClass() ;
	virtual QString Cpp_isInline()   ;
	// NOT Q_PROPERTY
	QString Cpp_getCorrectParentId();

	enum Wt_Slots_
	{
		Wt_Slots_invalid,
		Wt_Slots_setStyleClass,
		Wt_Slots_addStyleClass,
		Wt_Slots_removeStyleClass,
		Wt_Slots_toggleStyleClass
	};
	Q_ENUM(Wt_Slots_)

	static Wt_Slots_ GetSlotEnumByName(QString name);

};


class WtQtWebWidget : public WtQtWidget
{
	Q_OBJECT
		// Inherited Wt::WWebWidget
		//Q_PROPERTY(QString Wt_htmlTagName   READ Wt_htmlTagName    WRITE Wt_setHtmlTagName)
		// Cpp code generation
		//Q_PROPERTY(QString Cpp_htmlTagName     READ Cpp_htmlTagName)
public:
	WtQtWebWidget(QObject *qparent = 0) : WtQtWidget(qparent) {  }

	//virtual QString Wt_htmlTagName() = 0;
	//virtual void    Wt_setHtmlTagName(QString tagname) = 0;

	//// Cpp code generation
	//virtual QString Cpp_htmlTagName();

};

class WtQtCompositeWidget : public WtQtWidget
{
	Q_OBJECT
public:
	WtQtCompositeWidget(QObject *qparent = 0) : WtQtWidget(qparent) {  }

};

class WtQtInteractWidget : public WtQtWebWidget
{
	Q_OBJECT
public:
	WtQtInteractWidget(QObject *qparent = 0) : WtQtWebWidget(qparent) {  }

	enum Wt_Signals_ 
	{ 
		Wt_Signals_invalid,
		Wt_Signals_clicked,
		Wt_Signals_doubleClicked,
		Wt_Signals_mouseWentDown,
		Wt_Signals_mouseWentUp,
		Wt_Signals_mouseWentOut,
		Wt_Signals_mouseWentOver,
		Wt_Signals_mouseMoved
	};
	Q_ENUM(Wt_Signals_)

	Wt::Signals::connection Wt_Connect_Step1(Wt::WInteractWidget *wsender, Wt_Signals_ wsignal, Wt::WWidget *wreceiver, Wt_Slots_ wslot, QStringList &wparameter);
	Wt::Signals::connection Wt_Connect_Step2(Wt::EventSignal< Wt::WMouseEvent > &wsignalfun, Wt::WWidget *wreceiver, Wt_Slots_ wslot, QStringList &wparameter);

	Wt_Signals_ GetSignalEnumByName(QString name);

	QHash<QString, Wt::Signals::connection> m_hashconnections;

};

class WtQtFormWidget : public WtQtInteractWidget
{
	Q_OBJECT
		// Inherited Wt::WFormWidget
		Q_PROPERTY(QString Wt_emptyText      READ Wt_emptyText      WRITE Wt_setEmptyText)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_emptyText     READ Cpp_emptyText) // TODO : CHANGE FOR placeholderText, setPlaceholderText
public:
	WtQtFormWidget(QObject *qparent = 0) : WtQtInteractWidget(qparent) {  }

	virtual QString Wt_emptyText() = 0;
	virtual void    Wt_setEmptyText(QString emptytext) = 0;

	// Cpp code generation
	QString Cpp_emptyText();
};

class WtQtAbstractToggleButton : public WtQtFormWidget
{
	Q_OBJECT
		// Inherited Wt::WAbstractToggleButton
		Q_PROPERTY(QString Wt_text      READ Wt_text      WRITE Wt_setText)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_text     READ Cpp_text)
public:
	WtQtAbstractToggleButton(QObject *qparent = 0) : WtQtFormWidget(qparent) { }

	virtual QString Wt_text() = 0;
	virtual void    Wt_setText(QString emptytext) = 0;

	// Cpp code generation
	virtual QString Cpp_text();
};


// ------------------------------------------------- Main Mixed Classes ------------------------------------------------------------------------------------------------------------

// WARNING : Inheritance order is important, mixed class should inherit from QObject first, for Qt metadata to work
//           In order to keep same Wt code to work in the original order, the constructor should have the Wt parameter first because then qobject param can be undefined (= 0)

class WtQtContainerWidget : public WtQtInteractWidget, public Wt::WContainerWidget
{
	Q_OBJECT
	// PREVIOUSLY Inherited Wt::WWebWidget
	Q_PROPERTY(QString Wt_htmlTagName   READ Wt_htmlTagName    WRITE Wt_setHtmlTagName)
	// Cpp code generation
	Q_PROPERTY(QString Cpp_htmlTagName     READ Cpp_htmlTagName)
public:
	WtQtContainerWidget(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtContainerWidget();
	WtQtContainerWidget(const WtQtContainerWidget &) = delete;
	WtQtContainerWidget & operator=(const WtQtContainerWidget &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); 
	void    Wt_setInline(QString isinline);

	virtual QString Wt_htmlTagName();
	virtual void    Wt_setHtmlTagName(QString tagname);

	// Cpp code generation
	virtual QString Cpp_htmlTagName();

};

class WtQtAnchor : public WtQtContainerWidget, public Wt::WAnchor // [NOTE] if inherits from non-abstract mixed class, should re-implement its own properties
{
	Q_OBJECT
		// Specific  Wt::WAnchor
		Q_PROPERTY(QString Wt_link           READ Wt_link           WRITE Wt_setLink        )
		Q_PROPERTY(QString Wt_target         READ Wt_target         WRITE Wt_setTarget      )
		Q_PROPERTY(QString Wt_text           READ Wt_text           WRITE Wt_setText        )
		Q_PROPERTY(QString Wt_textFormat     READ Wt_textFormat     WRITE Wt_setTextFormat  )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_link          READ Cpp_link)
		Q_PROPERTY(QString Cpp_target        READ Cpp_target)
		Q_PROPERTY(QString Cpp_text          READ Cpp_text)
		Q_PROPERTY(QString Cpp_textFormat    READ Cpp_textFormat)

public:
	WtQtAnchor(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtAnchor();
	WtQtAnchor(const WtQtAnchor &) = delete;
	WtQtAnchor & operator=(const WtQtAnchor &) = delete;

	QString Wt_className();

	QString Wt_id();  // [NOTE] added WAnchor specific otherwise wt would create an extra container and assign it the uder-defined id and give a random id to the anchor
	void    Wt_setId(QString id);

	QString Wt_styleClass(); // [NOTE] added WAnchor specific
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); // [NOTE] added WAnchor specific
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();                   // REIMPLEMENT EMPTY
	//void    Wt_setHtmlTagName(QString tagname); // REIMPLEMENT EMPTY

	QString Wt_link();
	void    Wt_setLink(QString link);

	QString Wt_target();
	void    Wt_setTarget(QString target);

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_textFormat();
	void    Wt_setTextFormat(QString textFormat);

	// Cpp code generation
	QString Cpp_link();
	QString Cpp_target();
	QString Cpp_text();
	//QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY
	QString Cpp_textFormat();

private:
	QString m_strText; // Added to fix Wt bug of returning no text after being set upon initialization


};

class WtQtText : public WtQtInteractWidget, public Wt::WText
{
	Q_OBJECT
		// Specific  Wt::WText
		Q_PROPERTY(QString Wt_textFormat     READ Wt_textFormat     WRITE Wt_setTextFormat  )
		Q_PROPERTY(QString Wt_text           READ Wt_text           WRITE Wt_setText        )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_textFormat    READ Cpp_textFormat)
		Q_PROPERTY(QString Cpp_text          READ Cpp_text      )

public:
	WtQtText(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtText();
	WtQtText(const WtQtText& other) = delete;
	WtQtText & operator=(const WtQtText &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); 
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_textFormat();
	void    Wt_setTextFormat(QString textFormat);

	QString Wt_text();
	void    Wt_setText(QString text);

	// Cpp code generation
	QString Cpp_text();
	QString Cpp_textFormat();

private:
	QString m_strText; // Added to fix Wt bug of returning no text after being set upon initialization

};

class WtQtLineEdit : public WtQtFormWidget, public Wt::WLineEdit
{
	Q_OBJECT
public:
	WtQtLineEdit(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtLineEdit();
	WtQtLineEdit(const WtQtLineEdit &) = delete;
	WtQtLineEdit & operator=(const WtQtLineEdit &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	// Cpp code generation
	QString Cpp_instantiate();
	bool    m_boolIsSearch;
};

class WtQtPushButton : public WtQtFormWidget, public Wt::WPushButton
{
	Q_OBJECT
		// Specific  Wt::WPushButton
		Q_PROPERTY(QString Wt_text          READ Wt_text           WRITE Wt_setText        )
		Q_PROPERTY(QString Wt_link          READ Wt_link           WRITE Wt_setLink        )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_text         READ Cpp_text)
		Q_PROPERTY(QString Cpp_link         READ Cpp_link)

public:
	WtQtPushButton(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtPushButton();
	WtQtPushButton(const WtQtPushButton &) = delete;
	WtQtPushButton & operator=(const WtQtPushButton &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_link();
	void    Wt_setLink(QString link);

	// Cpp code generation
	QString Cpp_text();
	QString Cpp_link();

};

class WtQtImage : public WtQtInteractWidget, public Wt::WImage
{
	Q_OBJECT
		// Specific  Wt::WImage
		Q_PROPERTY(QString Wt_imageLink      READ Wt_imageLink      WRITE Wt_setImageLink   )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_imageLink     READ Cpp_imageLink)

public:
	WtQtImage(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtImage();
	WtQtImage(const WtQtImage &) = delete;
	WtQtImage & operator=(const WtQtImage &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_imageLink(); 
	void    Wt_setImageLink(QString imagelink);

	// Cpp code generation
	QString Cpp_imageLink();

private:
	QString strImageLink;

};

class WtQtTemplate : public WtQtInteractWidget, public Wt::WTemplate
{
	Q_OBJECT
		// Specific  Wt::WTemplate
		Q_PROPERTY(QString Wt_textFormat     READ Wt_textFormat     WRITE Wt_setTextFormat) // DUMMY PROPERTY
		Q_PROPERTY(QString Wt_templateText   READ Wt_templateText   WRITE Wt_setTemplateText   )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_templateText  READ Cpp_templateText)

public:
	WtQtTemplate(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTemplate();
	WtQtTemplate(const WtQtTemplate &) = delete;
	WtQtTemplate & operator=(const WtQtTemplate &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_templateText();
	void    Wt_setTemplateText(QString templatetext);

	QString Wt_textFormat();
	void    Wt_setTextFormat(QString textFormat);

	// Cpp code generation
	QString Cpp_templateText();

private:
	QString        m_strText;     // Added to fix Wt bug of returning no text after being set upon initialization
	Wt::TextFormat m_textFormat;  // DUMMY PROPERTY

};


class WtQtSplitButton  : public WtQtCompositeWidget, public Wt::WSplitButton
{
	Q_OBJECT
		// Specific  Wt::WTemplate
		Q_PROPERTY(QString Wt_text          READ Wt_text           WRITE Wt_setText        )
		Q_PROPERTY(QString Wt_allItems      READ Wt_allItems       WRITE Wt_setAllItems    )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_text         READ Cpp_text    )
		Q_PROPERTY(QString Cpp_allItems     READ Cpp_allItems)
public:
	WtQtSplitButton(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtSplitButton();
	WtQtSplitButton(const WtQtSplitButton &) = delete;
	WtQtSplitButton & operator=(const WtQtSplitButton &) = delete;

	QString Wt_className(); // [NOTE] : need to add to helperfunctions::GetMetaObjectByClassName

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() { return QString::fromStdString(htmlTagName()); }// [NOTE] n/a for compositewidget derived classes
	//void    Wt_setHtmlTagName(QString tagname) { setHtmlTagName(tagname.toStdString()); }

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_allItems();
	void    Wt_setAllItems(QString strAllItems);

	// Cpp code generation
	QString Cpp_text();
	QString Cpp_allItems();

};


class WtQtRadioButton : public WtQtAbstractToggleButton, public Wt::WRadioButton
{
	Q_OBJECT
		// Cpp code generation
		Q_PROPERTY(QString Cpp_setGroup         READ Cpp_setGroup)
public: // [NOTE] need to pass a string to WRadioButton constructorotherwise it does not create the label and therefore not able to modify it later
	WtQtRadioButton(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtRadioButton();
	WtQtRadioButton(const WtQtRadioButton &) = delete;
	WtQtRadioButton & operator=(const WtQtRadioButton &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_text();
	void    Wt_setText(QString text);

	// Cpp code generation
	QString Cpp_instantiate();
	QString Cpp_setGroup();
	QString Cpp_text();

private:
	WtQtWidget * m_qtparent;
	bool         m_printed;

	QVector<WtQtRadioButton*> getSiblings();
	bool isFirstToPrint();
	bool isLastToPrint();
	void clearAllPrinted();

};

class WtQtCheckBox : public WtQtAbstractToggleButton, public Wt::WCheckBox
{
	Q_OBJECT
public: // [NOTE] need to pass a string to WCheckBox constructorotherwise it does not create the label and therefore not able to modify it later
	WtQtCheckBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtCheckBox();
	WtQtCheckBox(const WtQtCheckBox &) = delete;
	WtQtCheckBox & operator=(const WtQtCheckBox &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_text();
	void    Wt_setText(QString text);

};


class WtQtComboBox : public WtQtFormWidget, public Wt::WComboBox
{
	Q_OBJECT
		// Specific  Wt::WComboBox
		Q_PROPERTY(QString Wt_allItems      READ Wt_allItems       WRITE Wt_setAllItems)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_allItems     READ Cpp_allItems)
public:
	WtQtComboBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtComboBox();
	WtQtComboBox(const WtQtComboBox &) = delete;
	WtQtComboBox & operator=(const WtQtComboBox &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	virtual QString Wt_allItems();
	virtual void    Wt_setAllItems(QString strAllItems); // [NOTE] made virtual for WtQtSelectionBox

	// Cpp code generation
	virtual QString Cpp_allItems();

};

class WtQtInPlaceEdit : public WtQtCompositeWidget, public Wt::WInPlaceEdit
{
	Q_OBJECT
		// Specific  Wt::WInPlaceEdit
		Q_PROPERTY(QString Wt_text            READ Wt_text            WRITE Wt_setText)
		Q_PROPERTY(QString Wt_placeholderText READ Wt_placeholderText WRITE Wt_setPlaceholderText)
		Q_PROPERTY(QString Wt_buttonsEnabled  READ Wt_buttonsEnabled  WRITE Wt_setButtonsEnabled)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_text            READ Cpp_text)
		Q_PROPERTY(QString Cpp_placeholderText READ Cpp_placeholderText)
		Q_PROPERTY(QString Cpp_buttonsEnabled  READ Cpp_buttonsEnabled)
public:
	WtQtInPlaceEdit(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtInPlaceEdit();
	WtQtInPlaceEdit(const WtQtInPlaceEdit &) = delete;
	WtQtInPlaceEdit & operator=(const WtQtInPlaceEdit &) = delete;

	QString Wt_className(); // [NOTE] : need to add to helperfunctions::GetMetaObjectByClassName

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() // [NOTE] n/a for compositewidget derived classes

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_placeholderText();
	void    Wt_setPlaceholderText(QString text);

	QString Wt_buttonsEnabled();
	void    Wt_setButtonsEnabled(QString isinline);

	// Cpp code generation
	QString Cpp_text();
	QString Cpp_placeholderText();
	QString Cpp_buttonsEnabled();

private:
	bool m_boolButtonsEnabled;

};


class WtQtTextArea : public WtQtFormWidget, public Wt::WTextArea
{
	Q_OBJECT
		// Specific  Wt::WTextArea
		Q_PROPERTY(QString Wt_text            READ Wt_text            WRITE Wt_setText)
		Q_PROPERTY(QString Wt_columns         READ Wt_columns         WRITE Wt_setColumns)
		Q_PROPERTY(QString Wt_rows            READ Wt_rows            WRITE Wt_setRows)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_text            READ Cpp_text)
		Q_PROPERTY(QString Cpp_columns         READ Cpp_columns)
		Q_PROPERTY(QString Cpp_rows            READ Cpp_rows)

public:
	WtQtTextArea(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTextArea();
	WtQtTextArea(const WtQtTextArea &) = delete;
	WtQtTextArea & operator=(const WtQtTextArea &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_columns();
	void    Wt_setColumns(QString columns);

	QString Wt_rows();
	void    Wt_setRows(QString rows);

	// Cpp code generation
	QString Cpp_text();
	QString Cpp_columns();
	QString Cpp_rows();

};

class WtQtSelectionBox : public WtQtComboBox, public Wt::WSelectionBox 
{
	Q_OBJECT
		// [NOTE] Wt_allItems and Cpp_allItems are inherited and made virtual in WtQtComboBox
		// [NOTE] Had to hide WtQtComboBox in constructor because it was showing in chrome
public:
	WtQtSelectionBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtSelectionBox();
	WtQtSelectionBox(const WtQtSelectionBox &) = delete;
	WtQtSelectionBox & operator=(const WtQtSelectionBox &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_allItems();
	void    Wt_setAllItems(QString strAllItems);

	// Cpp code generation
	QString Cpp_allItems();

};

class WtQtSpinBox : public WtQtLineEdit, public Wt::WSpinBox
{
	Q_OBJECT
		// Specific  Wt::WSpinBox
		Q_PROPERTY(QString Wt_minimum          READ Wt_minimum          WRITE Wt_setMinimum)
		Q_PROPERTY(QString Wt_maximum          READ Wt_maximum          WRITE Wt_setMaximum)
		Q_PROPERTY(QString Wt_value            READ Wt_value            WRITE Wt_setValue)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_minimum         READ Cpp_minimum)
		Q_PROPERTY(QString Cpp_maximum         READ Cpp_maximum)
		Q_PROPERTY(QString Cpp_value           READ Cpp_value)
public:
	WtQtSpinBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtSpinBox();
	WtQtSpinBox(const WtQtSpinBox &) = delete;
	WtQtSpinBox & operator=(const WtQtSpinBox &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_minimum();
	void    Wt_setMinimum(QString minimum);

	QString Wt_maximum();
	void    Wt_setMaximum(QString maximum);

	QString Wt_value();
	void    Wt_setValue(QString value);

	// Cpp code generation
	QString Cpp_minimum();
	QString Cpp_maximum();
	QString Cpp_value();
};


class WtQtDoubleSpinBox : public WtQtLineEdit, public Wt::WDoubleSpinBox
{
	Q_OBJECT
		// Specific  Wt::WDoubleSpinBox
		Q_PROPERTY(QString Wt_minimum          READ Wt_minimum          WRITE Wt_setMinimum)
		Q_PROPERTY(QString Wt_maximum          READ Wt_maximum          WRITE Wt_setMaximum)
		Q_PROPERTY(QString Wt_value            READ Wt_value            WRITE Wt_setValue)
		Q_PROPERTY(QString Wt_singleStep       READ Wt_singleStep       WRITE Wt_setSingleStep)
		Q_PROPERTY(QString Wt_decimals         READ Wt_decimals         WRITE Wt_setDecimals )
		// Cpp code generation
		Q_PROPERTY(QString Cpp_minimum         READ Cpp_minimum)
		Q_PROPERTY(QString Cpp_maximum         READ Cpp_maximum)
		Q_PROPERTY(QString Cpp_value           READ Cpp_value)
		Q_PROPERTY(QString Cpp_singleStep      READ Cpp_singleStep)
		Q_PROPERTY(QString Cpp_decimals        READ Cpp_decimals)
public:
	WtQtDoubleSpinBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtDoubleSpinBox();
	WtQtDoubleSpinBox(const WtQtDoubleSpinBox &) = delete;
	WtQtDoubleSpinBox & operator=(const WtQtDoubleSpinBox &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_minimum();
	void    Wt_setMinimum(QString minimum);

	QString Wt_maximum();
	void    Wt_setMaximum(QString maximum);

	QString Wt_value();
	void    Wt_setValue(QString value);

	QString Wt_singleStep();
	void    Wt_setSingleStep(QString step);

	QString Wt_decimals();
	void    Wt_setDecimals(QString decimals);

	// Cpp code generation
	QString Cpp_minimum();
	QString Cpp_maximum();
	QString Cpp_value();
	QString Cpp_singleStep();
	QString Cpp_decimals();
};


class WtQtTimeEdit : public WtQtLineEdit, public Wt::WTimeEdit
{
	Q_OBJECT
		// Specific  Wt::WTimeEdit
		Q_PROPERTY(QString Wt_minuteStep          READ Wt_minuteStep          WRITE Wt_setMinuteStep )
		Q_PROPERTY(QString Wt_format              READ Wt_format              WRITE Wt_setFormat )
		// TODO : time necessary? maybe not
		// Cpp code generation
		Q_PROPERTY(QString Cpp_minuteStep         READ Cpp_minuteStep)
		Q_PROPERTY(QString Cpp_format             READ Cpp_format)
public:
	WtQtTimeEdit(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTimeEdit();
	WtQtTimeEdit(const WtQtTimeEdit &) = delete;
	WtQtTimeEdit & operator=(const WtQtTimeEdit &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_minuteStep();
	void    Wt_setMinuteStep(QString step);

	QString Wt_format();
	void    Wt_setFormat(QString format);

	// Cpp code generation
	QString Cpp_minuteStep();
	QString Cpp_format();

};

class WtQtDateEdit : public WtQtLineEdit, public Wt::WDateEdit
{
	Q_OBJECT
		// Specific  Wt::WDateEdit
		Q_PROPERTY(QString Wt_format              READ Wt_format              WRITE Wt_setFormat)
		// TODO : date necessary? maybe not
		// Cpp code generation
		Q_PROPERTY(QString Cpp_format             READ Cpp_format)
public:
	WtQtDateEdit(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtDateEdit();
	WtQtDateEdit(const WtQtDateEdit &) = delete;
	WtQtDateEdit & operator=(const WtQtDateEdit &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_format();
	void    Wt_setFormat(QString format);

	// Cpp code generation
	QString Cpp_format();

};

class WtQtCalendar : public WtQtCompositeWidget, public Wt::WCalendar
{
	Q_OBJECT

public:
	WtQtCalendar(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtCalendar();
	WtQtCalendar(const WtQtCalendar &) = delete;
	WtQtCalendar & operator=(const WtQtCalendar &) = delete;

	QString Wt_className(); // [NOTE] : need to add to helperfunctions::GetMetaObjectByClassName

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() { return QString::fromStdString(htmlTagName()); } // [NOTE] n/a for compositewidget derived classes
	//void    Wt_setHtmlTagName(QString tagname) { setHtmlTagName(tagname.toStdString()); }

};

class WtQtSlider : public WtQtFormWidget, public Wt::WSlider
{
	Q_OBJECT
		// Specific  Wt::WSpinBox
		Q_PROPERTY(QString Wt_minimum          READ Wt_minimum          WRITE Wt_setMinimum)
		Q_PROPERTY(QString Wt_maximum          READ Wt_maximum          WRITE Wt_setMaximum)
		Q_PROPERTY(QString Wt_value            READ Wt_value            WRITE Wt_setValue)
		Q_PROPERTY(QString Wt_tickInterval     READ Wt_tickInterval     WRITE Wt_setTickInterval)
		Q_PROPERTY(QString Wt_tickPosition     READ Wt_tickPosition     WRITE Wt_setTickPosition)
		Q_PROPERTY(QString Wt_orientation      READ Wt_orientation      WRITE Wt_setOrientation)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_minimum         READ Cpp_minimum)
		Q_PROPERTY(QString Cpp_maximum         READ Cpp_maximum)
		Q_PROPERTY(QString Cpp_value           READ Cpp_value)
		Q_PROPERTY(QString Cpp_tickInterval    READ Cpp_tickInterval)
		Q_PROPERTY(QString Cpp_tickPosition    READ Cpp_tickPosition)
		Q_PROPERTY(QString Cpp_orientation     READ Cpp_orientation)
public:
	WtQtSlider(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtSlider();
	WtQtSlider(const WtQtSlider &) = delete;
	WtQtSlider & operator=(const WtQtSlider &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_emptyText();
	void    Wt_setEmptyText(QString emptytext);

	QString Wt_minimum();
	void    Wt_setMinimum(QString minimum);

	QString Wt_maximum();
	void    Wt_setMaximum(QString maximum);

	QString Wt_value();
	void    Wt_setValue(QString value);

	QString Wt_tickInterval();
	void    Wt_setTickInterval(QString interval);

	QString Wt_tickPosition();
	void    Wt_setTickPosition(QString position);

	QString Wt_orientation();
	void    Wt_setOrientation(QString orientation);

	// Cpp code generation
	QString Cpp_minimum();
	QString Cpp_maximum();
	QString Cpp_value();
	QString Cpp_tickInterval();
	QString Cpp_tickPosition();
	QString Cpp_orientation();
};


class WtQtFileUpload : public WtQtWebWidget, public Wt::WFileUpload
{
	Q_OBJECT
public:
	WtQtFileUpload(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtFileUpload();
	WtQtFileUpload(const WtQtFileUpload &) = delete;
	WtQtFileUpload & operator=(const WtQtFileUpload &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

};

class WtQtProgressBar : public WtQtInteractWidget, public Wt::WProgressBar
{
	Q_OBJECT
		// Specific  Wt::WSpinBox
		Q_PROPERTY(QString Wt_minimum          READ Wt_minimum          WRITE Wt_setMinimum)
		Q_PROPERTY(QString Wt_maximum          READ Wt_maximum          WRITE Wt_setMaximum)
		Q_PROPERTY(QString Wt_value            READ Wt_value            WRITE Wt_setValue)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_minimum         READ Cpp_minimum)
		Q_PROPERTY(QString Cpp_maximum         READ Cpp_maximum)
		Q_PROPERTY(QString Cpp_value           READ Cpp_value)
public:
	WtQtProgressBar(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtProgressBar();
	WtQtProgressBar(const WtQtProgressBar &) = delete;
	WtQtProgressBar & operator=(const WtQtProgressBar &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_minimum();
	void    Wt_setMinimum(QString minimum);

	QString Wt_maximum();
	void    Wt_setMaximum(QString maximum);

	QString Wt_value();
	void    Wt_setValue(QString value);

	// Cpp code generation
	QString Cpp_minimum();
	QString Cpp_maximum();
	QString Cpp_value();
};


class WtQtGroupBox : public WtQtContainerWidget, public Wt::WGroupBox // [NOTE] if inherits from non-abstract mixed class, should re-implement its own properties
{
	Q_OBJECT
		// Specific  Wt::WGroupBox
		Q_PROPERTY(QString Wt_title           READ Wt_title           WRITE Wt_setTitle)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_title          READ Cpp_title)

public:
	WtQtGroupBox(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtGroupBox();
	WtQtGroupBox(const WtQtGroupBox &) = delete;
	WtQtGroupBox & operator=(const WtQtGroupBox &) = delete;

	QString Wt_className();

	QString Wt_id();  // [NOTE] added WGroupBox specific otherwise wt would create an extra container and assign it the uder-defined id and give a random id to the anchor
	void    Wt_setId(QString id);

	QString Wt_styleClass(); // [NOTE] added WGroupBox specific
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); // [NOTE] added WGroupBox specific
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();                   // REIMPLEMENT EMPTY
	void    Wt_setHtmlTagName(QString tagname); // REIMPLEMENT EMPTY

	QString Wt_title();
	void    Wt_setTitle(QString text);

	// Cpp code generation
	QString Cpp_title();
	QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY

};

// [NOTE] : Any widget that we want to act as a container needs to be added to the if-else list in
// 1) helperfunctions::GetWContainerParent
// *) Finally if children widgets are not direct children of widget but of a hidden widget
//    don't forget to add a case in WtQtWidget::Cpp_getCorrectParentId
class WtQtPanel : public WtQtCompositeWidget, public Wt::WPanel
{
	Q_OBJECT
	// Specific  Wt::WPanel
	Q_PROPERTY(QString Wt_title          READ Wt_title           WRITE Wt_setTitle)
	Q_PROPERTY(QString Wt_isCollapsible  READ Wt_isCollapsible   WRITE Wt_setCollapsible)
	// Cpp code generation
	Q_PROPERTY(QString Cpp_title         READ Cpp_title)
	Q_PROPERTY(QString Cpp_isCollapsible READ Cpp_isCollapsible)
	Q_PROPERTY(QString Cpp_centralWidget READ Cpp_centralWidget)
public:
	WtQtPanel(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtPanel();
	WtQtPanel(const WtQtPanel &) = delete;
	WtQtPanel & operator=(const WtQtPanel &) = delete;

	QString Wt_className(); // [NOTE] : need to add to helperfunctions::GetMetaObjectByClassName

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() {// [NOTE] n/a for compositewidget derived classes

	QString Wt_title();
	void    Wt_setTitle(QString text);

	QString Wt_isCollapsible();
	void    Wt_setCollapsible(QString collapsible);

	// EXTRA CODE FOR FUNCTIONALITY
	Wt::WContainerWidget* getCentralContainer();
	QString getCentralWidgetId();

	// Cpp code generation
	virtual QString Cpp_instantiate(); // Overload !
	QString Cpp_title();
	QString Cpp_isCollapsible();
	QString Cpp_centralWidget();

private:
	QString m_strCentralWidgetId;
	Wt::WContainerWidget * m_centralWidget;
};

class WtQtStackedWidget : public WtQtContainerWidget, public Wt::WStackedWidget // [NOTE] if inherits from non-abstract mixed class, should re-implement its own properties
{
	Q_OBJECT
		// Specific  Wt::WGroupBox
		Q_PROPERTY(QString Wt_currentIndex           READ Wt_currentIndex           WRITE Wt_setCurrentIndex)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_currentIndex          READ Cpp_currentIndex)

public:
	WtQtStackedWidget(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtStackedWidget();
	WtQtStackedWidget(const WtQtStackedWidget &) = delete;
	WtQtStackedWidget & operator=(const WtQtStackedWidget &) = delete;

	QString Wt_className();

	QString Wt_id();  // [NOTE] added WGroupBox specific otherwise wt would create an extra container and assign it the uder-defined id and give a random id to the anchor
	void    Wt_setId(QString id);

	QString Wt_styleClass(); // [NOTE] added WGroupBox specific
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); // [NOTE] added WGroupBox specific
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();                   // REIMPLEMENT EMPTY
	void    Wt_setHtmlTagName(QString tagname); // REIMPLEMENT EMPTY

	QString Wt_currentIndex();
	void    Wt_setCurrentIndex(QString index);

	// Cpp code generation
	QString Cpp_currentIndex();
	QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY

};

// [NOTE] WtQtWTabWidget should only accept WtQtTabPage as children, ideally from right click only
//        but also acts as parent so should be included in options
class WtQtTabWidget : public WtQtCompositeWidget, public Wt::WTabWidget
{
	Q_OBJECT

public:
	WtQtTabWidget(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTabWidget();
	WtQtTabWidget(const WtQtTabWidget &) = delete;
	WtQtTabWidget & operator=(const WtQtTabWidget &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() [NOTE] n/a for compositewidget derived classes

	Wt::WContainerWidget * getWtParent();
	bool                   isBeingDeleted();

private:
	Wt::WContainerWidget *m_wparent;
	bool                  m_beingDeleted;

};

// [NOTE] NOT a real WWidget but a convenience class, ideally only could be added with right click on WTabWidget
class WtQtTabItem : public WtQtContainerWidget
{
	Q_OBJECT
		// Specific  WtQtTabPage
		Q_PROPERTY(QString Wt_tabText            READ Wt_tabText           WRITE Wt_setTabText     )
		Q_PROPERTY(QString Wt_isTabEnabled       READ Wt_isTabEnabled      WRITE Wt_setTabEnabled  )
		Q_PROPERTY(QString Wt_isTabHidden        READ Wt_isTabHidden       WRITE Wt_setTabHidden   )
		Q_PROPERTY(QString Wt_isTabCloseable     READ Wt_isTabCloseable    WRITE Wt_setTabCloseable)
		// Cpp code generation
		//Q_PROPERTY(QString Cpp_tabText            READ Cpp_tabText       ) 
		Q_PROPERTY(QString Cpp_isTabEnabled       READ Cpp_isTabEnabled  )
		Q_PROPERTY(QString Cpp_isTabHidden        READ Cpp_isTabHidden   )
		Q_PROPERTY(QString Cpp_isTabCloseable     READ Cpp_isTabCloseable)
public:
	WtQtTabItem(WtQtTabWidget *qparent);
	~WtQtTabItem();
	WtQtTabItem(const WtQtTabItem &) = delete;
	WtQtTabItem & operator=(const WtQtTabItem &) = delete;

	QString Wt_className();

	// -----------------------------------------

	QString Wt_id();  // for m_childWidget
	void    Wt_setId(QString id);

	QString Wt_styleClass(); // for m_childWidget
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); // for m_childWidget
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();// for m_childWidget  // REIMPLEMENT EMPTY
	void    Wt_setHtmlTagName(QString tagname);    // REIMPLEMENT EMPTY

	// -----------------------------------------

	QString Wt_tabText();  // for tab
	void    Wt_setTabText(QString tabtext);

	QString Wt_isTabEnabled();  // for tab
	void    Wt_setTabEnabled(QString benabled);

	QString Wt_isTabHidden();  // for tab
	void    Wt_setTabHidden(QString bhidden);

	QString Wt_isTabCloseable();  // for tab
	void    Wt_setTabCloseable(QString bclosable);

	// Cpp code generation
	QString Cpp_declare();     // declare as WContainerWidget
	QString Cpp_instantiate(); // wrt WtQtWTabWidget parent

	//QString Cpp_tabText(); // already implemented in Cpp_instantiate
	QString Cpp_isTabEnabled();
	QString Cpp_isTabHidden();
	QString Cpp_isTabCloseable();
	QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY

private:
	WtQtTabWidget        * m_qparent;
	Wt::WContainerWidget * m_childWidget;

};


// [NOTE] WtQtMenu should only accept WtQtMenuItem as children, ideally from right click only
//        but also acts as parent so should be included in options
class WtQtMenu : public WtQtCompositeWidget, public Wt::WMenu
{
	Q_OBJECT

public:
	WtQtMenu(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtMenu();
	WtQtMenu(const WtQtMenu &) = delete;
	WtQtMenu & operator=(const WtQtMenu &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName() [NOTE] n/a for compositewidget derived classes

	Wt::WContainerWidget * getWtParent();
	bool                   isBeingDeleted();

	// Cpp code generation
	QString Cpp_instantiate();

private:
	Wt::WContainerWidget *m_wparent;
	bool                  m_beingDeleted;
	bool                  m_isLeftWMenu;
};

// [NOTE] NOT a real WWidget but a convenience class, ideally only could be added with right click on WMenu
class WtQtMenuItem : public WtQtContainerWidget
{
	Q_OBJECT
		// Specific  WtQtTabPage
		Q_PROPERTY(QString Wt_text                READ Wt_text              WRITE Wt_setText        )
		Q_PROPERTY(QString Wt_isItemDisabled      READ Wt_isItemDisabled    WRITE Wt_setItemDisabled)
		Q_PROPERTY(QString Wt_isItemHidden        READ Wt_isItemHidden      WRITE Wt_setItemHidden  )
		// Cpp code generation
		//Q_PROPERTY(QString Cpp_text               READ Cpp_text       ) 
		Q_PROPERTY(QString Cpp_isItemDisabled     READ Cpp_isItemDisabled)
		Q_PROPERTY(QString Cpp_isItemHidden       READ Cpp_isItemHidden)
public:
	WtQtMenuItem(WtQtMenu *qparent);
	~WtQtMenuItem();
	WtQtMenuItem(const WtQtMenuItem &) = delete;
	WtQtMenuItem & operator=(const WtQtMenuItem &) = delete;

	QString Wt_className();

	QString Wt_id();  
	void    Wt_setId(QString id);

	QString Wt_styleClass(); 
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline(); 
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();                   // REIMPLEMENT EMPTY
	void    Wt_setHtmlTagName(QString tagname); // REIMPLEMENT EMPTY

	QString Wt_text(); 
	void    Wt_setText(QString text);

	QString Wt_isItemDisabled(); 
	void    Wt_setItemDisabled(QString disabled);

	QString Wt_isItemHidden(); 
	void    Wt_setItemHidden(QString hidden);

	// Cpp code generation
	QString Cpp_declare();     // overload
	QString Cpp_instantiate(); // overload
	//
	//QString Cpp_text(); 
	QString Cpp_isItemDisabled();
	QString Cpp_isItemHidden();
	QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY

private:
	WtQtMenu             * m_qparent;
	Wt::WContainerWidget * m_contentwidget;
	Wt::WMenuItem        * m_menuitem;
};

// [NOTE] WtQtPopupMenu should only accept WtQtPopupItem or WtQtPopupSeparator as children, ideally from right click only
//        but also acts as parent so should be included in options
class WtQtPopupMenu : public WtQtContainerWidget // need to instantiate WtQtContainerWidget in order to form part of the QObject tree
{
	Q_OBJECT
		// Specific if is submenu
		Q_PROPERTY(QString Wt_text                READ Wt_text              WRITE Wt_setText       )

public:// doesnt add WContainerWidget to Wt tree but does add to Qt tree to be able to be found
	WtQtPopupMenu(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0); 
	~WtQtPopupMenu();
	WtQtPopupMenu(const WtQtPopupMenu &) = delete;
	WtQtPopupMenu & operator=(const WtQtPopupMenu &) = delete;

	QString Wt_className();

	// WPushButton properties

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_text();
	void    Wt_setText(QString text);

	bool                   isBeingDeleted();
	Wt::WContainerWidget * getWtParent();
	Wt::WWidget          * getInternalWWidget();
	Wt::WPopupMenu       * getWPopupMenu();

	// Cpp code generation
	QString Cpp_declare();     // overload 
	QString Cpp_instantiate(); // reimplement
	QString Cpp_isInline()   ; // reimplement
	QString Cpp_htmlTagName(); // reimplement


private:
	Wt::WContainerWidget * m_wparent;
	Wt::WPushButton      * m_pushbutton;
	Wt::WPopupMenu       * m_wmenu;
	Wt::WMenuItem        * m_submenuitem;
	Wt::WMenu            * m_rightMenu;
	bool                   m_beingDeleted;
	bool                   m_isSubmenu;
	bool                   m_isRightWMenu;

};

// [NOTE] NOT a real WWidget but a convenience class (fake father), ideally only could be added with right click on WtQtPopupMenu
class WtQtPopupItem : public WtQtContainerWidget
{
	Q_OBJECT
		// Specific  WtQtTabPage
		Q_PROPERTY(QString Wt_text                READ Wt_text              WRITE Wt_setText       )
		Q_PROPERTY(QString Wt_icon                READ Wt_icon              WRITE Wt_setIcon       )
		Q_PROPERTY(QString Wt_isCheckable         READ Wt_isCheckable       WRITE Wt_setCheckable  )
		Q_PROPERTY(QString Wt_isChecked           READ Wt_isChecked         WRITE Wt_setChecked    )
		Q_PROPERTY(QString Wt_isCloseable         READ Wt_isCloseable       WRITE Wt_setCloseable  )
		// Cpp code generation
		//Q_PROPERTY(QString Cpp_text               READ Cpp_text       ) 
		Q_PROPERTY(QString Cpp_icon              READ Cpp_icon        )
		Q_PROPERTY(QString Cpp_isCheckable       READ Cpp_isCheckable )
		Q_PROPERTY(QString Cpp_isChecked         READ Cpp_isChecked   )
		Q_PROPERTY(QString Cpp_isCloseable       READ Cpp_isCloseable)
public:
	WtQtPopupItem(WtQtPopupMenu *qparent); // hides WContainerWidget
	~WtQtPopupItem();
	WtQtPopupItem(const WtQtPopupItem &) = delete;
	WtQtPopupItem & operator=(const WtQtPopupItem) = delete;

	QString Wt_className();

	// Properties act on Wt::WMenuItem

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass(); 
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	QString Wt_htmlTagName();                   // REIMPLEMENT EMPTY
	void    Wt_setHtmlTagName(QString tagname); // REIMPLEMENT EMPTY

	QString Wt_text();
	void    Wt_setText(QString text);

	QString Wt_icon();
	void    Wt_setIcon(QString iconpath);

	QString Wt_isCheckable();
	void    Wt_setCheckable(QString checkable);

	QString Wt_isChecked();
	void    Wt_setChecked(QString checked);

	QString Wt_isCloseable();
	void    Wt_setCloseable(QString closable);

	// Cpp code generation
	QString Cpp_declare();     // overload
	QString Cpp_instantiate(); // overload
	//
	//QString Cpp_text(); 
	QString Cpp_icon();
	QString Cpp_isCheckable();
	QString Cpp_isChecked();
	QString Cpp_isCloseable();
	QString Cpp_htmlTagName(); // REIMPLEMENT EMPTY

private:
	WtQtPopupMenu        * m_qparent;
	Wt::WMenuItem        * m_menuitem;
};

class WtQtTable : public WtQtInteractWidget, public Wt::WTable
{
	Q_OBJECT
	// Specific  Wt::WTable
	Q_PROPERTY(QString Wt_headerCountHorizontal       READ Wt_headerCountHorizontal       WRITE Wt_setHeaderCountHorizontal)
	Q_PROPERTY(QString Wt_headerCountVertical         READ Wt_headerCountVertical         WRITE Wt_setHeaderCountVertical  )
	// Cpp code generation
	Q_PROPERTY(QString Cpp_headerCountHorizontal      READ Cpp_headerCountHorizontal )
	Q_PROPERTY(QString Cpp_headerCountVertical        READ Cpp_headerCountVertical   )
	// TODO : Width and Height or through CSS ???
public:
	WtQtTable(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTable();
	WtQtTable(const WtQtTable &) = delete;
	WtQtTable & operator=(const WtQtTable &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_headerCountHorizontal();
	void    Wt_setHeaderCountHorizontal(QString headcount);

	QString Wt_headerCountVertical();
	void    Wt_setHeaderCountVertical(QString headcount);

	// Cpp code generation
	QString Cpp_headerCountHorizontal();
	QString Cpp_headerCountVertical();

};

class WtQtTree : public WtQtCompositeWidget, public Wt::WTree
{
	Q_OBJECT

public:
	WtQtTree(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTree();
	WtQtTree(const WtQtTree &) = delete;
	WtQtTree & operator=(const WtQtTree &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

};

class WtQtTreeTable : public WtQtCompositeWidget, public Wt::WTreeTable
{
	Q_OBJECT

public:
	WtQtTreeTable(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtTreeTable();
	WtQtTreeTable(const WtQtTreeTable &) = delete;
	WtQtTreeTable & operator=(const WtQtTreeTable) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

private:
	Wt::WTreeTableNode *addNode(Wt::WTreeTableNode *parent, const char *name,
		const char *yuppie, const char *holidays,
		const char *favorite);

};


// [NOTE] NOT a real WWidget but a convenience class (fake father), 
// only accepts WMenu      ( addMenu(leftMenu)                   ), 
//              WPopupMenu ( addMenu(rightMenu, Wt::AlignRight)  ),
//          and WLineEdit  ( addSearch(edit, Wt::AlignRight)     ),
// ADD : MainPage::CreateWtQtInstance
// ADD : helperfunctions::GetMetaObjectByClassName
// ADD : MainPage::PerformDisconnection
class WtQtNavigationBar : public WtQtInteractWidget, public Wt::WNavigationBar // need to instantiate WtQtInteractWidget in order to form part of the QObject tree
{
	Q_OBJECT
		// Specific if is navbar
		Q_PROPERTY(QString Wt_title                READ Wt_title              WRITE Wt_setTitle)
		Q_PROPERTY(QString Wt_responsive           READ Wt_responsive         WRITE Wt_setResponsive)
		// Cpp code generation
		Q_PROPERTY(QString Cpp_title               READ Cpp_title)
		Q_PROPERTY(QString Cpp_responsive          READ Cpp_responsive)
public:
	WtQtNavigationBar(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtNavigationBar();
	WtQtNavigationBar(const WtQtNavigationBar &) = delete;
	WtQtNavigationBar & operator=(const WtQtNavigationBar &) = delete;

	QString Wt_className();

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_title();
	void    Wt_setTitle(QString text);

	QString Wt_responsive();
	void    Wt_setResponsive(QString isresponsive);

	Wt::WContainerWidget * getWtParent();       // for wt tree
	QString                getParentWidgetId(); // for cpp autogen

	// Cpp code generation
	QString Cpp_title();
	QString Cpp_responsive();

private:
	Wt::WContainerWidget * m_wparent;
	QString                m_strTitle;
	QString                m_strResponsive;
	QString                m_strParentId;
};

// Made-up widget, on WtDesigner should behave as empty container, no childrem, but
// upon cpp autogen should insert headerLocation in includes with <> if isGlobalInclude
// or "" if not, and declare the class according to promotedClass
class WtQtPromotedWidget : public WtQtWidget, public Wt::WContainerWidget
{
	Q_OBJECT
		// Specific if is promoted widget
		Q_PROPERTY(QString Wt_promotedClass        READ Wt_promotedClass      WRITE Wt_setPromotedClass )
		Q_PROPERTY(QString Wt_headerLocation       READ Wt_headerLocation     WRITE Wt_setHeaderLocation)
		Q_PROPERTY(QString Wt_isGlobalInclude      READ Wt_isGlobalInclude    WRITE Wt_setGlobalInclude )
		// Cpp code generation
		//Q_PROPERTY(QString Cpp_promotedClass       READ Cpp_promotedClass  ) // [NOTE] Not necessary   
		//Q_PROPERTY(QString Cpp_headerLocation      READ Cpp_headerLocation ) // [NOTE] Not necessary
		//Q_PROPERTY(QString Cpp_isGlobalInclude     READ Cpp_isGlobalInclude) // [NOTE] Not necessary
public:
	WtQtPromotedWidget(Wt::WContainerWidget *wparent = 0, QObject *qparent = 0);
	~WtQtPromotedWidget();
	WtQtPromotedWidget(const WtQtPromotedWidget &) = delete;
	WtQtPromotedWidget & operator=(const WtQtPromotedWidget &) = delete;

	QString Wt_className(); // "WPromotedWidget" not real, just to follow convention

	QString Wt_id();
	void    Wt_setId(QString id);

	QString Wt_styleClass();
	void    Wt_setStyleClass(QString styleclass);

	QString Wt_isInline();
	void    Wt_setInline(QString isinline);

	//QString Wt_htmlTagName();
	//void    Wt_setHtmlTagName(QString tagname);

	QString Wt_promotedClass();
	void    Wt_setPromotedClass(QString promotedClass);

	QString Wt_headerLocation();
	void    Wt_setHeaderLocation(QString headerLocation);

	QString Wt_isGlobalInclude();
	void    Wt_setGlobalInclude(QString isGlobalInclude);

	// Cpp code generation
	QString Cpp_declare();     // declare as Wt_promotedClass
	QString Cpp_instantiate(); // instantiate as Wt_promotedClass

private:
	QString m_strPromotedClass;
	QString m_strHeaderLocation;
	bool    m_boolIsGlobalInclude;

};


#endif // MIXEDCLASSES_H
