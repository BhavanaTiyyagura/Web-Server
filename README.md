# A Simple Web Server in C 

A simple web server that returns files on a certain endpoint.

* Any URL should map to the current `src` directory and files that lie within. For example:

  ```
  http://localhost:8080/index.html
  ```

  should serve the file

  ```
  ./index.html
  ```

The `src` folder has files called `Work.html` and `index.html` for testing purposes. Any other html files can replace this. 

### To Build and Run:

There is a `Makefile` provided. On the command line, type `make` to build the server.

Type `./server` to run the server.

### 

