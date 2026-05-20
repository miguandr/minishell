# minishell

A Unix shell built from scratch in C as part of the 42 Berlin core curriculum. It replicates core bash behavior: tokenization, parsing, variable expansion, redirections, pipes, heredocs, and all mandatory built-in commands.

Built by [Miguel Andrade](https://github.com/miguandr) and [Daniela Torretta](https://github.com/dtorretta).

---

## Features

- Interactive prompt with command history (via readline)
- Single and multi-command pipelines (`cmd1 | cmd2 | cmd3`)
- Input/output redirections (`<`, `>`, `>>`)
- Heredoc (`<<`)
- Environment variable expansion (`$VAR`, `$?`)
- Single and double quote handling
- Signal handling: `Ctrl+C`, `Ctrl+D`, `Ctrl+\`
- 7 built-in commands
- PATH-based command resolution

---

## Built-in Commands

| Command | Description |
|---------|-------------|
| `echo [-n]` | Print arguments to stdout. `-n` suppresses the trailing newline |
| `cd [path]` | Change directory. Supports `..`, `-` (OLDPWD), and no-arg (HOME) |
| `pwd` | Print the current working directory |
| `export [NAME=value]` | Set or update an environment variable |
| `unset [NAME]` | Remove an environment variable |
| `env` | Print all current environment variables |
| `exit [code]` | Exit the shell with an optional status code |

---

## Build & Run

**Dependencies:** `readline` must be installed.

```bash
# macOS (Homebrew)
brew install readline

# Ubuntu/Debian
sudo apt-get install libreadline-dev
```

```bash
# Clone and build
git clone https://github.com/miguandr/minishell.git
cd minishell
make

# Run
./minishell
```

```bash
# Clean object files
make clean

# Full clean (objects + binary)
make fclean

# Rebuild from scratch
make re
```

The Makefile automatically detects macOS vs Linux and adjusts the readline include/library paths.

---

## Architecture

The shell processes each command through a three-stage pipeline:

```
readline input
     │
     ▼
 ┌─────────┐
 │  Lexer  │  Tokenizes raw input into a doubly-linked list of tokens
 └────┬────┘  (WORD, PIPE, GREAT, GREAT_GREAT, LESS, HERE_DOC)
      │
      ▼
 ┌──────────┐
 │  Parser  │  Builds a linked list of t_parser nodes (one per command).
 └────┬─────┘  Separates arguments from redirections. Calls the expander.
      │
      ▼
 ┌──────────────┐
 │   Expander   │  Resolves $VAR, $?, quote stripping before execution
 └──────┬───────┘
        │
        ▼
 ┌──────────────┐
 │   Executor   │  Dispatches to builtins or execve. Handles pipes via
 └──────────────┘  fork/dup2. Manages heredoc temp files.
```

### Data Structures

**`t_mshell`** — global shell state: environment, PATH array, PWD/OLDPWD, pipe count, PID array, exit code.

**`t_lexer`** — doubly-linked list node: raw string + token type + quote flags (`is_single`, `is_double`).

**`t_parser`** — doubly-linked list node per command: argument array, builtin function pointer, redirections list, heredoc info.

### Key design decisions

- **Builtins are stored as function pointers** in `t_parser.builtins`. At parse time, `builtins_handler()` does a name lookup and stores the pointer directly — no string comparison at execution time.
- **`cd`, `exit`, `export`, `unset` run in the parent process**; all others fork. These four mutate shell state (env, PWD, exit code) so they can't run in a child.
- **Heredocs write to a temp file** with a generated name; the executor reads from that fd.
- **The expander runs during parsing**, not execution. Each token in the argument array is expanded in place before being handed off to the executor.
- **Error handling is centralized** via `handle_error()` and `handle_error2()`, which print to stderr, set the exit code, and call `reset_data()` to bring the shell back to a clean state without exiting.

---

## Project Structure

```
minishell/
├── includes/
│   └── minishell.h          # All structs, enums, and function prototypes
├── sources/
│   ├── main.c               # Entry point + main loop (lexer → parser → executor)
│   ├── lexer/               # Tokenizer: splits input into t_lexer nodes
│   ├── parser/              # Builds t_parser nodes, separates args/redirections
│   ├── expander/            # Variable and quote expansion
│   ├── executor/            # Forks, pipes, execve, heredoc
│   ├── builtins/            # echo, cd, pwd, export, unset, env, exit
│   ├── utils/               # Init, reset, signals, environment, memory
│   └── error/               # Centralized error handling
└── libft/                   # Custom C standard library (ft_printf, get_next_line, libft)
```

---

## My Contributions (Miguel Andrade)

I was responsible for the **input processing pipeline** — everything from raw user input to the structured representation the executor consumes.

### Lexer (`sources/lexer/`)
Full implementation of the tokenizer:
- Scans raw input character by character, producing a doubly-linked list of `t_lexer` nodes
- Handles all 6 token types: `WORD`, `PIPE`, `GREAT`, `GREAT_GREAT`, `LESS`, `HERE_DOC`
- Quote-aware word boundary detection: single and double quotes are respected when determining where a token ends
- `lexer_error_check.c`: validates token sequences before parsing (catches things like `| |`, trailing pipes, commands starting with `>>`)
- Quote balance check (`count_quotes`) before tokenization even starts

### Expander (`sources/expander/`)
Full implementation of the variable expansion engine:
- `expander.c`: entry point — iterates over the argument array and dispatches to the right handler
- `expander_variables.c`: `$VAR` lookup against `envp`, `$?` expansion (exit code), variable name extraction
- `expander_utils.c`: double-quote expansion (variables expand inside `"..."`), single-quote passthrough (variables do NOT expand inside `'...'`), edge cases like `$VAR` at end of string or followed by non-alphanumeric chars
- Also handled `expander_heredoc` — variable expansion inside heredoc content

### Error handling (`sources/error/error.c`)
Centralized error system used throughout the entire codebase:
- `handle_error()`: maps 9 error codes to specific messages (syntax errors, memory failures, system call failures), sets exit code, calls `reset_data()` to recover
- `handle_error2()`: handles `export`-specific errors (invalid identifiers, event not found, non-numeric exit args)

### Shell infrastructure
- `signals.c`: signal handlers for `SIGINT`/`SIGQUIT` with different behavior in parent vs child processes
- `init.c`: shell initialization and reset loop (`reset_data` brings the shell back to a clean state after each command without restarting the process)
- `utils/enviroment.c`: PATH parsing and splitting, PWD/OLDPWD tracking from `envp`
- `main.c`: entry point, argument validation, initial environment setup

---

My teammate **Daniela Torretta** built the executor (`fork`/`pipe`/`dup2`/`execve`), all built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`), the parser, and heredoc execution.
