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

3. **Install the Executable and Library:**

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
```python3
lcrs/			# project folder
├── bin/		# executables
│   └── lcrs
├── build/		# object files
│   └── AddrBook.o
│   └── Configure.o
│   └── Connect.o
│   └── File.o
│   └── FileInfo.o
│   └── Network.o
│   └── Packet.o
│   └── Sys.o
├── etc/		# configuration files
│   └── Init.config
├── include/		# library interfaces
│   └── AddrBook.h
│   └── Configure.h
│   └── Connect.h
│   └── File.h
│   └── FileInfo.h
│   └── Network.h
│   └── Packet.h
│   └── Sys.h
├── lib/		# shared library
│   └── lcrs.so
├── main.cpp		# starting file
├── Makefile		# dependencies and compile files
├── README.md		# compiling and working explenation
└── src/		# library implementations
    └── AddrBook.cpp
    └── Configure.cpp
    └── Connect.cpp
    └── File.cpp
    └── FileInfo.cpp
    └── Network.cpp
    └── Packet.cpp
    └── Sys.cpp
```

## Working

- **LOADING INITIALIZATIONS**
1.	initialise default internal data
	auto detect network interface and configure system id from that
2.	check for configuration file
3.	load configuration if file present

- **PORT SETUP**
1.	create a tcp socket and try to bind it with
	specific port provided by user or default 
2.	multicast the tcp port that we bond to socket 
	in step 1.0 to specific group provided by user
	or default

- **BACKGROUND CONNECTIONS**
1.	crate an listening socket, join to specific 
	multicast group provided by user or defaul
2.	receive requests from others and work according 
	to that type of request is receive
3.	update sender address in our address book
4.	listen on the socket that we created in step no 1.0
5.	accept connections and work according to packet type
6.	update sender address in our address book

- **STORING FILE**
1.	locate file
	set file stat
2.	decide chunk size
4.	request receiver send global request for Adoptive nodes
3.	create empty metadata file
	initialise metafile
5.	select adoptive nodes to adopt file chunk
	send file chunk with acknowledgement
6.	update metafile
7.	after getting whole chunks stored store meta file also
	
conf.h line 172 check note

