NOTE: some library procedures (zero? and list) were implemented in sceme in a file called library.scm. This must be loaded (load is an extension we did).

Optional extenions:

Support for ifs without a false case has been added. For example, (if #f 3) is now valid.

Extra built-in functions to support lists have been added. These functions can be seen in lists.scm.

Extra built-in functions to support math functions have been added. These functions can be seen in math.scm. 

"load" function for importing and running another scheme file.
