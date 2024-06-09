# LCRS (Loosely Coupled Resilient Storage)

## Overview

LCRS (Loosely Coupled Resilient Storage) is a storage system designed to provide reliable and efficient file storage, retrieval, and search capabilities. It leverages UDP, TCP/IP, and multicast communication protocols to achieve robust and resilient storage operations.

## Features

- **Store File**: Efficiently store files in the system.
- **Retrieve File**: Retrieve stored files with ease.
- **Search File**: Search for files based on specific criteria.

## Build and Installation

### Prerequisites

Ensure you have the following installed:

- GCC (g++)
- Make
- uuid-dev

### Build Instructions

1. **Compile the Project:**

    ```sh
    make all
    ```

2. **Install the Executable and Library:**

    ```sh
    sudo make install
    ```

### Cleaning Up

1. **To clean up the build files:**

	```sh
	make clean
	```
2. **Uninstall the Executable and Library:**

    ```sh
    sudo make uninstall
    ```

### Configuration

	Configuration settings can be found and modified in the etc/Init.config file. Ensure that this file is correctly set up before running the executable.

### Usage

	After building and installing, you can run the LCRS executable:
	
	```sh
	./bin/lcrs
	```
	Ensure the shared library lcrs.so is in the correct location (/usr/local/lib by default if installed using make install).

### Acknowledgements
	Nilesh Nandkishor Ahire - Project Creator

### Contact
	For any questions or inquiries, please reach out to 

	```Mail
	niluahirepatil@gmail.com
	```
## Directory Structure
```
lcrs/
├── bin/
│   └── lcrs
├── build/
│   └── AddrBook.o
│   └── Configure.o
│   └── Connect.o
│   └── File.o
│   └── FileInfo.o
│   └── Network.o
│   └── Packet.o
│   └── Sys.o
├── etc/
│   └── Init.config
├── include/
│   └── AddrBook.h
│   └── Configure.h
│   └── Connect.h
│   └── File.h
│   └── FileInfo.h
│   └── Network.h
│   └── Packet.h
│   └── Sys.h
├── lib/
│   └── lcrs.so
├── main.cpp
├── Makefile
├── README.md
├── src/
│   └── AddrBook.cpp
│   └── Configure.cpp
│   └── Connect.cpp
│   └── File.cpp
│   └── FileInfo.cpp
│   └── Network.cpp
│   └── Packet.cpp
│   └── Sys.cpp
```

### Working

**LOADING INITIALIZATIONS**

0.0	*initialise default data*
0.1	*check for configuration file*
0.2	load configuration in file present

1.0	create a tcp socket and try to bind it with
	specific port provided by user or default 

2.0	multicast the tcp port that we bond to socket 
	in step 1.0 to specific group provided by user
	or default

** BACKGROUND CONNECTIONS **

3.0	crate an listening socket, join to specific 
	multicast group provided by user or default
3.1	receive requests from others and work according 
	to that type of request is receive
3.2	update sender address in our address book

4.0	listen on the socket that we created in step no 1.0
4.1	accept connections and work according to packet type
4.2	update sender address in our address book

** STORING FILE **

5.0 	locate file
	decide chunk size
	create metadata file
	request receiver for each chunk of file
	decide transfer file chunk to which receiver
	update metafile
	after getting whole chunks stored store meta file also
	automatic detect network interface and configure sys id from that
	
conf.h line 172 check note

