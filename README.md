# Homophonic-Substitution-Cipher
Created a homophonic substitution cipher and proposed and attack on it.

 The handycipher is based on the homophonic substitution cipher, which basically means that, instead of having to substitute one char for each letter,
 there is a choice of characters to choose from to subsitute.
 This confuses the encrypted text more.


 Following is the implementation of a homophonic cipher on which the handycipher is based.
 There are three files we deal with:
 filewithkey.txt
 plaintextfile.txt
 En_Dec_textfile.txt
 
 Please do the following to compile and run: cc handycipher_265.c -o handycipher_265

handycipher_265 -e filewithkey.txt plaintextfile.txt En_Dec_textfile.txt to encrypt
Then check the En_Dec_textfile.txt for encrypted text
 
handycipher_265 -d FileWithKey.txt plaintextfile.txt En_Dec_textfile.txt to decrypt
 Then check En_Dec_textfile.txt again for the decrypted text
 
 filewithkey is a file which contains the numbers that replace the letters
 in PlainTextFile.
 
 
 There are 26 lines in the FileWithKey, each for every letter in the alphabet.
 Every line starts with a number denoting the number of numbers on the line.
 The rest of the line is a choice of characters to choose from as the actual substituion.
 
 
 The program randomly chooses a seed value based on the time stamp of the particular period and uses that to randomly choose from the list of values
