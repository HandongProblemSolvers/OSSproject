# OSS json parser

1. understanding on json parser

   - json file is basically text file which is independent from certain programming language

   - it contains various data with designed syntax which is universally promised

2. process

   - first, read json file in C file
   - consider the whole data as sequence of characters
   - parse them (tokenize the characters based on promised syntax of json file)
   - print meaningful data out in certain form
   - there are some query to find some useful data quickly
   - there is function that makes random matching list

3. implementation progress

   - complete file read
   - complete json parsing
   - complete 8 available functions for application
      1. parser
      
         1. 1. 1. readfile
      
               2. 1. When user enters filename as standard input, readfile function first reads all character and store it as one big chunk of string.
                  2. If it successfully reads the json file, pass it to startParse function so that program initiate parsing procedure.
      
               3. startParse
      
               4. 1. This function contains all cases that can happen when each character shows up while traversing the whole data of json file.
                  2. For now, there are six cases which are : object, array, string, primitive, boolean, and ‘:’ case for adding size of pairs.
      
               5. parseString
      
               6. 1. In json file, string type starts and ends with “ symbol. So this parseString function reads from next position of “ up to next “ symbol. While reading it, it increases position index.
                  2. After done reading, save start and end point of token, increases token index and total token size.
      
               7. parsePrimitive
      
               8. 1. parsePrimitive incluedes all cases of number, boolean, and null type data. These are comma-separated data in json syntax.
                  2. Same as parseString function’s logic, it read from ‘,’ symbol to next ‘,’ symbol.
                  3. After done reading, save start and end point of token, increases token index and total token size
      
               9. parseObject
      
               10. 1. Object comes with ‘{‘ symbol and there may be multiple, nested objects. Also, array can come inside of object.
                   2. Since object itself is token, increase size by 1.
                   3. Until find ‘}’ symbol, read json data and change cnt and flag variable when it encounters ‘{‘ or ‘[‘ so that it can distinguish nested object and array.
                   4. Also, it can’t just increases size when encounters comma but must be ensured that flag is 0 which means it’s not data in nested object or array.
      
               11. parseArray
      
               12. 1. This is basically same as parseObject but just differs from the symbol. It stops when finds ‘]’ instead of ‘}’ symbol in loop.
      
      2. print all parsing data
      
      3. random matching
      
      4. see client information
      
      5. see client by genger
      
      6. see client by hobby
      
      7. see clients by age limit
      
      8. see clients by company
      
      9. see client information by e-mail
