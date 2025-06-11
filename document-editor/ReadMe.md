# Document editor

* User enters commands in a command line
* Commands allow to edit and print document on the screen
* Supported commands:
  
  - Exit - exits the app
    
  - Print - prints document on a screen

    ```
    > Enter a command:
    Print
    ---
    Hello World!!!
    ---
    ```
  - AddLine
    - prompts the user to write a line that will be appended to a document

    ```
    > Enter a command:
    AddLine
    Add new line:
    This is TDD training for C++ programmers
    > Enter a command:
    Print
    ---
    Hello World!!!
    This is TDD training for C++ programmers
    ---
    ```

  - ToUpper 
    - changes letters to uppercase

    ```
    > Enter a command:
    ToUpper
    > Enter a command:
    Print
    ---
    HELLO WORLD!!!
    THIS IS TDD TRAINING FOR C++ PROGRAMMERS
    ---
    ```

  - ToLower
    - changes letters to lowercase

  - Copy
    - copies the whole content of document to a clipboard

  - Paste
    - appends content of a clipboard to a document

* Unknown command prints a message

  ```
  > Enter a command:
  > Cmd
  > Unknown command: Cmd
  > Enter a command:
  ```