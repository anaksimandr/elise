
#include "skinobject.h"

const QString cqsBackgroundColor = "backgroundColor";
const QString cqsTitleColor = "titleColor";
const QString cqsAlphaValue = "alphaValu";
const QString cqsTitleFont = "titleFont";
const QString cqsSkinPath = "skinPath";

SkinObject::SkinObject(QWidget* wgtParent)
	: QObject()
{
	//-- Widget which will be skinned
	skinWidget = wgtParent;
	styleRenderer = 0;
	contentsRect = skinWidget->rect();
	//-- Save flags for later unsset the skin
	flags = skinWidget->windowFlags();
	//-- Do some initialisation
	//skinTimer = new QBasicTimer();
	widgetMask = QPixmap(skinWidget->width()+12, skinWidget->height()+12);
	mousePress = false;
}

int SkinObject::startSkinning()
{
	if (loadSettings())
		return 1;
	QPoint pos = skinWidget->pos();
	skinWidget->setAttribute(Qt::WA_TranslucentBackground);
	//-- No windows frame
	skinWidget->setWindowFlags(Qt::FramelessWindowHint);

	//-- Mouse tracking on, because the close-button should glow if the cursor comes over it
	skinWidget->setMouseTracking(true);

	QRectF rect_top = styleRenderer->boundsOnElement("border_top");
	QRectF rect_right = styleRenderer->boundsOnElement("border_right");
	QRectF rect_bottom = styleRenderer->boundsOnElement("border_bottom");
	QRectF rect_left = styleRenderer->boundsOnElement("border_left");

	skinWidget->setContentsMargins(rect_left.width(), rect_top.height(), rect_right.width(), rect_bottom.height());

	skinWidget->installEventFilter(this);
	resizeHandler =  new QSkinWidgetResizeHandler(this, skinWidget);
	setAlphaValue(alpha);
	skinTimer->start(100, this);
	skinWidget->show();
	skinWidget->move(pos);
	return 0;
}

void SkinObject::stopSkinning()
{
	skinWidget->setContentsMargins ( 5, 5, 5, 5);
	skinWidget->clearMask();
	skinWidget->setAttribute(Qt::WA_TranslucentBackground, false);
	skinWidget->setWindowFlags(flags);
	skinWidget->removeEventFilter(this);
	skinWidget->removeEventFilter(resizeHandler);
	//skinTimer->stop();
	skinWidget->setMouseTracking(false);
	//setAlphaValue(1.0);
	skinWidget->show();
}

int SkinObject::saveSettings()
{
	bool fail = false;
	QString settingName;
	EColor col;
	QString textVal;
	//-- Prepare for save settings
	SETTING* set = new SETTING;
	set->contact = 0;
	set->qsModule = &E_SKIN_MODULE;
	set->qsSetting = &settingName;
	set->var = new DBVARIANT;
	set->var->type = intType;
	//////-- Background color --////////////////////////////////////////////
	settingName = cqsBackgroundColor;
	col.chars[0] = backgroundColor.red();
	col.chars[1] = backgroundColor.green();
	col.chars[2] = backgroundColor.blue();
	set->var->intValue = col.value;
	if (CallService(&DB_WRITESETTING, 0, (uintptr_t)set))
		fail = true;
	//////-- Title color --/////////////////////////////////////////////////
	settingName = cqsTitleColor;
	col.chars[0] = titleColor.red();
	col.chars[1] = titleColor.green();
	col.chars[2] = titleColor.blue();
	set->var->intValue = col.value;
	if (CallService(&DB_WRITESETTING, 0, (uintptr_t)set))
		fail = true;
	//////-- Alpha value --/////////////////////////////////////////////////
	settingName = cqsAlphaValue;
	set->var->type = realType;
	set->var->realValue = alpha;
	if (CallService(&DB_WRITESETTING, 0, (uintptr_t)set))
		fail = true;
	//////-- Title font --//////////////////////////////////////////////////
	settingName =  cqsTitleFont;
	set->var->type = textType;
	textVal = titleFont.toString();
	set->var->textValue = &textVal;
	if (CallService(&DB_WRITESETTING, 0, (uintptr_t)set))
		fail = true;
	//////-- Skin path --///////////////////////////////////////////////////
	settingName = cqsSkinPath;
	set->var->textValue = &skinPath;
	if (CallService(&DB_WRITESETTING, 0, (uintptr_t)set))
		fail = true;
	delete set->var;
	delete set;
	return fail;
}

int SkinObject::loadSettings()
{
	QString settingName;
	EColor col;
	QString textRes;
	//-- Prepare for read settings
	SETTING* set = new SETTING;
	set->contact = 0;
	set->qsModule = &E_SKIN_MODULE;
	set->qsSetting = &settingName;
	set->var = new DBVARIANT;
	set->var->type = intType;
	//////-- Background color --////////////////////////////////////////////
	settingName = cqsBackgroundColor;
	if (CallService(&DB_READSETTING, 0, (uintptr_t)set))
		return 1;
	col.value = set->var->intValue;
	backgroundColor = QColor(col.chars[0], col.chars[1], col.chars[2]);
	//////-- Title color --/////////////////////////////////////////////////
	settingName = cqsTitleColor;
	if (CallService(&DB_READSETTING, 0, (uintptr_t)set))
		return 1;
	col.value = set->var->intValue;
	titleColor = QColor(col.chars[0], col.chars[1], col.chars[2]);
	//////-- Alpha value --/////////////////////////////////////////////////
	settingName = cqsAlphaValue;
	set->var->type = realType;
	if (CallService(&DB_READSETTING, 0, (uintptr_t)set))
		return 1;
	alpha = set->var->realValue;
	//////-- Title font --//////////////////////////////////////////////////
	settingName =  cqsTitleFont;
	set->var->type = textType;
	set->var->textValue = &textRes;
	if (CallService(&DB_READSETTING, 0, (uintptr_t)set))
		return 1;
	titleFont.fromString(*set->var->textValue);
	//////-- Skin path --///////////////////////////////////////////////////
	settingName = cqsSkinPath;
	if (CallService(&DB_READSETTING, 0, (uintptr_t)set))
		return 1;
	skinPath = *set->var->textValue;
	styleRenderer = new QSvgRenderer(skinPath);
	delete set->var;
	delete set;
	return 0;
}

