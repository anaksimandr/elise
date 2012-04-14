#ifndef SKINOBJECT_H
#define SKINOBJECT_H

#include "../../commonheaders.h"
#include "skinresizehandler.h"

class SkinResizeHandler;
class SkinObject;

class SkinObject : public QObject
{
	Q_OBJECT
private:
	QWidget*		skinWidget;
	QSvgRenderer*	styleRenderer;
	QRect			contentsRect;
	Qt::WindowFlags	flags;
	//QBasicTimer*	skinTimer;
	QPixmap			widgetMask;			//-- The pixmap, in which the ready frame is stored
	bool			mousePress;

	QColor			titleColor;
	QColor			backgroundColor;
	QFont			titleFont;
	qreal			alpha;
	QString			skinPath;
protected:
	//bool eventFilter(QObject *o, QEvent *e);
	//void mouseReleaseEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *mouseEvent);
	//void paintEvent(QPaintEvent *event);
	//void resizeEvent(QResizeEvent *e);
	int				saveSettings();
	int				loadSettings();
	//void manageRegions();
	//void timerEvent (QTimerEvent * event);
public slots:
	//void updateStyle();
	//void updateButtons();
	int				startSkinning();
	void			stopSkinning();
	//void setAlphaValue(double a) {alpha = a; skinWidget->setWindowOpacity(alpha);}
public:
	//void			setSkinPath(const QString& skinPath);
	//QString			getSkinPath();
	//int				customFrameWidth();

	SkinObject(QWidget* wgtParent);
	~QSkinObject();
};

#endif // SKINOBJECT_H
