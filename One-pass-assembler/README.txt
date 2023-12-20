								ONE PASS ASSEMBLER

Used High Level language	 :	C++
Used data structure for SYMAB:	linked list using poiners
Example input code			 :	Division by sequential subtraction

Input file 		 : Each line of input file consists of label field, opcode field, operand field.
Object code file : It consists of header file, text files, end file.

code working	:

				 The input file streams are used for taking input from input1.txt file, optab and mnemonic from 
		optab1.txt and output file streams are used for writing oject code and source program with object code 
		into files.
		
				Run  the program, it reads the program line by line and checks the label filed,
		and if it is data fiels, then the location counter increases according to its length
		if it is instruction then location counter is incremented by 3. 
				
				It checks the symtab, if the label is not there, then that label has its entry to symtab with
		corresponding location. If symbol is already present
						1.check weather flag bit is there (if flag bit is there symbol is till not defined,
								now it is defined, object code is written in new text record)
						2.if flag bit is not there(then symbol is already defined and it sets error flag)	
	
		check for operand that corresponding to that label :
						1.If operand is not found then add to symtab and make flag, write object code ad mnemonic
									+0000 ;
						2.If found
							i.if flag is there add address field needed to be modified in that symtab by extending 
									linked list)
							ii. if flag is not there write object code
		
		This process contines for all lines in input.1 file.
		
		used functions   
					1.addsymtab()	 -> to handle instructions
					2.adddatasymtab()-> to handle data members
					
		Final display on consele consists
			1.name of program
			2.length of program
			3.SYMTAB.
				
		In this diaplay of symta, if still flag appears as * , that symbol is not defined.		
						
	NOTE: 1.** in input file means its field is vacant.
		  2.^ symbols between object codes in text file is for understanding purpose				
									
		
								 					
		
