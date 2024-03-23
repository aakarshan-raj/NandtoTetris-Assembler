#include <iostream>
#include <fstream>
#include <string> 

std::string dec_to_binary(int);

int main()
{

  std::ifstream file;
  std::ofstream output("output.out");
  file.open("input.asm");
  std::string line;

  std::string output_string;
  std::string output_line;
  while (file >> line)
  {
    if (line[0] == '@')
    {
      // Type A
      int num = std::stoi(line.substr(1,line.length()-1));
      std::string type_a = dec_to_binary(num);
      output_line.append(type_a);
      output_line.insert(0,"0");
    }
    else
    {
      // Type C
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
  for(int i=0;i<len_to_append;i++){
    num.insert(0,"0");
  }
  return num;
}