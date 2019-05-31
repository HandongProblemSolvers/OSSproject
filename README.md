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
      1. parser
            - readfile  
                  a. when user gives argument to main as argv, readfile will read all the character and store it as one big chunk of string.  
	      - startParse  
                  a. this contains all the scenarios that can come in json file.  
                  b. while traversing from top to bottom of the file, startParse function will call various functions for each cases. 
	      - parseString  
                  a. all string data type in json file starts and ends with '"' charater. Therefore, when the pointing variable encounters '"' charcater, parseString function will be called.  
                  b. it will increase position until next '"' character shows up which indicates the end of string.  
            - parsePrimitive  
                  a. primitive includes numbers, ture/false, and null data.  
                  b. for primitive values, they are not nested with any character and different data are sepearated by comma.  
                  c. so parsing can be implemented by increasing pointing position of string up to next comma or closing bracket, or blank.  
            - parseObject  
                  a. object starts and ends with '{', '}'bracket character.  
                  b. For object and array, nested object and array must be considered.  
                  c. so declare counting and flag variable for correct counting of values in object.  
            - parseArray  
                  a. array starts and ends with '[', ']' bracket character.  
                  b. just same as object. consider the nested cases.  
      2. print all parsing data
      3. random matching
      4. see client information
      5. see client by genger
      6. see client by hobby
      7. see clients by age limit
      8. see clients by company
      9. see client information by e-mail
