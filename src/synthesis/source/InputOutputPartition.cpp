#include "InputOutputPartition.h"

#include <fstream>
#include "String_utilities.h"

namespace Syft {

std::runtime_error InputOutputPartition::bad_file_format_exception(
    std::size_t line_number) {
  return std::runtime_error("Incorrect format in line " +
                            std::to_string(line_number) +
                            " of the partition file.");
}
  
InputOutputPartition::InputOutputPartition()
{}

InputOutputPartition InputOutputPartition::read_from_file(
    const std::string& filename) {
  InputOutputPartition partition;		     
			     
  std::ifstream in(filename);

  std::size_t line_number = 1;
  std::string line;
  std::getline(in, line);
  
  std::vector<std::string> input_substr;
  input_substr = Syft::split(line, ":");

  if (input_substr.size() != 2 || input_substr[0] != ".inputs") {
    throw bad_file_format_exception(line_number);
  }

  std::string trimmed_input_substr = Syft::trim(input_substr[1]); // remove leading and trailing whitespace
  partition.input_variables = Syft::split(trimmed_input_substr, " ");

  ++line_number;
  std::getline(in, line);
  
  std::vector<std::string> output_substr;
  output_substr = Syft::split(line, ":");

  if (output_substr.size() != 2 || output_substr[0] != ".outputs") {
    throw bad_file_format_exception(line_number);
  }

  std::string trimmed_output_substr = Syft::trim(output_substr[1]); // remove leading and trailing whitespace
  partition.output_variables = Syft::split(trimmed_output_substr, " ");


  return partition;
}

}
