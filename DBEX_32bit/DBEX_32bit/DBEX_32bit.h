#pragma warning(disable: 4996)

#include <QtWidgets/QWidget>
#include "ui_DBEX_32bit.h"
#include "Function.hpp"
#include "excelWriter.hpp"
#include "DBConverter.hpp"
#include "ExcelConverter.hpp"
#include "typedef.hpp"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qdesktopservices.h>
#include <qprocess.h>
#include <qobject.h>
#include <stdio.h>
#include <QStyledItemDelegate>
#include <io.h>
#include <qthread.h>
#include <thread>
#include <filesystem>

class DBEX_32bit : public QWidget
{
    Q_OBJECT

public:
    DBEX_32bit(QWidget *parent = Q_NULLPTR);
    struct Option;

private:
    Ui::DBEX_32bitClass ui;


private slots:
    void DBEX_32bit::NextButtonClick();
    void DBEX_32bit::BeforeButtonClick();
    void DBEX_32bit::OpendialogInputButtonClicked();
    void DBEX_32bit::OpendialogOutputButtonClicked();
    void DBEX_32bit::FunctionStart();
    void DBEX_32bit::Option_save();
    void DBEX_32bit::Table_show();
    void DBEX_32bit::Option_set();
    void DBEX_32bit::Option1_tablecombobox_change();
    void DBEX_32bit::FK_tablechange();
    void DBEX_32bit::Option1_fieldcombobox_change();
    void DBEX_32bit::LabelChange();
    void DBEX_32bit::FileOpen();
    void DBEX_32bit::Firstback();
    void DBEX_32bit::Option2_combobox_change();
    void DBEX_32bit::Option2_save();
    void DBEX_32bit::Option2_delete();
    void DBEX_32bit::Option2_table_show();
    std::string DBEX_32bit::tostring(QString input);
    QString DBEX_32bit::fromstring(std::string input);
};
