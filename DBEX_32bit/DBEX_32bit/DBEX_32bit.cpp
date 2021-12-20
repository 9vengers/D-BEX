#include "DBEX_32bit.h"

#define toKor(str) QString::fromLocal8Bit(str)
struct SchemaStruct;
struct RuleStruct;
struct FK;

bool isFunctionSave;
std::map<std::string, std::map<std::string, SchemaStruct>> schema_map; //테이블이름, 키 이름, 옵션
std::map <std::string, std::vector<RuleStruct>> rule_map; //테이블 이름, 제약조건 구조체

DBEX_32bit::DBEX_32bit(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QWidget* widget_1 = ui.widget_1;
	QWidget* widget_2 = ui.widget_2;
	QWidget* widget_1_2 = ui.widget_1_2;
	QWidget* widget_1_3 = ui.widget_1_3;
	QWidget* widget_1_4 = ui.widget_1_4;
	QWidget* widget_1_5 = ui.widget_1_5;

	ui.before_button->setVisible(false);
	ui.nowtable_label3->setVisible(false);

	ui.datatype_textbox->setReadOnly(false);
	ui.default_textbox->setReadOnly(false);
	ui.collation_textbox->setReadOnly(false);
	ui.condition_textbox->setReadOnly(false);
	ui.deleterule_textbox->setReadOnly(false);
	ui.correction_textbox->setReadOnly(false);
	ui.rulename_textbox->setReadOnly(false);

	QPixmap image = (".\\images\\gui_1.png");
	ui.image->setPixmap(image.scaled(800, 80));

	widget_2->setVisible(false);
	widget_1_2->setVisible(false);
	widget_1_3->setVisible(false);
	widget_1_4->setVisible(false);
	widget_1_5->setVisible(false);

	QObject::connect(ui.next_button, &QPushButton::clicked, this, &DBEX_32bit::NextButtonClick);
	QObject::connect(ui.before_button, &QPushButton::clicked, this, &DBEX_32bit::BeforeButtonClick);
	QObject::connect(ui.savepath_button, &QPushButton::clicked, this, &DBEX_32bit::OpendialogInputButtonClicked); //&output_path, false
	QObject::connect(ui.openfile_button, &QPushButton::clicked, this, &DBEX_32bit::OpendialogOutputButtonClicked); //&input_path, true
	QObject::connect(ui.table_button, &QPushButton::clicked, this, &DBEX_32bit::Option1_tablecombobox_change);
	QObject::connect(ui.option2_save_button, &QPushButton::clicked, this, &DBEX_32bit::Option2_save);
	QObject::connect(ui.option2_delete_button, &QPushButton::clicked, this, &DBEX_32bit::Option2_delete);
	QObject::connect(ui.optionsave_button, &QPushButton::clicked, this, &DBEX_32bit::Option_save);
	QObject::connect(ui.field_button, &QPushButton::clicked, this, &DBEX_32bit::Option1_fieldcombobox_change);
	QObject::connect(ui.fktable_button, &QPushButton::clicked, this, &DBEX_32bit::FK_tablechange);
	QObject::connect(ui.table_button2, &QPushButton::clicked, this, &DBEX_32bit::Option2_combobox_change);
	QObject::connect(ui.table_button3, &QPushButton::clicked, this, &DBEX_32bit::Table_show);
	QObject::connect(ui.open_button, &QPushButton::clicked, this, &DBEX_32bit::FileOpen);
	QObject::connect(ui.firstback_button, &QPushButton::clicked, this, &DBEX_32bit::Firstback);

}

std::string DBEX_32bit::tostring(QString input){

		return input.toUtf8().constData();
}

QString DBEX_32bit::fromstring(std::string input) {
	return QString::fromLatin1(input.c_str());
}

void DBEX_32bit::NextButtonClick() {

	if (ui.widget_1->isVisible()) {

		QString savepath_qstr = ui.savepath_textbox->toPlainText();
		QString openfile_qstr = ui.openfile_textbox->toPlainText();

		if (!savepath_qstr.isEmpty() && !openfile_qstr.isEmpty()){

			if (ui.db_to_xml->isChecked()) {
				ui.widget_2->setVisible(true);
				ui.widget_1->setVisible(false);
				FunctionStart();
				ui.next_button->setVisible(false);
				ui.before_button->setVisible(false);
				QPixmap image = (".\\images\\gui_2.png");
				ui.image->setPixmap(image.scaled(800, 80));
			}
			else if (ui.xml_to_db->isChecked()) {
				QPixmap image = (".\\images\\gui_1_2.png");
				ui.image->setPixmap(image.scaled(800, 80));
				// auto thread = QThread::create(LabelChange);

				// thread->start();
				// thread->terminate();
				
				schema_map = table_list(tostring(openfile_qstr), tostring(savepath_qstr));

				Option_set();
				ui.widget_1_2->setVisible(true);
				ui.widget_1->setVisible(false);
				ui.before_button->setVisible(true);

			}
		}
		else {
			QMessageBox::information(this, toKor("경고창"), toKor("빈칸이 존재합니다!"));;
		}
	}

	else if (ui.widget_1_2->isVisible()) {
		QPixmap image = (".\\images\\gui_1_3.png");
		ui.image->setPixmap(image.scaled(800, 80));
		ui.widget_1_2->setVisible(false);
		ui.widget_1_3->setVisible(true);
		Table_show();
	}

	else if (ui.widget_1_3->isVisible()) {

		QPixmap image = (".\\images\\gui_1_4.png");
		ui.image->setPixmap(image.scaled(800, 80));
		ui.widget_1_3->setVisible(false);
		ui.widget_1_4->setVisible(true);
		Option2_table_show();

	}

	else if (ui.widget_1_4->isVisible()) {
		QPixmap image = (".\\images\\gui_1_5.png");
		ui.image->setPixmap(image.scaled(800, 80));

		ui.widget_1_4->setVisible(false);
		ui.widget_1_5->setVisible(true);
	}


	else if (ui.widget_1_5->isVisible()) {
		QPixmap image = (".\\images\\gui_2.png");
		ui.image->setPixmap(image.scaled(800, 80));
		ui.widget_1_5->setVisible(false);
		ui.widget_2->setVisible(true);
		if (ui.yes->isChecked()) isFunctionSave = true;
		else if (ui.no->isChecked()) isFunctionSave = false;
		FunctionStart();
		ui.next_button->setVisible(false);
		ui.before_button->setVisible(false);
	}

}

void DBEX_32bit::LabelChange() {
	ui.state_label->setText("기본 옵션 설정중..");
}

void DBEX_32bit::BeforeButtonClick() {

	if (ui.widget_1_2->isVisible()) {
		ui.widget_1_2->setVisible(false);
		ui.widget_1->setVisible(true);
		ui.before_button->setVisible(false);
	}

	else if (ui.widget_1_3->isVisible()) {
		ui.widget_1_3->setVisible(false);
		ui.widget_1_2->setVisible(true);
		Option_set();
	}

	else if (ui.widget_1_4->isVisible()) {
		ui.widget_1_4->setVisible(false);
		ui.widget_1_3->setVisible(true);
	}

	else if (ui.widget_1_5->isVisible()) {
		ui.widget_1_5->setVisible(false);
		ui.widget_1_4->setVisible(true);
	}
}


void DBEX_32bit::Option_save() {

	std::string now_table = tostring(ui.nowtable_label->text());
	std::string now_field = tostring(ui.nowfield_label->text());

	if (ui.numeric->isChecked()) schema_map[now_table][now_field].datatype = tostring(ui.numeric_combobox->currentText());
	else if (ui.nvarchar->isChecked()) schema_map[now_table][now_field].datatype = tostring(ui.nvarchar_combobox->currentText());

	schema_map[now_table][now_field].datatype_2 = tostring(ui.datatype_textbox->toPlainText());
	schema_map[now_table][now_field].default_value = tostring(ui.default_textbox->toPlainText());
	schema_map[now_table][now_field].collation = tostring(ui.collation_textbox->toPlainText());

	schema_map[now_table][now_field].null = ui.null_yes->isChecked();
	schema_map[now_table][now_field].up = ui.up_yes->isChecked();

}

void DBEX_32bit::Option2_save(){
	std::string now_table = tostring(ui.nowtable_label2->text());
	RuleStruct now_rule;

	now_rule.rule_name= tostring(ui.rulename_textbox->toPlainText());
	now_rule.pk = tostring(ui.pk_combobox->currentText());
	now_rule.uk = tostring(ui.uk_combobox->currentText());
	now_rule.condition = *(new pair(tostring(ui.condition_combobox->currentText()), tostring(ui.condition_textbox->toPlainText())));

	FK now_fk;
	now_fk.fk=tostring(ui.nowfield_combobox->currentText());
	now_fk.fk_table=tostring(ui.nowtable_label3->text());
	now_fk.fk_field = tostring(ui.fkfield_combobox->currentText());

	if (ui.deleterule_textbox->toPlainText() != "") now_fk.delete_rule = tostring(ui.deleterule_textbox->toPlainText());
	if (ui.correction_textbox->toPlainText() != "") now_fk.correction_rule = tostring(ui.correction_textbox->toPlainText());

	now_rule.fk = now_fk;
	rule_map[now_table].push_back(now_rule);

	Option2_table_show();

	ui.rulename_textbox->clear();
	ui.deleterule_textbox->clear();
	ui.correction_textbox->clear();
	ui.rulename_textbox->clear();

	ui.pk_combobox->setCurrentText("");
	ui.uk_combobox->setCurrentText("");
	ui.condition_combobox->setCurrentText("");
	ui.fktable_combobox->setCurrentText("");
	ui.nowfield_combobox->setCurrentText("");

}

void DBEX_32bit::Option2_delete() {
	std::string now_table = tostring(ui.nowtable_label2->text());
	QModelIndex now_cell = ui.table2->selectionModel()->currentIndex();
	int index=now_cell.row();

	rule_map[now_table].erase(rule_map[now_table].begin()+index);
	
	Option2_table_show();


}

void DBEX_32bit::Option_set() {
	
	//option1 set
	ui.tablename_combobox->clear();
	ui.tablename_combobox2->clear();
	ui.tablename_combobox3->clear();
	ui.numeric_combobox->clear();
	ui.nvarchar_combobox->clear();
	ui.field_combobox->clear();
	ui.datatype_textbox->clear();
	ui.default_textbox->clear();
	ui.collation_textbox->clear();

	//option2 set
	ui.pk_combobox->clear();
	ui.uk_combobox->clear();
	ui.condition_combobox->clear();
	ui.condition_textbox->clear();
	ui.fktable_combobox->clear();
	ui.fkfield_combobox->clear();
	ui.deleterule_textbox->clear();
	ui.collation_textbox->clear();
	ui.nowfield_combobox->clear();

	schema_map.erase("DataSchema");
	ui.numeric_combobox->addItem("NUMERIC");
	ui.numeric_combobox->addItem("DECIMAL");
	ui.numeric_combobox->addItem("SMALLINT");
	ui.numeric_combobox->addItem("INTEGER");
	ui.numeric_combobox->addItem("BIGINT");
	ui.numeric_combobox->addItem("FLOAT");
	ui.numeric_combobox->addItem("DATETIME");
	ui.nvarchar_combobox->addItem("NVARCHAR");
	ui.nvarchar_combobox->addItem("CHAR");

	auto iter = schema_map.begin();
	ui.nowtable_label->setText(fromstring(iter->first));
	ui.nowtable_label2->setText(fromstring(iter->first));

	while (iter != schema_map.end()) { //테이블 순환

		std::vector<RuleStruct> now_vector;
		rule_map.insert(make_pair(" ", now_vector));
		ui.tablename_combobox->addItem(fromstring(iter->first));
		ui.tablename_combobox2->addItem(fromstring(iter->first));
		ui.tablename_combobox3->addItem(fromstring(iter->first));
		ui.fktable_combobox->addItem(fromstring(iter->first));
		if (iter == schema_map.begin()) { //필드 순환
			auto iter2 = iter->second.begin();
			QString now = QString::fromLatin1((iter2->first).c_str()); 
			ui.nowfield_label->setText(now);
			while (iter2 != iter->second.end()) {
				if(iter2->second.pk) ui.pk_combobox->addItem(fromstring(iter2->first)); //여기서 문제
				ui.uk_combobox->addItem(fromstring(iter2->first));
				ui.condition_combobox->addItem(fromstring(iter2->first));
				ui.field_combobox->addItem(fromstring(iter2->first));
				ui.nowfield_combobox->addItem(fromstring(iter2->first));
				++iter2;
			}
		}

		++iter;
	}
	ui.pk_combobox->addItem("");
	ui.pk_combobox->setCurrentText("");

	ui.uk_combobox->addItem("");
	ui.uk_combobox->setCurrentText("");

	ui.condition_combobox->addItem("");
	ui.condition_combobox->setCurrentText("");

	ui.fktable_combobox->addItem("");
	ui.fktable_combobox->setCurrentText("");

	ui.nowfield_combobox->addItem("");
	ui.nowfield_combobox->setCurrentText("");

}

void DBEX_32bit::Option1_tablecombobox_change() {

	ui.field_combobox->clear();
	ui.datatype_textbox->clear();
	ui.default_textbox->clear();
	ui.collation_textbox->clear();

	QString now_table = ui.tablename_combobox->currentText();
	ui.nowtable_label->setText(now_table);


	auto iter = schema_map[tostring(now_table)].begin();
	QString now_field = fromstring(iter->first);
	ui.nowfield_label->setText(now_field);

	if (iter->second.datatype == "NVARCHAR" || (iter->second.datatype == "CHAR")) {
		ui.numeric->setChecked(false);
		ui.nvarchar->setChecked(true);
		ui.numeric_combobox->setCurrentIndex(0);
		ui.nvarchar_combobox->setCurrentText(fromstring(iter->second.datatype));
	}
	else {
		ui.numeric->setChecked(true);
		ui.nvarchar->setChecked(false);
		ui.numeric_combobox->setCurrentText(fromstring(iter->second.datatype));
		ui.nvarchar_combobox->setCurrentIndex(0);
	}

	if (iter->second.datatype_2 != "") {
		ui.datatype_textbox->setText(fromstring(iter->second.datatype_2));
	}

	if (iter->second.null) {
		ui.null_yes->setChecked(true);
		ui.null_no->setChecked(false);
	}
	else {
		ui.null_yes->setChecked(false);
		ui.null_no->setChecked(true);
	}

	if (iter->second.up) {
		ui.up_yes->setChecked(true);
		ui.up_no->setChecked(false);
	}
	else {
		ui.up_yes->setChecked(false);
		ui.up_no->setChecked(true);
	}

	if (iter->second.default_value != "") {
		ui.default_textbox->setText(fromstring(iter->second.default_value));
	}

	if (iter->second.collation != "") {
		ui.collation_textbox->setText(fromstring(iter->second.collation));
	}

	while (iter != schema_map[tostring(now_table)].end()) {

		ui.field_combobox->addItem(fromstring(iter->first));

		++iter;
	}


}

void DBEX_32bit::Table_show() {

	ui.table->setRowCount(0);

	QString table = ui.tablename_combobox3->currentText();

	auto iter = schema_map[tostring(table)].begin();
	while (iter != schema_map[tostring(table)].end()) {

		ui.table->insertRow(ui.table->rowCount());
		//열이름
		ui.table->setItem(ui.table->rowCount() - 1, 0, new QTableWidgetItem(fromstring(iter->first)));
		//데이터타입
		if(iter->second.datatype_2!="") ui.table->setItem(ui.table->rowCount() - 1, 1, new QTableWidgetItem(fromstring(iter->second.datatype+"("+iter->second.datatype_2+")")));
		else ui.table->setItem(ui.table->rowCount() - 1, 1, new QTableWidgetItem(fromstring(iter->second.datatype)));
		//null
		if (iter->second.null) ui.table->setItem(ui.table->rowCount() - 1, 2, new QTableWidgetItem("true"));
		else ui.table->setItem(ui.table->rowCount() - 1, 2, new QTableWidgetItem("false"));
		//자동증가
		if (iter->second.up) ui.table->setItem(ui.table->rowCount() - 1, 3, new QTableWidgetItem("true"));
		else ui.table->setItem(ui.table->rowCount() - 1, 3, new QTableWidgetItem("false"));

		++iter;
	}

}

void DBEX_32bit::Option2_table_show() {

	ui.table2->setRowCount(0);

	QString table2 = ui.nowtable_label2->text();

	auto iter = rule_map[tostring(table2)].begin();
	while (iter != rule_map[tostring(table2)].end()) {

		ui.table2->insertRow(ui.table2->rowCount());
		ui.table2->setItem(ui.table2->rowCount() - 1, 0, new QTableWidgetItem(fromstring(iter->rule_name)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 1, new QTableWidgetItem(fromstring(iter->pk)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 2, new QTableWidgetItem(fromstring(iter->uk)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 3, new QTableWidgetItem(fromstring(iter->condition.first)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 4, new QTableWidgetItem(fromstring(iter->condition.second)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 5, new QTableWidgetItem(fromstring(iter->fk.fk)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 6, new QTableWidgetItem(fromstring(iter->fk.fk_table)));
		ui.table2->setItem(ui.table2->rowCount() - 1, 7, new QTableWidgetItem(fromstring(iter->fk.fk_field)));

		++iter;
	}

}

void DBEX_32bit::FK_tablechange() {
	ui.nowtable_label3->setText(ui.fktable_combobox->currentText());
	ui.fkfield_combobox->clear();

	std::string now_table = tostring(ui.nowtable_label3->text());

	auto iter = schema_map[now_table].begin();
	while (iter != schema_map[now_table].end()) {

		ui.fkfield_combobox->addItem(fromstring(iter->first));

		++iter;
	}
}

void DBEX_32bit::Option1_fieldcombobox_change() {

	ui.datatype_textbox->clear();
	ui.default_textbox->clear();
	ui.collation_textbox->clear();

	ui.nowfield_label->setText(ui.field_combobox->currentText());
	std::string now_table = tostring(ui.nowtable_label->text());
	std::string now_field = tostring(ui.field_combobox->currentText());


	schema_map[now_table][now_field];

	if (schema_map[now_table][now_field].datatype == "NVARCHAR" || schema_map[now_table][now_field].datatype == "CHAR") {
		ui.numeric->setChecked(false);
		ui.nvarchar->setChecked(true);
		ui.numeric_combobox->setCurrentIndex(0);
		ui.nvarchar_combobox->setCurrentText(fromstring(now_field));
	}
	else {
		ui.numeric->setChecked(true);
		ui.nvarchar->setChecked(false);
		ui.numeric_combobox->setCurrentText(fromstring(now_field));
		ui.nvarchar_combobox->setCurrentIndex(0);
	}

	if (schema_map[now_table][now_field].datatype_2 != "") {
		ui.datatype_textbox->setText(fromstring(schema_map[now_table][now_field].datatype_2));
	}

	if (schema_map[now_table][now_field].null) {
		ui.null_yes->setChecked(true);
		ui.null_no->setChecked(false);
	}
	else {
		ui.null_yes->setChecked(false);
		ui.null_no->setChecked(true);
	}

	if (schema_map[now_table][now_field].up) {
		ui.up_yes->setChecked(true);
		ui.up_no->setChecked(false);
	}
	else {
		ui.up_yes->setChecked(false);
		ui.up_no->setChecked(true);
	}

	if (schema_map[now_table][now_field].default_value != "") {
		ui.default_textbox->setText(fromstring(schema_map[now_table][now_field].default_value));
	}

	if (schema_map[now_table][now_field].collation != "") {
		ui.collation_textbox->setText(fromstring(schema_map[now_table][now_field].collation));
	}

}


void DBEX_32bit::OpendialogInputButtonClicked()
{
	QString path = QFileDialog::getExistingDirectory(this, toKor("경로 선택"), QDir::currentPath(), QFileDialog::ShowDirsOnly);

	ui.savepath_textbox->setText(path);

}

void DBEX_32bit::OpendialogOutputButtonClicked()
{
	QString path = QFileDialog::getOpenFileName(this, toKor("파일 선택"), "C:\\", "Files (*.*)");

	ui.openfile_textbox->setText(path);
}

void DBEX_32bit::FunctionStart() {

	ui.open_button->setVisible(false);
	ui.firstback_button->setVisible(false);
	ui.state_text->clear();

	std::string savepath = tostring(ui.savepath_textbox->toPlainText());
	std::string openfile = tostring(ui.openfile_textbox->toPlainText());

	std::string functionjson;
	bool functionjson_exist = false;

	for (int x = openfile.length() - 1; x >= 0; x--) {
		if (openfile[x] == '.') {
			functionjson = openfile.substr(0, x) + "Functions.json";
		}
	}


	std::string filepath;
	for (int x = 0; x < openfile.size(); x++) {
		if (openfile[x] == '/')
		{
			filepath += "\\";
		}
		else filepath += openfile[x];
	}
	std::string savepath_;
	for (int x = 0; x < savepath.size(); x++) {
		if (savepath[x] == '/')
		{
			savepath_ += "\\";
		}
		else savepath_ += savepath[x];
	}



	if (ui.xml_to_db->isChecked()) {
		ui.state_text->append(toKor("XML에서 DB로 변환합니다."));

		
		if (ExceltoDB(filepath, savepath, isFunctionSave, make_pair(schema_map, rule_map)) == -1)
		{
			ui.state_text->append(toKor("에러발생"));
		}
		
		ui.state_text->append(toKor("변환을 완료했습니다."));
	}

	if (ui.db_to_xml->isChecked()) {
		ui.state_text->append(toKor("DB에서 XML로 변환합니다."));
		if (DBtoExcel(filepath, savepath, isFunctionSave) == -1)
		{
			ui.state_text->append(toKor("에러발생"));
		}
		ui.state_text->append(toKor("변환을 완료했습니다."));

	}


	ui.open_button->setVisible(true);
	ui.firstback_button->setVisible(true);
	schema_map.clear();
	rule_map.clear();
}

void DBEX_32bit::FileOpen() {
	//임시값. 이후에 수정해 주어야 함

	std::string savepath = tostring(ui.savepath_textbox->toPlainText());
	std::string openfile = tostring(ui.openfile_textbox->toPlainText());

	std::string savepath_;
	for (int x = 0; x < savepath.size(); x++) {
		if (savepath[x] == '/')
		{
			savepath_ += "\\";
		}
		else savepath_ += savepath[x];
	}

	QDesktopServices::openUrl(QUrl(fromstring(savepath), QUrl::TolerantMode));
}
void DBEX_32bit::Firstback() {
	ui.widget_1->setVisible(true);
	ui.widget_2->setVisible(false);

	ui.next_button->setVisible(true);
}

void DBEX_32bit::Option2_combobox_change() {

	ui.pk_combobox->clear();
	ui.uk_combobox->clear();
	ui.condition_combobox->clear();
	ui.condition_textbox->clear();
	ui.fkfield_combobox->clear();
	ui.deleterule_textbox->clear();
	ui.collation_textbox->clear();
	ui.nowfield_combobox->clear();
	ui.rulename_textbox->clear();


	QString now_table = ui.tablename_combobox2->currentText();
	ui.nowtable_label2->setText(now_table);
	auto iter = schema_map[tostring(now_table)].begin();

	while (iter != schema_map[tostring(now_table)].end()) {
		ui.pk_combobox->addItem(fromstring(iter->first));
		ui.uk_combobox->addItem(fromstring(iter->first));
		ui.condition_combobox->addItem(fromstring(iter->first));
		ui.field_combobox->addItem(fromstring(iter->first));
		ui.nowfield_combobox->addItem(fromstring(iter->first));

		++iter;
	}

	Option2_table_show();


	ui.pk_combobox->addItem("");
	ui.pk_combobox->setCurrentText("");

	ui.uk_combobox->addItem("");
	ui.uk_combobox->setCurrentText("");

	ui.condition_combobox->addItem("");
	ui.condition_combobox->setCurrentText("");

	ui.fktable_combobox->addItem("");
	ui.fktable_combobox->setCurrentText("");

	ui.nowfield_combobox->addItem("");
	ui.nowfield_combobox->setCurrentText("");
}
