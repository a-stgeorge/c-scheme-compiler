Scheme interpreter written in c. Does not include all the subtleties of the scheme language, but captures quite a few. Final project submission for COS371, Organization of Programming Languages. Collaborative effort between Isaac Kubas, Jon Jagt, and Aidan St. George. A list of extensions completed for the project can be found below. Test cases for the final product are labeled as test.eval.input.xx, with a sample output labeled as test.eval.output.xx.


NOTE: some library procedures (zero? and list) were implemented in sceme in a file called library.scm. This must be loaded (load is an extension we did).


OPTIONAL EXTENSIONS:
 1) Support for ifs without a false case has been added. For example, (if #f 3) is now valid.
 2) Extra built-in functions to support lists have been added. These functions can be seen in lists.scm.
 3) Extra built-in functions to support math functions have been added. These functions can be seen in math.scm. 
 4) "load" function for importing and running another scheme file.
