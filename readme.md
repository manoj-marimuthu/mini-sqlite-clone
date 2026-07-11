## Sqlite Mini Clone 

### current version - v0.0.0 

The following repository is a sqlite-inspired relational 
database engine.Note that the original is massive and the
clone will only implement the important subsets and features.

### Features 

- CREATE
- INSERT
- SELECT (Only * for now)
- DESCRIBE
- meta commands such as .help, .version and .exit (or .quit)
- INT, TEXT (Fixed size)
- REPL-based
- Binary storage (each table gets a .tbl file with schema and rows).

### Future plans 

- Bytecode compilation
- Aggregate functions and other features
- Paging
- B+ Trees (long term goal)

### Author 
Manoj K.M
Written with C and Curiosity