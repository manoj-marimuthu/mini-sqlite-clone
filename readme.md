## Mini RDBMS

#### current version - v0.0.0 

an sqlite-inspired relational database engine written from scratch in C which I built
to learn more about databases, their architecture, different design choices and tradeoffs. Started as
educational, but may evolve if time permits.

### Overview

This database consists of an SQL-like language interpreter that interprets input from the REPL using a custom lexer for tokenizing, a recursive descent parser for building the AST and finally the executor which runs the statements. The storage layer is handled by the executor. Tables are stored in ```.tbl``` files and contain information such as schema, column names, column metadata and of course, the rows. The contents are serialized and written and manipulated as binary. 

### Build instructions

- Clone the repository
```shell
git clone https://www.github.com/manoj-marimuthu/mini-rdbms
```

- Use the makefile
```shell
make
```
- Remember that the command might differ based on the compiler or Operating system. For example-
```shell
mingw32-make
```
- Use a C compiler such as mingw
### Features 

- CREATE
- INSERT
- SELECT (Only * for now)
- DESCRIBE
- meta commands such as .help, .version and .exit (or .quit)
- INT, TEXT (Fixed size)
- REPL-based
- Binary storage (each table gets a .tbl file with schema and rows).

### Examples

- A create a table with a specific schema use the ```CREATE``` keyword. Example-
```sql
CREATE TABLE example(field1 INT,field2 TEXT);
```
- To insert into a table,
```sql
INSERT INTO example(10,"Some random text");
```

- To view the table
```sql
SELECT * FROM example;
```
- For now only ```*``` is supported and new features will be added in v0.1.0.
- To see the table structure,
```sql
DESCRIBE example;
```
- The above displays column names, the data type it accepts and the size (in bytes).
- Meta commands
<table>
  <tr>
      <td>Command</td>
      <td>What it does ? </td>
  </tr>
  <tr>
      <td>.exit or .quit</td>
      <td>exits the database shell </td>
  </tr>
  <tr>
      <td>.help</td>
      <td>Displays all supported features for current version </td>
  </tr>
  <tr>
      <td>.version</td>
      <td>Displays DB version </td>
  </tr>
</table>

### Datatypes

- ```INT``` - holds integer (no negatives / decimals yet, I was lazy to add them since my goal was to build a working version first and then progress it with features)

- ```TEXT``` - 32 byte string, fixed size (Will be variable size in the upcoming versions after paging).
### Future plans 

- Bytecode compilation
- Aggregate functions, more datatypes and variable sizing.
- Paging
- B+ Trees (long term goal)

### Author 
Manoj K.M
Written with C and Curiosity
