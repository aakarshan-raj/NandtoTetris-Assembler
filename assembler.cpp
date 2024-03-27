#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::string dec_to_binary(int);

struct typec_instruction
{

  std::map<std::string, std::string> dest_map = {
      {"", "000"},
      {"m", "001"},
      {"d", "010"},
      {"md", "011"},
      {"a", "100"},
      {"am", "101"},
      {"ad", "110"},
      {"amd", "111"},
  };
  std::map<std::string, std::string> comp_map = {
      {"0", "0101010"},
      {"1", "0111111"},
      {"-1", "0111010"},
      {"d", "0001100"},
      {"a", "0110000"},
      {"!d", "0001101"},
      {"!a", "0110001"},
      {"-d", "0001111"},
      {"-a", "0110011"},
      {"d+1", "0011111"},
      {"a+1", "0110111"},
      {"d-1", "0001110"},
      {"a-1", "0110010"},
      {"d+a", "0000010"},
      {"d-a", "0010011"},
      {"a-d", "00000111"},
      {"d&a", "0000000"},
      {"d|a", "0010101"},

      {"", "1101010"},
      {"", "1111111"},
      {"", "1111010"},
      {"", "1001100"},
      {"m", "1110000"},
      {"", "1001101"},
      {"!m", "1110001"},
      {"", "1001111"},
      {"-m", "1110011"},
      {"", "1011111"},
      {"m+1", "1110111"},
      {"", "1001110"},
      {"m-1", "1110010"},
      {"d+m", "1000010"},
      {"d-m", "1010011"},
      {"m-d", "1000111"},
      {"d&m", "1000000"},
      {"d|m", "1010101"},

  };
  std::map<std::string, std::string> jump_map = {
      {"", "000"},
      {"jgt", "001"},
      {"jeq", "010"},
      {"jge", "011"},
      {"jlt", "100"},
      {"jne", "101"},
      {"jle", "110"},
      {"jmp", "111"},
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
    {"r0", 0},
    {"r1", 1},
    {"r2", 2},
    {"r3", 3},
    {"r4", 4},
    {"r5", 5},
    {"r6", 6},
    {"r7", 7},
    {"r8", 8},
    {"r9", 9},
    {"r10", 10},
    {"r11", 11},
    {"r12", 12},
    {"r13", 13},
    {"r14", 14},
    {"r15", 15},
    {"r16", 16},
    {"sp", 0},
    {"lcl", 1},
    {"arg", 2},
    {"this", 3},
    {"that", 4},
};

int main()
{

  std::ifstream file;
  std::ofstream output("output.out");
  file.open("input.asm");
  std::string line;

  std::string output_string;
  std::string output_line;

  // First Pass
  int num_of_lines = -1;
  while (file >> line)
  {
    if (line[0] == '(')
    {
      symbol_table[line.substr(1, line.length() - 2)] = num_of_lines + 1;
    }
    else
    {
      num_of_lines++; // dont increase when its a label
    }
  }

  for (const auto &x : symbol_table)
  {
    std::cout << x.first << ":" << x.second << std::endl;
  }
  // Second Pass
  // while (file >> line)
  // {
  //   if (line[0] == '@')
  //   {
  //     // Type A
  //     int num = std::stoi(line.substr(1, line.length() - 1));
  //     std::string type_a = dec_to_binary(num);
  //     output_line.append(type_a);
  //     output_line.insert(0, "0");
  //   }
  //   else
  //   {
  //     // Type C
  //     typec_instruction ins(line);
  //     output_line.append(ins.get_instruction());
  //     output_line.insert(0, "111");
  //   }
  //   output_string.append(output_line);
  //   output_string.append("\n");
  //   output_line = "";
  // }
  // output << output_string;
  // file.close();
  // output.close();
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