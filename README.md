# OSS json parser ('backup.c' is the final source code!! plz only see the backup.c!!)

1. Understanding on json parser
   - json file is basically text file which is independent from certain programming language

   - it contains various data with designed syntax which is universally promised

2. Process

   - first, read json file in C file
   - consider the whole data as sequence of characters
   - parse them (tokenize the characters based on promised syntax of json file)
   - print meaningful data out in certain form
   - there are some query to find some useful data quickly
   - there is function that makes random matching list

3. Implementation progress

   - complete file read
   - complete json parsing
      1. parser   
             - readfile  
                  - when user gives argument to main as argv, readfile will read all the character and store it as one big chunk of string.  
	      - startParse  
                  - this contains all the scenarios that can come in json file.  
                  - while traversing from top to bottom of the file, startParse function will call various functions for each cases.   
	      - parseString  
                  - all string data type in json file starts and ends with '"' charater. Therefore, when the pointing variable encounters '"' charcater, parseString function will be called.  
                  - it will increase position until next '"' character shows up which indicates the end of string.  
             - parsePrimitive  
                  - primitive includes numbers, ture/false, and null data.  
                  - for primitive values, they are not nested with any character and different data are sepearated by comma.  
                  - so parsing can be implemented by increasing pointing position of string up to next comma or closing bracket, or blank.  
             - parseObject  
                  - object starts and ends with '{', '}'bracket character.  
                  - For object and array, nested object and array must be considered.  
                  - so declare counting and flag variable for correct counting of values in object.  
             - parseArray  
                  - array starts and ends with '[', ']' bracket character.  
                  - just same as object. consider the nested cases.
# Apllication
1. Implementaion progress

	- complete funtions that manage application
		- mystrcmp
			- This function is useful,  this is used for compare certain words and token’s words.
		- extractToken
			- This function extracts the string from certain token
			- We know the range of all token,  just using this index we can convert token to string we can use.
		- printMenu
			- It just print main menu for manager.  
		- wrongInput
			- It just print error message for manager.
		- printTokens
			- It just print all token information.
	- complete functions of Application
		- printTokens
			- It just print all token information.
		- randomMatching
			- Using rand function, it generates random matching list. 
			- One male and one female are matched.
			- If male is more than female, some male can’t make matching female.
		- seeClient
			- If manager types the name, this function lists the information about client who have that name.
		- seeClientbyGenger
			- If manager types the gender, this function lists all client by gender
		- seeClientby]Hobby
			- If manager types the age by number, this function lists all clients by exist hobby in clients information
		- seeClientsbyAge
			- If manager types the age by number, this function lists all clients under that age.	
		- seeClientsbyCompany
			- If manager types the company, this function lists all client with that commay
		- seeClientinbyEmail
			- If manager types the Email, this function find cilent with that e-mail, and show the entire information.
		- seeClientbyLocalInfo
			- If manager types Area Code in phone number, this functions shows clients with the given Area code. 

2. Project Description
	- Our Application is program that manage meeting application
		- Use Json file of clients informations
		- There are two main mode, one is Admin mode, and the other is client mode
		- Admin mode can see all clients by each information
		- Client mode can find person who want to dating
	- This Application is useful to person who want to dating
		- Users can find their date mate with information such as hobbies and ages.
		- They can also find date mate completely random.
3. How to start this App
	- First, you should chose one of two mode(Admin, client)
	- If chose 'Admin'
		- Frist you have to enter password
		- In Admin mode, you can see all of clients informations.
		- There are functions that can be retrieved under various conditions.
	- If chose 'client'
		- First you enter your private number.
		- And then, conditions about your interest.
		- What you get is randomly chosen(with conditions fulfilled) person’s name. 
		- If you are too ‘picky’ person so, if there’s no match.
		- Since you got name of other client you can get information about that client, Name, Job, Email, and phone number.

4. More information or Help
	- If you want more information or need help, plz send email
		- 21500426@handong.edu
		- 21300186@handong.edu
		- 21500050@handong.edu
		- 21400738@handong.edu





