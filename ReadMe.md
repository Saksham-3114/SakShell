# SakShell  

**SakShell** is a lightweight, custom Unix-like shell written in C++. It provides essential shell functionalities, including running built-in commands, handling external executables, and managing input/output operations. This shell is designed to give users an intuitive command-line interface for interacting with the operating system.  

## Features  

### 1. **Command Execution**  
   - Supports running built-in commands (`echo`, `cd`, `pwd`, etc.).  
   - Executes external programs by resolving their paths in the `PATH` environment variable.  

### 2. **Custom Built-in Commands**  
   - **`echo`**: Prints strings to the console, including handling quotes and escape characters.  
   - **`pwd`**: Displays the current working directory.  
   - **`cd`**: Changes the current directory, including shortcuts like `~` for the home directory.  
   - **`type`**: Determines if a command is built-in or an external program and provides its location.  

### 3. **Quoted Text Handling**  
   - Proper parsing of single (`'`) and double (`"`) quotes.  
   - Handles escape sequences (`\`) within quoted text.  

### 4. **Error Handling**  
   - Gracefully handles incorrect commands with meaningful error messages.  

### 5. **System Integration**  
   - Supports executing external commands by integrating with the system shell.  
   - Allows piping through commands like `cat` and other common Unix tools.  

### 6. **Interactive Shell**  
   - Continuous prompt (`$`) for user input.  
   - Provides a clean and intuitive user experience for executing commands.  

---

## Installation  

### Prerequisites  
- **C++ Compiler** (Supports C++17 or higher).  
- **GCC** or an equivalent build system.  

### Build Instructions  
1. Clone the repository:  
   ```bash  
   git clone https://github.com/Saksham-3114/SakShell.git  
   cd SakShell  
