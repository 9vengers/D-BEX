
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

bool ReadFromFile(char* buffer, int len)
{
    FILE* fp = nullptr;

    fopen_s(&fp, "test.json", "rb");

    if (fp == nullptr)
    {
        return false;
    }

    size_t fileSize = fread(buffer, 1, len, fp);

    fclose(fp);

    return true;
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

void readFunctionsJson(std::string filename, std::vector<functionInfo*>* functions) {
    Json::Value root;
    Json::Reader reader;
    filename = filename.substr(0, filename.length() - 5);
    filename += "Functions.json";
    cout << filename << endl;
    ifstream json(filename, ifstream::binary);
    

    bool parsingSuccessful = reader.parse(json, root);

    if (parsingSuccessful == false) {
        std::cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
        return;
    }

    std::cout << "...Converting...." << std::endl;

    int sheetNum = root.get("sheetNum", "").asInt();

    for (int i = 0; i < sheetNum; i++) {
        functionInfo* tmp = new functionInfo();

        std::string sheetCount = "sheet" + std::to_string(i + 1);
        const Json::Value cellFormula = root[sheetCount]["cellFormula"];
        const Json::Value cellName = root[sheetCount]["cellName"];
        const Json::Value sheetName = root[sheetCount]["sheetName"];
        tmp->sheetName = sheetName.asString();
        for (int j = 0; j < cellFormula.size(); j++) {
            tmp->cellFormula.push_back(cellFormula[j].asString());
            tmp->cellName.push_back(cellName[j].asString());
            functions->push_back(tmp);
        }
    }

    std::cout << " Read Functions JSON complete!.." << std::endl;
}

void writeExcel(std::string filename, excelInfo* info, std::string absPath, bool hasFunctions)
{
    std::cout << "...Make Excel..." << std::endl;
    std::string cellName;
    int size = info->getSheets().size();
    
    std::vector<functionInfo*>* functions = new std::vector<functionInfo*>();
    if (hasFunctions) {
        readFunctionsJson(filename, functions);
    }
    
    xlnt::workbook wb;
    xlnt::worksheet ws;
    for (int i = 0; i < info->getSheets().size(); i++) { //각 sheet
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

        for (int j = 0; j < tmp->cells.size(); j++) { //각 sheet의 cell들
            for (int k = 0; k < tmp->cells.at(j)->size(); k++) {
                cellName = alpha[j] + std::to_string(((k + 1)));
               
               if (tmp->schemas.at(j)->type == "INTEGER" && k>=1)
               {
                 int convertiedInt = std::stoi(tmp->cells.at(j)->at(k)->data);
                 ws.cell(cellName).value(convertiedInt); //tmp->cells.at(j)->at(k)->data
               }
               else if (tmp->schemas.at(j)->type.find("NUMERIC") != std::string::npos && k >= 1)
               {
                   double convertiedDouble = std::stod(tmp->cells.at(j)->at(k)->data);
                    ws.cell(cellName).value(convertiedDouble);
               }
               else
               {
                   if (isNumber(tmp->cells.at(j)->at(k)->data))
                   {
                       if (isDouble(tmp->cells.at(j)->at(k)->data)) {
                           double convertiedDouble = std::stod(tmp->cells.at(j)->at(k)->data);
                           ws.cell(cellName).value(convertiedDouble);
                       }
                       else
                       {
                           int convertiedInt = std::stoi(tmp->cells.at(j)->at(k)->data);
                           ws.cell(cellName).value(convertiedInt); //tmp->cells.at(j)->at(k)->data
                       }
                   }
                   else {
                       ws.cell(cellName).value(tmp->cells.at(j)->at(k)->data); //tmp->cells.at(j)->at(k)->data
                   }
               }
                //std::cout << cellName << std::endl;
                //std::cout << tmp->cells.at(j)->at(k)->data << std::endl;
            }
        }
        if (hasFunctions && functions->size()!=0) {
            for (int i = 0; i < functions->size(); i++) {
                functionInfo* f = functions->at(i);
                if (f->sheetName == tmp->sheetTitle) {
                    for (int j = 0; j < f->cellFormula.size(); j++) {
                        ws.cell(f->cellName.at(j)).formula(f->cellFormula.at(j));
                    }
                    functions->erase(functions->begin() + i);
                    break;
                }
            }
        }
    }

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

    std::string utf_string = u8"스키마";
    ws.cell("A1").value(utf_string);
    ws.cell("A1").alignment(alignment);

    setDataSchemaSheetForm(ws, "A2", u8"테이블");
    setDataSchemaSheetForm(ws, "B2", u8"필드");
    setDataSchemaSheetForm(ws, "C2", u8"타입");
    setDataSchemaSheetForm(ws, "D2", u8"null");
    setDataSchemaSheetForm(ws, "E2", u8"기본값");
    setDataSchemaSheetForm(ws, "F2", u8"콜레이션");
    setDataSchemaSheetForm(ws, "G2", u8"자동증가");

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


    cellName = "A" + to_string(index++);
    utf_string = u8"제약조건";
    ws.cell(cellName).value(utf_string);
    ws.cell(cellName).alignment(alignment);

    int indexPlusOne = index + 1;

    setDataSchemaSheetForm(ws, "A" + to_string(index), u8"테이블");

    cellName = "A" + to_string(index) + ":A" + to_string(indexPlusOne);
    ws.merge_cells(cellName);
    ws.cell("A" + to_string(indexPlusOne)).border(border);

    setDataSchemaSheetForm(ws, "B" + to_string(index), u8"제약조건이름");

    cellName = "B" + to_string(index) + ":B" + to_string(indexPlusOne);
    ws.merge_cells(cellName);
    ws.cell("B" + to_string(indexPlusOne)).border(border);

    setDataSchemaSheetForm(ws, "C" + to_string(index), u8"주요키");
    setDataSchemaSheetForm(ws, "C" + to_string(indexPlusOne), u8"필드");
    setDataSchemaSheetForm(ws, "D" + to_string(index), u8"고유키");
    setDataSchemaSheetForm(ws, "D" + to_string(indexPlusOne), u8"필드");
    setDataSchemaSheetForm(ws, "E" + to_string(index), u8"조건");

    cellName = "E" + to_string(index) + ":F" + to_string(index);
    ws.merge_cells(cellName);
    ws.cell("F" + to_string(index)).border(border);

    setDataSchemaSheetForm(ws, "E" + to_string(indexPlusOne), u8"필드");
    setDataSchemaSheetForm(ws, "F" + to_string(indexPlusOne), u8"조건식");
    setDataSchemaSheetForm(ws, "G" + to_string(index), u8"외래키");
    setDataSchemaSheetForm(ws, "G" + to_string(indexPlusOne), u8"필드");
    setDataSchemaSheetForm(ws, "H" + to_string(index), u8"참조");


    cellName = "H" + to_string(index) + ":K" + to_string(index);
    ws.merge_cells(cellName);
    ws.cell("I" + to_string(index)).border(border);
    ws.cell("J" + to_string(index)).border(border);
    ws.cell("K" + to_string(index)).border(border);

    setDataSchemaSheetForm(ws, "H" + to_string(indexPlusOne), u8"테이블");
    setDataSchemaSheetForm(ws, "I" + to_string(indexPlusOne), u8"필드");
    setDataSchemaSheetForm(ws, "J" + to_string(indexPlusOne), u8"삭제규칙");
    setDataSchemaSheetForm(ws, ("K" + to_string(indexPlusOne)), u8"수정규칙");


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
            ws.cell("A" + to_string(index)).fill(xlnt::fill::solid(xlnt::rgb_color(242, 242, 242)));
            ws.cell("A" + to_string(index)).border(border);

            setDataSchemaSheetContents(ws, "B" + to_string(index), tmp->constraints.at(j)->name);
            setDataSchemaSheetContents(ws, "C" + to_string(index), tmp->constraints.at(j)->primaryKeyField);
            setDataSchemaSheetContents(ws, "D" + to_string(index), tmp->constraints.at(j)->uniqueKeyField);
            setDataSchemaSheetContents(ws, "E" + to_string(index), tmp->constraints.at(j)->conditionField);
            setDataSchemaSheetContents(ws, "F" + to_string(index), tmp->constraints.at(j)->ConditionalStatement);
            setDataSchemaSheetContents(ws, "G" + to_string(index), tmp->constraints.at(j)->foreignKeyField);
            setDataSchemaSheetContents(ws, "H" + to_string(index), tmp->constraints.at(j)->referTable);
            setDataSchemaSheetContents(ws, "I" + to_string(index), tmp->constraints.at(j)->referField);
            setDataSchemaSheetContents(ws, "J" + to_string(index), tmp->constraints.at(j)->removeRule);
            setDataSchemaSheetContents(ws, "K" + to_string(index), tmp->constraints.at(j)->modifyRule);

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

bool isNumber(std::string str)
{
    int cnt = 0;

    if (str.empty())
    {
        return false;
    }

    for (int i = 0; i < str.size(); i++)
    {
        wchar_t wchar = str.at(i);
            
        if (wchar<48||wchar>57)
        {
               
            if (str.at(i) != '.') //.이 아닌경우
            {
                return false;
            }
            else if (cnt == 2) { //실수의 경우 .가 있을 수있는데 .가 두개있을수는 없으니깐
                return false;
            }
            else if (str.at(i) == '.')
            {
                cnt++;
            }
        }
    }
    return true;
}

bool isDouble(std::string str) 
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str.at(i) == '.')
        {
                return true;
        }
        
    }
    return false;
}

int writeXLSX(std::string filename, std::string absPath, bool hasFunctions) {
    excelInfo* info = new excelInfo();

    if (readJson(info, filename) == -1)
    {
        delete info;
        return -1;
    }

   writeExcel(filename, info, absPath, hasFunctions);

    delete info;
    return 0;
}
