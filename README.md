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
      1. print all parsing data
      2. random matching
      3. see client information
      4. see client by genger
      5. see client by hobby
      6. see clients by age limit
      7. see clients by company
      8. see client information by e-mail
      9. see client information by Area Code from phone number
