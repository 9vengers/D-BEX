
#include "excelWriter.hpp"

extern std::string alpha[ALPHASIZE] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N","O", "P", "Q", "R", "S","T","U","V","W","X","Y","Z" };

void setDataSchemaSheetForm(xlnt::worksheet ws, std::string cellName, std::string utf_string)
{
    ws.cell(cellName).value(utf_string);
    ws.cell(cellName).fill(xlnt::fill::solid(xlnt::rgb_color(217, 217, 217)));

    xlnt::alignment alignment;
    alignment.horizontal(xlnt::horizontal_alignment::center);
    alignment.vertical(xlnt::vertical_alignment::center);

    ws.cell(cellName).alignment(alignment);

    xlnt::border border;
    xlnt::border::border_property border_pro;
    border_pro.style(xlnt::border_style::thin);

    border.side(xlnt::border_side::start, border_pro);
    border.side(xlnt::border_side::end, border_pro);
    border.side(xlnt::border_side::top, border_pro);
    border.side(xlnt::border_side::bottom, border_pro);

    ws.cell(cellName).border(border);
}

void setDataSchemaSheetContents(xlnt::worksheet ws, std::string cellName, std::string string)
{
    ws.cell(cellName).value(string);
    ws.cell(cellName).fill(xlnt::fill::solid(xlnt::rgb_color(242, 242, 242)));
    xlnt::alignment alignment;
    alignment.horizontal(xlnt::horizontal_alignment::center);
    alignment.vertical(xlnt::vertical_alignment::center);

    ws.cell(cellName).alignment(alignment);

    xlnt::border border;
    xlnt::border::border_property border_pro;
    border_pro.style(xlnt::border_style::thin);

    border.side(xlnt::border_side::start, border_pro);
    border.side(xlnt::border_side::end, border_pro);
    border.side(xlnt::border_side::top, border_pro);
    border.side(xlnt::border_side::bottom, border_pro);

    ws.cell(cellName).border(border);
}

int readJson(excelInfo* info, std::string filename) {
    Json::Value root;
    Json::Reader reader;
    std::ifstream json(filename, std::ifstream::binary);

    bool parsingSuccessful = reader.parse(json, root);

    if (parsingSuccessful == false) {
        std::cout << "[error] Failed to parse configuration\n" << reader.getFormatedErrorMessages();
        return -1;
    }

    std::string name = root.get("name", "").asString();
    info->setTitle(name);

    int sheetNum = root.get("tableNum", "").asInt();
    for (int i = 0; i < sheetNum; i++) {
        std::string sheetCount = "table" + std::to_string(i + 1);
        info->setSheet(sheetCount, root, i);
    }

    std::cout << "-------------SUCCESS READ JSON---------------" << std::endl;
    return 0;
}

void writeExcel(excelInfo* info, std::string absPath)
{
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "--------------START MAKE EXCEL---------------" << std::endl;
    std::string cellName;
    int size = info->getSheets().size();
    xlnt::workbook wb;
    xlnt::worksheet ws;
    std::cout << "--------------------...----------------------" << std::endl;
    for (int i = 0; i < info->getSheets().size(); i++) { //�� sheet
        if (i == 0) {
            ws = wb.active_sheet();
        }
        else {
            ws = wb.create_sheet();
        }

        sheetDetails* tmp = info->getSheets().at(i);
        //std::cout << tmp->sheetTitle << std::endl;
        ws.title(tmp->sheetTitle);
        /*
        for (int k = 0; k < ALPHASIZE; k++)
        {
            ws.column_properties(alpha[k]).best_fit = true;
        }
        */

        for (int j = 0; j < tmp->cells.size(); j++) { //�� sheet�� cell��
            for (int k = 0; k < tmp->cells.at(j)->size(); k++) {
                cellName = alpha[j] + std::to_string(((k + 1)));
                ws.cell(cellName).value(tmp->cells.at(j)->at(k)->data); //tmp->cells.at(j)->at(k)->data
                //std::cout << cellName << std::endl;
                //std::cout << tmp->cells.at(j)->at(k)->data << std::endl;
            }
        }
    }
    std::cout << "--------------------...----------------------" << std::endl;
    ws = wb.create_sheet();
    ws.title("DataSchema");
    /*
    for (int k = 0; k < ALPHASIZE; k++)
    {
        ws.column_properties(alpha[k]).best_fit = true;
    }
    */

    xlnt::alignment alignment;
    alignment.horizontal(xlnt::horizontal_alignment::center);
    alignment.vertical(xlnt::vertical_alignment::center);

    xlnt::border border;
    xlnt::border::border_property border_pro;
    border_pro.style(xlnt::border_style::thin);

    border.side(xlnt::border_side::start, border_pro);
    border.side(xlnt::border_side::end, border_pro);
    border.side(xlnt::border_side::top, border_pro);
    border.side(xlnt::border_side::bottom, border_pro);

    std::string utf_string = u8"��Ű��";
    ws.cell("A1").value(utf_string);
    ws.cell("A1").alignment(alignment);

    setDataSchemaSheetForm(ws, "A2", u8"���̺�");
    setDataSchemaSheetForm(ws, "B2", u8"�ʵ�");
    setDataSchemaSheetForm(ws, "C2", u8"Ÿ��");
    setDataSchemaSheetForm(ws, "D2", u8"null");
    setDataSchemaSheetForm(ws, "E2", u8"�⺻��");
    setDataSchemaSheetForm(ws, "F2", u8"�ݷ��̼�");
    setDataSchemaSheetForm(ws, "G2", u8"�ڵ�����");

    int index = 3;
    for (int i = 0; i < info->getSheets().size(); i++) {
        sheetDetails* tmp = info->getSheets().at(i);
        cellName = "A" + std::to_string(index);
        ws.cell(cellName).value(tmp->sheetTitle);
        ws.cell(cellName).border(border);
        for (int j = 0; j < tmp->schemas.size(); j++)
        {
            cellName = "A" + std::to_string(index);
            ws.cell(cellName).fill(xlnt::fill::solid(xlnt::rgb_color(242, 242, 242)));
            ws.cell(cellName).alignment(alignment);
            ws.cell(cellName).border(border);

            setDataSchemaSheetContents(ws, "B" + std::to_string(index), tmp->schemas.at(j)->field);
            setDataSchemaSheetContents(ws, "C" + std::to_string(index), tmp->schemas.at(j)->type);
            setDataSchemaSheetContents(ws, "D" + std::to_string(index), tmp->schemas.at(j)->isNull);
            setDataSchemaSheetContents(ws, "E" + std::to_string(index), tmp->schemas.at(j)->defaultValue);
            setDataSchemaSheetContents(ws, "F" + std::to_string(index), tmp->schemas.at(j)->collation);
            setDataSchemaSheetContents(ws, "G" + std::to_string(index), tmp->schemas.at(j)->autoInc);

            index++;
        }
    }

    std::cout << "--------------------...----------------------" << std::endl;

    cellName = "A" + std::to_string(index++);
    utf_string = u8"��������";
    ws.cell(cellName).value(utf_string);
    ws.cell(cellName).alignment(alignment);

    int indexPlusOne = index + 1;

    setDataSchemaSheetForm(ws, "A" + std::to_string(index), u8"���̺�");

    cellName = "A" + std::to_string(index) + ":A" + std::to_string(indexPlusOne);
    ws.merge_cells(cellName);
    ws.cell("A" + std::to_string(indexPlusOne)).border(border);

    setDataSchemaSheetForm(ws, "B" + std::to_string(index), u8"���������̸�");

    cellName = "B" + std::to_string(index) + ":B" + std::to_string(indexPlusOne);
    ws.merge_cells(cellName);
    ws.cell("B" + std::to_string(indexPlusOne)).border(border);

    setDataSchemaSheetForm(ws, "C" + std::to_string(index), u8"�ֿ�Ű");
    setDataSchemaSheetForm(ws, "C" + std::to_string(indexPlusOne), u8"�ʵ�");
    setDataSchemaSheetForm(ws, "D" + std::to_string(index), u8"����Ű");
    setDataSchemaSheetForm(ws, "D" + std::to_string(indexPlusOne), u8"�ʵ�");
    setDataSchemaSheetForm(ws, "E" + std::to_string(index), u8"����");

    cellName = "E" + std::to_string(index) + ":F" + std::to_string(index);
    ws.merge_cells(cellName);
    ws.cell("F" + std::to_string(index)).border(border);

    setDataSchemaSheetForm(ws, "E" + std::to_string(indexPlusOne), u8"�ʵ�");
    setDataSchemaSheetForm(ws, "F" + std::to_string(indexPlusOne), u8"���ǽ�");
    setDataSchemaSheetForm(ws, "G" + std::to_string(index), u8"�ܷ�Ű");
    setDataSchemaSheetForm(ws, "G" + std::to_string(indexPlusOne), u8"�ʵ�");
    setDataSchemaSheetForm(ws, "H" + std::to_string(index), u8"����");


    cellName = "H" + std::to_string(index) + ":K" + std::to_string(index);
    ws.merge_cells(cellName);
    ws.cell("I" + std::to_string(index)).border(border);
    ws.cell("J" + std::to_string(index)).border(border);
    ws.cell("K" + std::to_string(index)).border(border);

    setDataSchemaSheetForm(ws, "H" + std::to_string(indexPlusOne), u8"���̺�");
    setDataSchemaSheetForm(ws, "I" + std::to_string(indexPlusOne), u8"�ʵ�");
    setDataSchemaSheetForm(ws, "J" + std::to_string(indexPlusOne), u8"������Ģ");
    setDataSchemaSheetForm(ws, ("K" + std::to_string(indexPlusOne)), u8"������Ģ");


    index = indexPlusOne + 1;

    for (int i = 0; i < info->getSheets().size(); i++) {
        sheetDetails* tmp = info->getSheets().at(i);
        if (tmp->constraints.size() == 0) {
            continue;
        }
        cellName = "A" + std::to_string(index);
        ws.cell(cellName).value(tmp->sheetTitle);
        ws.cell(cellName).alignment(alignment);
        ws.cell(cellName).border(border);
        //std::cout << tmp->constraints.size() << std::endl;
        for (int j = 0; j < tmp->constraints.size(); j++) {
            //std::cout << tmp->constraints.at(j)->name << std::endl;
            ws.cell("A" + std::to_string(index)).fill(xlnt::fill::solid(xlnt::rgb_color(242, 242, 242)));
            ws.cell("A" + std::to_string(index)).border(border);

            setDataSchemaSheetContents(ws, "B" + std::to_string(index), tmp->constraints.at(j)->name);
            setDataSchemaSheetContents(ws, "C" + std::to_string(index), tmp->constraints.at(j)->primaryKeyField);
            setDataSchemaSheetContents(ws, "D" + std::to_string(index), tmp->constraints.at(j)->uniqueKeyField);
            setDataSchemaSheetContents(ws, "E" + std::to_string(index), tmp->constraints.at(j)->conditionField);
            setDataSchemaSheetContents(ws, "F" + std::to_string(index), tmp->constraints.at(j)->ConditionalStatement);
            setDataSchemaSheetContents(ws, "G" + std::to_string(index), tmp->constraints.at(j)->foreignKeyField);
            setDataSchemaSheetContents(ws, "H" + std::to_string(index), tmp->constraints.at(j)->referTable);
            setDataSchemaSheetContents(ws, "I" + std::to_string(index), tmp->constraints.at(j)->referField);
            setDataSchemaSheetContents(ws, "J" + std::to_string(index), tmp->constraints.at(j)->removeRule);
            setDataSchemaSheetContents(ws, "K" + std::to_string(index), tmp->constraints.at(j)->modifyRule);

            index++;

        }

    }

    ws.column_properties("A").width = 18;
    ws.column_properties("B").width = 18;
    ws.column_properties("C").width = 18;
    ws.column_properties("D").width = 9;
    ws.column_properties("E").width = 9;
    ws.column_properties("F").width = 9;
    ws.column_properties("G").width = 9;
    ws.column_properties("H").width = 9;
    ws.column_properties("I").width = 9;
    ws.column_properties("J").width = 9;
    ws.column_properties("K").width = 9;

    std::string path = absPath + "\\" + info->getTitle();
    wb.save(path);
}

int writeXLSX(std::string filename, std::string absPath) {
    excelInfo* info = new excelInfo();
    
    if (readJson(info, filename) == -1)
    {
        delete info;
        return -1;
    }

    writeExcel(info, absPath);

    delete info;
    return 0;
}