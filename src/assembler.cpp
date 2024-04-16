#include <iostream>
#include <fstream>
#include <string>
#include <map>

int VARIABLE_ADDRESS = 16;

std::string dec_to_binary(int);

struct typec_instruction
{

  std::map<std::string, std::string> dest_map = {
      {"", "000"},
      {"M", "001"},
      {"D", "010"},
      {"MD", "011"},
      {"A", "100"},
      {"AM", "101"},
      {"AD", "110"},
      {"AMD", "111"},
  };
  std::map<std::string, std::string> comp_map = {
      {"0", "0101010"},
      {"1", "0111111"},
      {"-1", "0111010"},
      {"D", "0001100"},
      {"A", "0110000"},
      {"!D", "0001101"},
      {"!A", "0110001"},
      {"-D", "0001111"},
      {"-A", "0110011"},
      {"D+1", "0011111"},
      {"A+1", "0110111"},
      {"D-1", "0001110"},
      {"A-1", "0110010"},
      {"D+A", "0000010"},
      {"D-A", "0010011"},
      {"A-D", "00000111"},
      {"D&A", "0000000"},
      {"D|A", "0010101"},

      {"", "1101010"},
      {"", "1111111"},
      {"", "1111010"},
      {"", "1001100"},
      {"M", "1110000"},
      {"", "1001101"},
      {"!M", "1110001"},
      {"", "1001111"},
      {"-M", "1110011"},
      {"", "1011111"},
      {"M+1", "1110111"},
      {"", "1001110"},
      {"M-1", "1110010"},
      {"D+M", "1000010"},
      {"D-M", "1010011"},
      {"M-D", "1000111"},
      {"D&M", "1000000"},
      {"D|M", "1010101"},

  };
  std::map<std::string, std::string> jump_map = {
      {"", "000"},
      {"JGT", "001"},
      {"JEQ", "010"},
      {"JGE", "011"},
      {"JLT", "100"},
      {"JNE", "101"},
      {"JLE", "110"},
      {"JMP", "111"},
  };
  ;

  std::string dest;
  std::string comp;
  std::string jump;

  typec_instruction(std::string line)
  {
    std::string token;
    for (char c : line)
    {
      if (c == '=')
      {
        dest = token;
        token = "";
      }
      else if (c == ';')
      {
        comp = token;
        token = "";
      }
      else if (c == ' ')
      {
        // ignore whitespaces
      }
      else
      {
        token += c;
      }
    }
    if (comp == "")
    {
      comp = token;
    }
    else
    {
      jump = token;
    }
  }

  std::string get_instruction()
  {
    return comp_map[comp] + dest_map[dest] + jump_map[jump];
  }
};

std::map<std::string, int> symbol_table = {
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    {"R8", 8},
    {"R9", 9},
    {"R10", 10},
    {"R11", 11},
    {"R12", 12},
    {"R13", 13},
    {"R14", 14},
    {"R15", 15},
    {"R16", 16},
    {"SP", 0},
    {"LCL", 1},
    {"ARG", 2},
    {"THIS", 3},
    {"THAT", 4},
    {"SCREEN", 16384},
    {"KBD", 24576},

};

int getValue(std::string key)
{
  return symbol_table[key];
}

int insertVariable(std::string key)
{
  auto it = symbol_table.find(key);
  if (it != symbol_table.end())
  {
    return it->second;
  }
  else
  {
    symbol_table[key] = VARIABLE_ADDRESS;
    return VARIABLE_ADDRESS++;
  }
}

int main()
{

  std::ifstream file;
  std::ofstream output("output.out");
  file.open("input.asm");
  std::string line;

  std::string output_string;
  std::string output_line;
  std::string full_line;

  // First Pass
  int num_of_lines = -1;
  while (file >> line)
  {
    if (line[0] == '/')
    {
      getline(file, full_line);
      continue;
    }
    else if (line[0] == '(')
    {
      symbol_table[line.substr(1, line.length() - 2)] = num_of_lines + 1;
    }
    else
    {
      num_of_lines++;
    }
  }

  file.close();
  file.open("input.asm");
  line.clear();

  // Second Pass
  while (file >> line)
  {
    if (line[0] == '/')
    {
      getline(file, full_line);
      continue;
    }
    else if (line[0] == '@')
    {
      // Type A
      int num = 0;
      try
      {
        num = std::stoi(line.substr(1, line.length() - 1));
      }
      catch (std::invalid_argument x)
      {
        num = insertVariable(line.substr(1, line.length() - 1));
      }
      std::string type_a = dec_to_binary(num);
      output_line.append(type_a);
      output_line.insert(0, "0");
    }
    else if (line[0] == '(')
    {
      continue;
    }
    else
    {
      // Type C
      typec_instruction ins(line);
      output_line.append(ins.get_instruction());
      output_line.insert(0, "111");
    }
    output_string.append(output_line);
    output_string.append("\n");
    output_line = "";
  }
  output << output_string;
  file.close();
  output.close();
}

std::string dec_to_binary(int n)
{
  std::string num;
  while (n > 0)
  {
    num = (n % 2 == 0 ? "0" : "1") + num;
    n /= 2;
  }
  int len_to_append = 15 - num.length();
  for (int i = 0; i < len_to_append; i++)
  {
    num.insert(0, "0");
  }
  return num;
}