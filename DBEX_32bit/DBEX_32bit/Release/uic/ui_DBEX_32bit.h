/********************************************************************************
** Form generated from reading UI file 'DBEX_32bit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBEX_32BIT_H
#define UI_DBEX_32BIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBEX_32bitClass
{
public:
    QWidget *widget_2;
    QTextEdit *state_text;
    QPushButton *firstback_button;
    QPushButton *open_button;
    QWidget *widget_1_5;
    QLabel *label_10;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_17;
    QWidget *widget_1_2;
    QComboBox *tablename_combobox;
    QPushButton *table_button;
    QPushButton *field_button;
    QComboBox *field_combobox;
    QLabel *nowfield_label;
    QLabel *nowtable_label;
    QWidget *widget_7;
    QRadioButton *numeric;
    QRadioButton *nvarchar;
    QLabel *label_7;
    QComboBox *numeric_combobox;
    QComboBox *nvarchar_combobox;
    QLabel *label_8;
    QTextEdit *datatype_textbox;
    QWidget *widget_3;
    QWidget *widget_8;
    QRadioButton *null_yes;
    QRadioButton *null_no;
    QLabel *label_9;
    QWidget *widget_9;
    QRadioButton *up_yes;
    QRadioButton *up_no;
    QLabel *label_11;
    QWidget *widget_10;
    QLabel *label_14;
    QTextEdit *default_textbox;
    QWidget *widget_11;
    QLabel *label_16;
    QTextEdit *collation_textbox;
    QPushButton *optionsave_button;
    QWidget *widget_1_3;
    QComboBox *tablename_combobox3;
    QPushButton *table_button3;
    QTableWidget *table;
    QWidget *widget_6;
    QRadioButton *yes;
    QRadioButton *no;
    QLabel *label_5;
    QWidget *widget_1;
    QRadioButton *xml_to_db;
    QRadioButton *db_to_xml;
    QWidget *widget_12;
    QPushButton *savepath_button;
    QLabel *label_2;
    QPushButton *openfile_button;
    QLabel *state_label;
    QTextEdit *openfile_textbox;
    QTextEdit *savepath_textbox;
    QLabel *label;
    QWidget *upbar;
    QLabel *image;
    QFrame *line;
    QPushButton *next_button;
    QPushButton *before_button;
    QFrame *line_2;
    QWidget *widget_1_4;
    QComboBox *tablename_combobox2;
    QPushButton *table_button2;
    QLabel *nowtable_label2;
    QWidget *widget_5;
    QComboBox *fktable_combobox;
    QPushButton *fktable_button;
    QComboBox *fkfield_combobox;
    QTextEdit *deleterule_textbox;
    QTextEdit *correction_textbox;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *nowtable_label3;
    QComboBox *nowfield_combobox;
    QLabel *label_15;
    QLabel *label_18;
    QWidget *widget_4;
    QComboBox *pk_combobox;
    QLabel *label_19;
    QLabel *label_20;
    QComboBox *uk_combobox;
    QLabel *label_22;
    QLabel *label_23;
    QComboBox *condition_combobox;
    QTextEdit *condition_textbox;
    QLabel *label_24;
    QPushButton *option2_delete_button;
    QLabel *label_3;
    QTextEdit *rulename_textbox;
    QPushButton *option2_save_button;
    QTableWidget *table2;
    QLabel *label_4;

    void setupUi(QWidget *DBEX_32bitClass)
    {
        if (DBEX_32bitClass->objectName().isEmpty())
            DBEX_32bitClass->setObjectName(QString::fromUtf8("DBEX_32bitClass"));
        DBEX_32bitClass->resize(824, 487);
        widget_2 = new QWidget(DBEX_32bitClass);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setEnabled(true);
        widget_2->setGeometry(QRect(10, 80, 800, 350));
        widget_2->setBaseSize(QSize(0, 0));
        widget_2->setToolTipDuration(0);
        widget_2->setAutoFillBackground(false);
        state_text = new QTextEdit(widget_2);
        state_text->setObjectName(QString::fromUtf8("state_text"));
        state_text->setGeometry(QRect(70, 20, 671, 291));
        state_text->setReadOnly(true);
        firstback_button = new QPushButton(widget_2);
        firstback_button->setObjectName(QString::fromUtf8("firstback_button"));
        firstback_button->setGeometry(QRect(110, 320, 75, 24));
        open_button = new QPushButton(widget_2);
        open_button->setObjectName(QString::fromUtf8("open_button"));
        open_button->setGeometry(QRect(30, 320, 75, 24));
        widget_1_5 = new QWidget(DBEX_32bitClass);
        widget_1_5->setObjectName(QString::fromUtf8("widget_1_5"));
        widget_1_5->setGeometry(QRect(10, 80, 800, 350));
        label_10 = new QLabel(widget_1_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(120, 110, 101, 16));
        label_12 = new QLabel(widget_1_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(120, 170, 551, 16));
        label_13 = new QLabel(widget_1_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(120, 200, 591, 16));
        label_17 = new QLabel(widget_1_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(120, 140, 571, 16));
        widget_1_2 = new QWidget(DBEX_32bitClass);
        widget_1_2->setObjectName(QString::fromUtf8("widget_1_2"));
        widget_1_2->setEnabled(true);
        widget_1_2->setGeometry(QRect(10, 80, 800, 350));
        tablename_combobox = new QComboBox(widget_1_2);
        tablename_combobox->setObjectName(QString::fromUtf8("tablename_combobox"));
        tablename_combobox->setGeometry(QRect(30, 30, 291, 22));
        table_button = new QPushButton(widget_1_2);
        table_button->setObjectName(QString::fromUtf8("table_button"));
        table_button->setGeometry(QRect(330, 30, 91, 21));
        field_button = new QPushButton(widget_1_2);
        field_button->setObjectName(QString::fromUtf8("field_button"));
        field_button->setGeometry(QRect(330, 60, 91, 21));
        field_combobox = new QComboBox(widget_1_2);
        field_combobox->setObjectName(QString::fromUtf8("field_combobox"));
        field_combobox->setGeometry(QRect(30, 60, 291, 22));
        nowfield_label = new QLabel(widget_1_2);
        nowfield_label->setObjectName(QString::fromUtf8("nowfield_label"));
        nowfield_label->setGeometry(QRect(430, 60, 161, 16));
        nowtable_label = new QLabel(widget_1_2);
        nowtable_label->setObjectName(QString::fromUtf8("nowtable_label"));
        nowtable_label->setGeometry(QRect(430, 30, 161, 16));
        widget_7 = new QWidget(widget_1_2);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setGeometry(QRect(20, 90, 731, 71));
        numeric = new QRadioButton(widget_7);
        numeric->setObjectName(QString::fromUtf8("numeric"));
        numeric->setGeometry(QRect(140, 10, 91, 20));
        nvarchar = new QRadioButton(widget_7);
        nvarchar->setObjectName(QString::fromUtf8("nvarchar"));
        nvarchar->setGeometry(QRect(360, 10, 91, 20));
        nvarchar->setChecked(true);
        label_7 = new QLabel(widget_7);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 10, 121, 20));
        numeric_combobox = new QComboBox(widget_7);
        numeric_combobox->setObjectName(QString::fromUtf8("numeric_combobox"));
        numeric_combobox->setGeometry(QRect(210, 10, 131, 22));
        nvarchar_combobox = new QComboBox(widget_7);
        nvarchar_combobox->setObjectName(QString::fromUtf8("nvarchar_combobox"));
        nvarchar_combobox->setGeometry(QRect(440, 10, 131, 22));
        label_8 = new QLabel(widget_7);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 40, 71, 20));
        datatype_textbox = new QTextEdit(widget_7);
        datatype_textbox->setObjectName(QString::fromUtf8("datatype_textbox"));
        datatype_textbox->setEnabled(true);
        datatype_textbox->setGeometry(QRect(140, 40, 571, 20));
        datatype_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        datatype_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        datatype_textbox->setUndoRedoEnabled(false);
        datatype_textbox->setReadOnly(true);
        datatype_textbox->setAcceptRichText(false);
        widget_3 = new QWidget(widget_1_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(10, 160, 731, 141));
        widget_8 = new QWidget(widget_3);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        widget_8->setGeometry(QRect(20, 10, 281, 41));
        null_yes = new QRadioButton(widget_8);
        null_yes->setObjectName(QString::fromUtf8("null_yes"));
        null_yes->setGeometry(QRect(140, 10, 91, 20));
        null_no = new QRadioButton(widget_8);
        null_no->setObjectName(QString::fromUtf8("null_no"));
        null_no->setGeometry(QRect(190, 10, 91, 20));
        null_no->setChecked(true);
        label_9 = new QLabel(widget_8);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 121, 20));
        widget_9 = new QWidget(widget_3);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        widget_9->setGeometry(QRect(310, 10, 281, 41));
        up_yes = new QRadioButton(widget_9);
        up_yes->setObjectName(QString::fromUtf8("up_yes"));
        up_yes->setGeometry(QRect(140, 10, 91, 20));
        up_no = new QRadioButton(widget_9);
        up_no->setObjectName(QString::fromUtf8("up_no"));
        up_no->setGeometry(QRect(190, 10, 91, 20));
        up_no->setChecked(true);
        label_11 = new QLabel(widget_9);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 10, 121, 20));
        widget_10 = new QWidget(widget_3);
        widget_10->setObjectName(QString::fromUtf8("widget_10"));
        widget_10->setGeometry(QRect(20, 50, 711, 41));
        label_14 = new QLabel(widget_10);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 10, 121, 20));
        default_textbox = new QTextEdit(widget_10);
        default_textbox->setObjectName(QString::fromUtf8("default_textbox"));
        default_textbox->setEnabled(true);
        default_textbox->setGeometry(QRect(140, 10, 511, 20));
        default_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        default_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        default_textbox->setUndoRedoEnabled(false);
        default_textbox->setReadOnly(true);
        default_textbox->setAcceptRichText(false);
        widget_11 = new QWidget(widget_3);
        widget_11->setObjectName(QString::fromUtf8("widget_11"));
        widget_11->setGeometry(QRect(20, 90, 651, 41));
        label_16 = new QLabel(widget_11);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 10, 121, 20));
        collation_textbox = new QTextEdit(widget_11);
        collation_textbox->setObjectName(QString::fromUtf8("collation_textbox"));
        collation_textbox->setEnabled(true);
        collation_textbox->setGeometry(QRect(140, 10, 511, 20));
        collation_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        collation_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        collation_textbox->setUndoRedoEnabled(false);
        collation_textbox->setReadOnly(true);
        collation_textbox->setAcceptRichText(false);
        optionsave_button = new QPushButton(widget_1_2);
        optionsave_button->setObjectName(QString::fromUtf8("optionsave_button"));
        optionsave_button->setGeometry(QRect(630, 310, 141, 28));
        widget_1_3 = new QWidget(DBEX_32bitClass);
        widget_1_3->setObjectName(QString::fromUtf8("widget_1_3"));
        widget_1_3->setEnabled(true);
        widget_1_3->setGeometry(QRect(10, 80, 800, 350));
        tablename_combobox3 = new QComboBox(widget_1_3);
        tablename_combobox3->setObjectName(QString::fromUtf8("tablename_combobox3"));
        tablename_combobox3->setGeometry(QRect(50, 60, 131, 22));
        table_button3 = new QPushButton(widget_1_3);
        table_button3->setObjectName(QString::fromUtf8("table_button3"));
        table_button3->setGeometry(QRect(190, 60, 91, 21));
        table = new QTableWidget(widget_1_3);
        if (table->columnCount() < 4)
            table->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        table->setObjectName(QString::fromUtf8("table"));
        table->setGeometry(QRect(20, 100, 761, 191));
        widget_6 = new QWidget(widget_1_3);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setGeometry(QRect(50, 10, 301, 41));
        yes = new QRadioButton(widget_6);
        yes->setObjectName(QString::fromUtf8("yes"));
        yes->setGeometry(QRect(140, 10, 91, 20));
        no = new QRadioButton(widget_6);
        no->setObjectName(QString::fromUtf8("no"));
        no->setGeometry(QRect(200, 10, 91, 20));
        no->setChecked(true);
        label_5 = new QLabel(widget_6);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 10, 121, 20));
        widget_1 = new QWidget(DBEX_32bitClass);
        widget_1->setObjectName(QString::fromUtf8("widget_1"));
        widget_1->setGeometry(QRect(10, 80, 800, 350));
        xml_to_db = new QRadioButton(widget_1);
        xml_to_db->setObjectName(QString::fromUtf8("xml_to_db"));
        xml_to_db->setGeometry(QRect(70, 60, 311, 20));
        xml_to_db->setChecked(true);
        db_to_xml = new QRadioButton(widget_1);
        db_to_xml->setObjectName(QString::fromUtf8("db_to_xml"));
        db_to_xml->setGeometry(QRect(70, 120, 221, 20));
        widget_12 = new QWidget(widget_1);
        widget_12->setObjectName(QString::fromUtf8("widget_12"));
        widget_12->setGeometry(QRect(50, 170, 711, 121));
        savepath_button = new QPushButton(widget_12);
        savepath_button->setObjectName(QString::fromUtf8("savepath_button"));
        savepath_button->setGeometry(QRect(650, 20, 31, 24));
        label_2 = new QLabel(widget_12);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 71, 16));
        openfile_button = new QPushButton(widget_12);
        openfile_button->setObjectName(QString::fromUtf8("openfile_button"));
        openfile_button->setGeometry(QRect(650, 60, 31, 24));
        state_label = new QLabel(widget_12);
        state_label->setObjectName(QString::fromUtf8("state_label"));
        state_label->setGeometry(QRect(420, 90, 52, 16));
        openfile_textbox = new QTextEdit(widget_12);
        openfile_textbox->setObjectName(QString::fromUtf8("openfile_textbox"));
        openfile_textbox->setGeometry(QRect(90, 60, 541, 20));
        openfile_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        openfile_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        openfile_textbox->setUndoRedoEnabled(false);
        openfile_textbox->setReadOnly(true);
        openfile_textbox->setAcceptRichText(false);
        savepath_textbox = new QTextEdit(widget_12);
        savepath_textbox->setObjectName(QString::fromUtf8("savepath_textbox"));
        savepath_textbox->setEnabled(true);
        savepath_textbox->setGeometry(QRect(90, 20, 541, 20));
        savepath_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        savepath_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        savepath_textbox->setUndoRedoEnabled(false);
        savepath_textbox->setReadOnly(true);
        savepath_textbox->setAcceptRichText(false);
        label = new QLabel(widget_12);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 60, 61, 16));
        upbar = new QWidget(DBEX_32bitClass);
        upbar->setObjectName(QString::fromUtf8("upbar"));
        upbar->setGeometry(QRect(10, 0, 800, 80));
        image = new QLabel(upbar);
        image->setObjectName(QString::fromUtf8("image"));
        image->setGeometry(QRect(0, 0, 800, 80));
        line = new QFrame(DBEX_32bitClass);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(-40, 70, 900, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        next_button = new QPushButton(DBEX_32bitClass);
        next_button->setObjectName(QString::fromUtf8("next_button"));
        next_button->setGeometry(QRect(700, 450, 75, 24));
        before_button = new QPushButton(DBEX_32bitClass);
        before_button->setObjectName(QString::fromUtf8("before_button"));
        before_button->setGeometry(QRect(620, 450, 75, 24));
        line_2 = new QFrame(DBEX_32bitClass);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(0, 430, 900, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        widget_1_4 = new QWidget(DBEX_32bitClass);
        widget_1_4->setObjectName(QString::fromUtf8("widget_1_4"));
        widget_1_4->setEnabled(true);
        widget_1_4->setGeometry(QRect(10, 80, 800, 350));
        tablename_combobox2 = new QComboBox(widget_1_4);
        tablename_combobox2->setObjectName(QString::fromUtf8("tablename_combobox2"));
        tablename_combobox2->setGeometry(QRect(30, 10, 291, 22));
        table_button2 = new QPushButton(widget_1_4);
        table_button2->setObjectName(QString::fromUtf8("table_button2"));
        table_button2->setGeometry(QRect(330, 10, 91, 21));
        nowtable_label2 = new QLabel(widget_1_4);
        nowtable_label2->setObjectName(QString::fromUtf8("nowtable_label2"));
        nowtable_label2->setGeometry(QRect(430, 10, 161, 16));
        widget_5 = new QWidget(widget_1_4);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(20, 250, 771, 71));
        fktable_combobox = new QComboBox(widget_5);
        fktable_combobox->setObjectName(QString::fromUtf8("fktable_combobox"));
        fktable_combobox->setGeometry(QRect(320, 10, 161, 22));
        fktable_button = new QPushButton(widget_5);
        fktable_button->setObjectName(QString::fromUtf8("fktable_button"));
        fktable_button->setGeometry(QRect(490, 10, 71, 21));
        fkfield_combobox = new QComboBox(widget_5);
        fkfield_combobox->setObjectName(QString::fromUtf8("fkfield_combobox"));
        fkfield_combobox->setGeometry(QRect(570, 10, 191, 22));
        deleterule_textbox = new QTextEdit(widget_5);
        deleterule_textbox->setObjectName(QString::fromUtf8("deleterule_textbox"));
        deleterule_textbox->setEnabled(true);
        deleterule_textbox->setGeometry(QRect(70, 40, 161, 20));
        deleterule_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        deleterule_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        deleterule_textbox->setUndoRedoEnabled(false);
        deleterule_textbox->setReadOnly(true);
        deleterule_textbox->setAcceptRichText(false);
        correction_textbox = new QTextEdit(widget_5);
        correction_textbox->setObjectName(QString::fromUtf8("correction_textbox"));
        correction_textbox->setEnabled(true);
        correction_textbox->setGeometry(QRect(320, 40, 181, 20));
        correction_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        correction_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        correction_textbox->setUndoRedoEnabled(false);
        correction_textbox->setReadOnly(true);
        correction_textbox->setAcceptRichText(false);
        label_26 = new QLabel(widget_5);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(10, 40, 221, 20));
        label_27 = new QLabel(widget_5);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(260, 40, 71, 20));
        nowtable_label3 = new QLabel(widget_5);
        nowtable_label3->setObjectName(QString::fromUtf8("nowtable_label3"));
        nowtable_label3->setGeometry(QRect(570, 40, 141, 20));
        nowfield_combobox = new QComboBox(widget_5);
        nowfield_combobox->setObjectName(QString::fromUtf8("nowfield_combobox"));
        nowfield_combobox->setGeometry(QRect(70, 10, 161, 22));
        label_15 = new QLabel(widget_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 10, 41, 20));
        label_18 = new QLabel(widget_5);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(240, 10, 81, 20));
        widget_4 = new QWidget(widget_1_4);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(20, 160, 771, 71));
        pk_combobox = new QComboBox(widget_4);
        pk_combobox->setObjectName(QString::fromUtf8("pk_combobox"));
        pk_combobox->setGeometry(QRect(90, 10, 201, 22));
        label_19 = new QLabel(widget_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(20, 10, 64, 15));
        label_20 = new QLabel(widget_4);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(20, 40, 64, 15));
        uk_combobox = new QComboBox(widget_4);
        uk_combobox->setObjectName(QString::fromUtf8("uk_combobox"));
        uk_combobox->setGeometry(QRect(90, 40, 201, 22));
        label_22 = new QLabel(widget_4);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(330, 10, 64, 15));
        label_23 = new QLabel(widget_4);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(370, 10, 64, 15));
        condition_combobox = new QComboBox(widget_4);
        condition_combobox->setObjectName(QString::fromUtf8("condition_combobox"));
        condition_combobox->setGeometry(QRect(420, 10, 221, 22));
        condition_textbox = new QTextEdit(widget_4);
        condition_textbox->setObjectName(QString::fromUtf8("condition_textbox"));
        condition_textbox->setEnabled(true);
        condition_textbox->setGeometry(QRect(420, 40, 221, 20));
        condition_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        condition_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        condition_textbox->setUndoRedoEnabled(false);
        condition_textbox->setReadOnly(true);
        condition_textbox->setAcceptRichText(false);
        label_24 = new QLabel(widget_4);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(370, 40, 64, 15));
        option2_delete_button = new QPushButton(widget_1_4);
        option2_delete_button->setObjectName(QString::fromUtf8("option2_delete_button"));
        option2_delete_button->setGeometry(QRect(680, 10, 91, 21));
        label_3 = new QLabel(widget_1_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(290, 320, 121, 16));
        rulename_textbox = new QTextEdit(widget_1_4);
        rulename_textbox->setObjectName(QString::fromUtf8("rulename_textbox"));
        rulename_textbox->setEnabled(true);
        rulename_textbox->setGeometry(QRect(380, 320, 251, 20));
        rulename_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rulename_textbox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rulename_textbox->setUndoRedoEnabled(false);
        rulename_textbox->setReadOnly(true);
        rulename_textbox->setAcceptRichText(false);
        option2_save_button = new QPushButton(widget_1_4);
        option2_save_button->setObjectName(QString::fromUtf8("option2_save_button"));
        option2_save_button->setGeometry(QRect(640, 320, 141, 28));
        table2 = new QTableWidget(widget_1_4);
        if (table2->columnCount() < 10)
            table2->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(5, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(6, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(7, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(8, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(9, __qtablewidgetitem13);
        table2->setObjectName(QString::fromUtf8("table2"));
        table2->setGeometry(QRect(30, 40, 751, 121));
        label_4 = new QLabel(widget_1_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 240, 64, 15));
        widget_1->raise();
        widget_2->raise();
        widget_1_5->raise();
        line->raise();
        widget_1_3->raise();
        widget_1_2->raise();
        widget_1_4->raise();
        upbar->raise();
        next_button->raise();
        before_button->raise();
        line_2->raise();

        retranslateUi(DBEX_32bitClass);

        QMetaObject::connectSlotsByName(DBEX_32bitClass);
    } // setupUi

    void retranslateUi(QWidget *DBEX_32bitClass)
    {
        DBEX_32bitClass->setWindowTitle(QCoreApplication::translate("DBEX_32bitClass", "DBEX_32bit", nullptr));
        firstback_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\262\230\354\235\214\354\234\274\353\241\234", nullptr));
        open_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\217\264\353\215\224 \354\227\264\352\270\260", nullptr));
        label_10->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\263\200\355\231\230\355\225\230\352\270\260 \354\240\204\354\227\220..", nullptr));
        label_12->setText(QCoreApplication::translate("DBEX_32bitClass", "*\355\225\250\354\210\230 \354\213\235 \354\240\200\354\236\245\354\227\220\354\204\234 \354\225\204\353\213\210\354\230\244\353\245\274 \354\204\240\355\203\235\355\225\230\354\205\250\353\213\244\353\251\264 \355\225\250\354\210\230 \352\262\260\352\263\274\352\260\222\353\247\214 \353\215\260\354\235\264\355\204\260\353\241\234 \354\240\200\354\236\245\353\220\251\353\213\210\353\213\244.", nullptr));
        label_13->setText(QCoreApplication::translate("DBEX_32bitClass", "*\354\231\234\353\236\230\355\202\244\353\241\234 \354\204\240\355\203\235\355\225\234 \354\271\274\353\237\274\352\263\274 \352\260\231\354\235\200 \353\202\264\354\232\251\354\235\204 \352\260\200\354\247\204 \354\271\274\353\237\274\354\235\264 \354\227\260\352\262\260\353\220\234 \355\205\214\354\235\264\353\270\224\354\227\220 \354\241\264\354\236\254\355\225\264\354\225\274 \355\225\251\353\213\210\353\213\244.", nullptr));
        label_17->setText(QCoreApplication::translate("DBEX_32bitClass", "*\352\270\260\353\263\270\355\202\244\353\212\224 \353\263\204\353\217\204\354\235\230 \354\210\230\354\240\225\354\235\264 \354\227\206\354\234\274\353\251\264 \354\247\200\354\240\225\353\220\230\354\247\200 \354\225\212\354\212\265\353\213\210\353\213\244.", nullptr));
        table_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\205\214\354\235\264\353\270\224 \354\204\240\355\203\235", nullptr));
        field_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\225\204\353\223\234 \354\204\240\355\203\235", nullptr));
        nowfield_label->setText(QString());
        nowtable_label->setText(QString());
        numeric->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\210\253\354\236\220\355\230\225", nullptr));
        nvarchar->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\254\270\354\236\220\355\230\225", nullptr));
        label_7->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\215\260\354\235\264\355\204\260\355\203\200\354\236\205", nullptr));
        label_8->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\236\220\353\246\254\354\210\230(n)", nullptr));
        null_yes->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\204\244", nullptr));
        null_no->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\225\204\353\213\210\354\230\244", nullptr));
        label_9->setText(QCoreApplication::translate("DBEX_32bitClass", "null", nullptr));
        up_yes->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\204\244", nullptr));
        up_no->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\225\204\353\213\210\354\230\244", nullptr));
        label_11->setText(QCoreApplication::translate("DBEX_32bitClass", " \354\236\220\353\217\231\354\246\235\352\260\200", nullptr));
        label_14->setText(QCoreApplication::translate("DBEX_32bitClass", "\352\270\260\353\263\270\352\260\222", nullptr));
        label_16->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\275\234\353\240\210\354\235\264\354\205\230", nullptr));
        optionsave_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\204\244\354\240\225\352\260\222 \354\240\200\354\236\245", nullptr));
        table_button3->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\205\214\354\235\264\353\270\224 \354\204\240\355\203\235", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\227\264 \354\235\264\353\246\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\215\260\354\235\264\355\204\260\355\203\200\354\236\205", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("DBEX_32bitClass", "null", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\236\220\353\217\231\354\246\235\352\260\200", nullptr));
        yes->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\204\244", nullptr));
        no->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\225\204\353\213\210\354\230\244", nullptr));
        label_5->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\252\250\353\223\240 \355\225\250\354\210\230 \354\213\235 \354\240\200\354\236\245", nullptr));
        xml_to_db->setText(QCoreApplication::translate("DBEX_32bitClass", "Xml\354\235\204 DB\353\241\234 \354\240\204\355\231\230\355\225\230\352\270\260", nullptr));
        db_to_xml->setText(QCoreApplication::translate("DBEX_32bitClass", "DB\353\245\274 Xml\353\241\234 \354\240\204\355\231\230\355\225\230\352\270\260", nullptr));
        savepath_button->setText(QCoreApplication::translate("DBEX_32bitClass", "..", nullptr));
        label_2->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\240\200\354\236\245\352\262\275\353\241\234", nullptr));
        openfile_button->setText(QCoreApplication::translate("DBEX_32bitClass", "..", nullptr));
        state_label->setText(QString());
        label->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\240\204\355\231\230\355\214\214\354\235\274", nullptr));
        image->setText(QString());
        next_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\353\213\244\354\235\214", nullptr));
        before_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\235\264\354\240\204", nullptr));
        table_button2->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\205\214\354\235\264\353\270\224 \354\204\240\355\203\235", nullptr));
        nowtable_label2->setText(QString());
        fktable_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\204\240\355\203\235", nullptr));
        label_26->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\202\255\354\240\234\352\267\234\354\271\231", nullptr));
        label_27->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\210\230\354\240\225\352\267\234\354\271\231", nullptr));
        nowtable_label3->setText(QString());
        label_15->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\225\204\353\223\234", nullptr));
        label_18->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\227\260\352\262\260\355\205\214\354\235\264\353\270\224", nullptr));
        label_19->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\243\274\354\232\224\355\202\244", nullptr));
        label_20->setText(QCoreApplication::translate("DBEX_32bitClass", "\352\263\240\354\234\240\355\202\244", nullptr));
        label_22->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264", nullptr));
        label_23->setText(QCoreApplication::translate("DBEX_32bitClass", "\355\225\204\353\223\234", nullptr));
        label_24->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264\354\213\235", nullptr));
        option2_delete_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264\354\202\255\354\240\234", nullptr));
        label_3->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\240\234\354\225\275\354\241\260\352\261\264\353\252\205", nullptr));
        option2_save_button->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264\354\266\224\352\260\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table2->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\240\234\354\225\275\354\241\260\352\261\264\353\252\205", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table2->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\243\274\354\232\224\355\202\244", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table2->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("DBEX_32bitClass", "\352\263\240\354\234\240\355\202\244", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table2->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264 \355\225\204\353\223\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table2->horizontalHeaderItem(4);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\241\260\352\261\264\354\213\235", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table2->horizontalHeaderItem(5);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\231\270\353\236\230\355\202\244", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table2->horizontalHeaderItem(6);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\260\270\354\241\260\355\205\214\354\235\264\353\270\224", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table2->horizontalHeaderItem(7);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\260\270\354\241\260\355\225\204\353\223\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table2->horizontalHeaderItem(8);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\202\255\354\240\234\352\267\234\354\271\231", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table2->horizontalHeaderItem(9);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\210\230\354\240\225\352\267\234\354\271\231", nullptr));
        label_4->setText(QCoreApplication::translate("DBEX_32bitClass", "\354\231\270\353\236\230\355\202\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBEX_32bitClass: public Ui_DBEX_32bitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBEX_32BIT_H