# SGX-SQLite3
Secure SQLite Database entirely inside a secure Intel SGX enclave (Linux).

You could execute your SQL statements securely.
The official SQLite library ("sqlite3.c", "sqlite3.h", "sqlite3ext.h") is entirely loaded into an enclave.
Currently, you cannot save your database to disk since security reason. Only in-memory database is supported in this version. 
I'm implementing the auto data encryption feature, but not finish yet. Stay tuned.

Technical discussions on SGX, feel free to contact me.

My linkedIn:
https://www.linkedin.com/in/su-wang-09b6081b/

SQLite Version - 3.30.1

The project started from the project SGX_SQLite on GitHub provided by @yerzhan7.
I found it has some security flaws that all file operations are performed out of SGX Enclave and it depends on untrusted OS syscalls, which makes the db insecure at all. 
So I made two major changes: 1) remove untrusted file operations till the trusted filesystem APIs are applied and 2)replace PRNG with SGX TRNG for randomness.


This project may act as guide on how to port C applications inside Intel SGX enclave on Linux.

# Prerequisites:
* Ubuntu* Desktop-18.04-LTS 64bits
* Intel SGX SDK v2.7, SGX Driver, SGX PSW from https://github.com/01org/linux-sgx
* 6th Generation Intel(R) Core(TM) Processor(Skeylake) or newer (only if you want to run it in hardware mode, otherwise run in software/simulation mode)

# What's Included
* **App/** - directory for untrusted part of the application
  * **App.cpp** - main file for our app
  * **ocalls.c** - implementations of untrusted ocalls in C
* **Enclave/** - directory for trusted part of the application
  * **Enclave.config.xml** - enclave config file (same as in SampleEnclave from Intel)
  * **Enclave.cpp** - implementations of trusted ecalls in C++
  * **Enclave.edl** - enclave interface file for declarations of all ecalls and ocalls
  * **Enclave.lds** - lds file (same as in SampleEnclave from Intel)
  * **Enclave_private.pem** - enclave private key (same as in SampleEnclave from Intel)
  * **ocall_interface.c** - implementations of redirected system calls from sqlite3.c (not all syscalls implemented)
  * **sqlite3.c** - official SQLite amalgamation (made minor changes to correctly redirect syscalls - see commit history)
  * **sqlite3.h** - official SQLite amalgamation (unchanged)
* **ocall_types.h** - declarations of some stdlib types for need for edl file (since stlib is untrusted)
  
# Building
`make` for simple simulation mode (unsecure)

`make SGX_MODE=HW SGX_DEBUG=0 SGX_PRERELEASE=1` for hardware mode in pre-release mode

`make SGX_MODE=HW SGX_DEBUG=0 SGX_PRERELEASE=0` for hardware mode in a secure release mode (but you need to sign your enclave first)

# Running
`./app` to create an in-memory database.
