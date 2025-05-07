# IRC Server Project - Detailed Work Division

## Overview
This document provides a detailed breakdown of tasks for the development of an IRC Server project. Each developer is assigned specific parts of the project to ensure comprehensive coverage and efficient workload distribution.

## Developer A: Core Server Infrastructure and Networking
### Responsibilities:
- Architect the core server framework necessary to support the IRC functionality.
- Establish robust and secure TCP/IP communication channels.
- Implement efficient non-blocking I/O operations to handle incoming and outgoing data streams.
- Ensure the server operates consistently across different operating systems, with a focus on Unix-like OSes and MacOS.

### Detailed Tasks:
1. Create a TCP server socket that listens for incoming connections on the specified port.
2. Utilize `poll()` or equivalent system calls to monitor multiple file descriptors concurrently, managing read and write operations without blocking the main thread.
3. Handle edge cases and implement proper error checking to avoid any server crashes or security vulnerabilities.
4. Develop a signal handling mechanism to gracefully shut down the server during unexpected scenarios or interruptions.
5. Write a cross-platform compatibility layer, with specific adjustments for MacOS using `fcntl()` to ensure non-blocking behavior.
6. Construct a test suite to emulate client connections and validate the stability of the networking infrastructure under various conditions.

## Developer B: IRC Protocol Implementation and User Management
### Responsibilities:
- Implement the IRC protocol stack to parse and respond to client commands accurately.
- Build the user authentication system and manage connected client sessions.
- Develop the core logic for user interaction within the server ecosystem, tracking nicknames and user modes.

### Detailed Tasks:
1. Design a protocol parser that segregates and interprets IRC commands from raw input data.
2. Build a secure authentication mechanism that verifies the connection password provided by clients.
3. Set up a user registry to manage nickname reservations, changes, and collisions as per IRC standards.
4. Implement user session management, including login, logout, and user state transitions.
5. Create a mechanism for users to query and modify server state (e.g., requesting a list of active channels, updating user modes).
6. Document the implementation and provide example usage scenarios to facilitate testing during the development phase and later for peer-evaluation.

## Developer C: Channel Management and Operator Commands
### Responsibilities:
- Manage all aspects of IRC channel lifecycle, from creation to deletion, and user participation within channels.
- Implement specialized operator commands and permissions required to moderate and manage channels effectively.
- Ensure broadcast and message routing systems are in place to facilitate real-time communication between channel participants.

### Detailed Tasks:
1. Develop a channel creation system with necessary checks for existing names and channel-specific passwords (keys).
2. Establish a user permission system to determine operator statuses, granting them the ability to use operator-specific commands.
3. Implement the `KICK`, `INVITE`, `TOPIC`, and `MODE` commands providing channel operators with tools to control the channel environment.
4. Program the logic required to broadcast messages to all channel members, handling user joining and leaving events.
5. Create a private messaging system enabling direct user-to-user communication without broadcasting to the entire channel.
6. Construct comprehensive test cases to cover all aspects of channel management functionality and command execution.

## Submission Guidelines
- Ensure the alignment with the C++98 standard throughout the codebase, avoiding the use of external libraries.
- Coordinate integration testing to confirm that all parts of the server operate together seamlessly.
- Prepare a final report summarizing the implemented functionalities, known issues, and areas for future development.

By adhering to these guidelines and fulfilling the outlined responsibilities, the collaborative effort will lead to the successful completion of the IRC Server project.
