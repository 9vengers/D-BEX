#include "ExcelToJson.h"

void ExcelToJson() {
  std::string output_path;
  std::string output_name;
  std::string file_extension;
  std::string file_name;
  std::string input_path;
  std::string input_name;
  xlnt::workbook wb;
  Json::Value root;

  std::cout << "���� �̸��� ������ ��θ� �Է����ּ���: ";
  std::cin >> input_path;
  file_extension = input_path.substr(input_path.size() - 4, 4);
  if (file_extension._Equal(EXTENSION) != 1) {
  std::cout << "�˸��� Ȯ���ڰ� �ƴմϴ�." << std::endl;
  return;
  }

  wb.load(input_path);

  std::cout << "\n���� ��θ� �Է����ּ���: ";
  std::cin >> output_path;
  output_path = "C:/Users/82109/Desktop/Project";
  std::cout << "\n������ ���ϸ��� Ȯ���� ���� �Է����ּ���: ";
  std::cin >> output_name;
  output_name = "test";
  output_name = output_name + ".json";
  output_path = output_path + "/" + output_name;

  std::cout << "\n�Ľ����Դϴ�.." << std::endl;
  std::stringstream ssInt;

  int number = 0;
  int sheet_count = wb.sheet_count();

  for (int x = 0; x < sheet_count; x++) {  // sheet for��
    Json::Value now_table;
    xlnt::worksheet sheet = wb.sheet_by_index(x);

    std::string sheet_name = sheet.print_titles();
    sheet_name = sheet_name.substr(0, sheet_name.size() - 1);
    int column_number = sheet.row_height(sheet.lowest_row());

    //���̺� �̸��� ����� ����
    ssInt << x + 1;
    std::string table_name = TABLE + ssInt.str();

    //�÷� �̸��� ����� ����
    xlnt::column_t column_name;

    for (auto column : sheet.columns(false)) {  // column for��
      Json::Value now_column;

      for (auto cell : column) {  // cell for��
        xlnt::cell now_cell = cell;
        now_column.append(now_cell.to_string());
      }

      now_table[column_name.column_string()] = now_column;  //���̺� �÷� ����

      std::string table_name = sheet.print_titles();
      table_name = table_name.substr(0, table_name.size() - 1);
      now_table["tableName"] = table_name;

      column_name++;
    }
    int row_count = now_table.size() - 1;
    now_table["columnNum"] = row_count;
    ssInt.str("");

    root[table_name] = now_table;  //��ü�� ���̺� ����

    root["tableNum"] = sheet_count;
    root["name"] = sheet_name;
  }

  std::cout << "�Ľ��� ���������� �̷�������ϴ�" << std::endl;
  //std::cout << root << std::endl;
  Json::Value confRoot;
  Json::StyledStreamWriter wirter;
  const char* path = output_path.c_str();
  std::ofstream writeOfStream(path);

  wirter.write(writeOfStream, root);

  writeOfStream.close();
}