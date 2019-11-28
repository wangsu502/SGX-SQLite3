#include "sgx_urts.h"
#include "Enclave_u.h"
#include <iostream>
#include <string>

# define MAX_PATH FILENAME_MAX
# define ENCLAVE_FILENAME "enclave.signed.so"

// ocalls for printing string (C++ ocalls)
void ocall_print_error(const char *str) {
  std::cerr << str << std::endl;
}

void ocall_print_string(const char *str) {
  std::cout << str;
}

void ocall_println_string(const char *str) {
  std::cout << str << std::endl;
}

// Application entry
int main(int argc, char *argv[]) {
  sgx_enclave_id_t eid = 0;
  char token_path[MAX_PATH] = {'\0'};
  sgx_launch_token_t token = {0};
  sgx_status_t ret = SGX_ERROR_UNEXPECTED; // status flag for enclave calls
  int updated = 0;
  int debug_flag = 1;

  // Initialize the enclave
  ret = sgx_create_enclave(ENCLAVE_FILENAME, debug_flag, &token, &updated, &eid, NULL);
  if (ret != SGX_SUCCESS) {
    std::cerr << "Error: creating enclave" << std::endl;
    return -1;
  }
  std::cout << "Info: SQLite SGX enclave successfully created." << std::endl;


  // Open SQLite database
  ret = ecall_opendb(eid);
  if (ret != SGX_SUCCESS) {
    std::cerr << "Error: Making an ecall_open()" << std::endl;
    return -1;
  }

  std::cout << "Enter SQL statement to execute or 'quit' to exit: " << std::endl;
  std::string input;
  std::cout << "> ";
  while(getline(std::cin, input)) {
    if (input == "quit") {
      break;
    }
    const char* sql = input.c_str();
    ret =  ecall_execute_sql(eid, sql);
    if (ret != SGX_SUCCESS) {
      std::cerr << "Error: Making an ecall_execute_sql()" << std::endl;
      return -1;
    }
    std::cout << "> ";
  }

  // Closing SQLite database inside enclave
  ret =  ecall_closedb(eid);
  if (ret != SGX_SUCCESS) {
    std::cerr << "Error: Making an ecall_closedb()" << std::endl;
    return -1;
  }

  // Destroy the enclave
  sgx_destroy_enclave(eid);
  if (ret != SGX_SUCCESS) {
    std::cerr << "Error: destroying enclave" << std::endl;
    return -1;
  }

  std::cout << "Info: SQLite SGX enclave successfully returned." << std::endl;
  return 0;
}
