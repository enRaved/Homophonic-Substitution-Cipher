//  main.c
//  HandyCipher
//
//  Created by Ravee Khandagale on 9/8/16.
//  Copyright © 2016 Ravee Khandagale. All rights reserved.


/*-----------------------------------------------------------------------------------
 Homophonic Substitution Cipher
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

---------------------------------------------------------------------------------------
 */



#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <time.h>

#include <string.h>

#define MAXIMUM_NUMLEN 20


typedef char num[MAXIMUM_NUMLEN];
// user defined type: KType
typedef struct{
    int n; // number of numbers on the given line used for substitution for the homophonic cipher.
    num *numbers; // actual array of numbers used for subsitution of each letter.
}KType;

typedef KType key[26];

void readMe(char *);

void readKey(FILE *,key);

char *get_Number(char,key);

char getLetter(char *,key);
//These are forward function defintions of the encryption and decryption functions.
//I have defined them to be at the start of the system to emphasize the purpose of this program.

//This does a homophonic encryption, one letter is substituted with any of the numbers or combination of numbers at random
void encryptFile(FILE *PlainTextFile, FILE *En_Dec_Text_File, key KeyArray) //En_Dec_Text_File will be updated with the encrypted/ decrypted data
{
    int StreamingChar;
    
    
    while((StreamingChar=getc(PlainTextFile))!=EOF){
        
        if(isalpha(StreamingChar)){
            
            fprintf(En_Dec_Text_File,"%s ",get_Number(tolower(StreamingChar),KeyArray)); //get corresponding number from key array and write to file with encrpted text
     //all letters are coverted to lower case for the sake of convenience
        }
        else if(isdigit(StreamingChar)){
            
            fprintf(En_Dec_Text_File,"{%c}",StreamingChar); //if digit, directly write to file
        }
        else if(StreamingChar=='\n'){
            
            fprintf(En_Dec_Text_File," -1\n"); // mark of a new line
        }
        
        else{
            
            fprintf(En_Dec_Text_File,"%c",StreamingChar);}
    }
}


// decryptFile decrypts PlainTextFile and writes back to En_Dec_Text_File


void decryptFile(FILE *PlainTextFile, FILE *En_Dec_Text_File, key KeyArray)
{
    int StreamingChar,position,Lcharacter,Xcharacter;
    char BLOCK[100],Pcharacter;
    
    while((StreamingChar=getc(PlainTextFile))!=EOF){
        
        position=0;
        
        if(isdigit(StreamingChar)){                       //decrytpting a stream of encrypted text
            BLOCK[position++]=StreamingChar;
            StreamingChar=getc(PlainTextFile);
            
            while((isdigit(StreamingChar))&&(StreamingChar!=EOF)){
                BLOCK[position++]=StreamingChar;
                StreamingChar=getc(PlainTextFile);
            }
            
        if(StreamingChar!=' ')
                ungetc(StreamingChar,PlainTextFile); // tokenising to get next character, whitespaces are separators between characters.
           
        BLOCK[position]='\0';
            
        Pcharacter=getLetter(BLOCK,KeyArray);
            
        if(Pcharacter) putc(Pcharacter,En_Dec_Text_File);
            
        else fprintf(En_Dec_Text_File,"Can't Find!");
            
        }
        
        else if(StreamingChar=='-'){                      //check for new line and end of line
            Lcharacter=getc(PlainTextFile);
            if(Lcharacter=='1'){
                Xcharacter=getc(PlainTextFile);
                if(Xcharacter=='\n'){
                    fprintf(En_Dec_Text_File,"\n");
                    continue;
                }
                else {
                    ungetc(Xcharacter,PlainTextFile);
                    ungetc(Lcharacter,PlainTextFile);
                }
            }
            else{
                ungetc(Lcharacter,PlainTextFile);
                putc(StreamingChar,En_Dec_Text_File);
                continue;
            }
            putc(StreamingChar,En_Dec_Text_File);
        }
        
        else if(StreamingChar=='{'){                //any plaintext digit is denoted in {} as a character, recognises that here.
            Lcharacter=getc(PlainTextFile);
            if(isdigit(Lcharacter)){
                Xcharacter=getc(PlainTextFile);
                if(Xcharacter=='}'){
                    putc(Lcharacter,En_Dec_Text_File); //puts this in the output file which will now contain the decrypted text
                    continue;
                }
                else{
                    ungetc(Xcharacter,PlainTextFile);
                    ungetc(Lcharacter,PlainTextFile);
                }
            }
            else {
                ungetc(Lcharacter,PlainTextFile);
                putc(StreamingChar,En_Dec_Text_File);
                continue;
            }
            putc(StreamingChar,En_Dec_Text_File);
        }
        
        else putc(StreamingChar,En_Dec_Text_File);
    }
    
    
}






int main(int argc,char *argv[])
{
    key KeyArray;
    
    int i,j;
    int FLAG_ENC=1;
    
    int SWITCH=0;
    FILE *FileWithKey, *PlainTextFile, *En_Dec_Text_File;
    
    
    
    
    /* Check arguments and open files */
    
    
    if((argc!=4)&&(argc!=5))
        readMe(argv[0]);
    else{
        if((argv[1])[0]=='-'){
            SWITCH=1;
            if(strlen(argv[1])!=2)
                readMe(argv[0]);
            else {
                if(tolower((argv[1])[1])=='e');
                else if(tolower((argv[1])[1])=='d')
                    FLAG_ENC=0;
                else readMe(argv[0]);
            }
        }
        if(SWITCH&(argc!=5)) readMe(argv[0]);
        
        //read-write file calls
        
        if((FileWithKey=fopen(argv[1+SWITCH],"r"))==NULL){
            fprintf(stderr,"Error opening key file.\n");
            exit(1);
        }
        
        if((PlainTextFile=fopen(argv[2+SWITCH],"r"))==NULL){
            fprintf(stderr,"Error opening input file.\n");
            exit(1);
        }
        
        if((En_Dec_Text_File=fopen(argv[3+SWITCH],"w"))==NULL){
            fprintf(stderr,"Error opening output file.\n");
            exit(1);
        }
    }
    
    readKey(FileWithKey,KeyArray);
    fclose(FileWithKey);
    
 
    srandom(time(NULL));                     // pseudo random number generator used to seed aa value, so as to pick random value for encrytion
    
    if(FLAG_ENC) encryptFile(PlainTextFile,En_Dec_Text_File,KeyArray);
    else decryptFile(PlainTextFile,En_Dec_Text_File,KeyArray);
    
    fclose(PlainTextFile);
    fclose(En_Dec_Text_File);
}




void readMe(char *name)
{
    
    printf("readMe: %s -e FileWithKey PlainTextFile En_Dec_Text_File\n",name);
    printf("\n-e is the encrypt mode\n");
    printf("\n-d is the encrypt mode\n");
    printf("\nFileWithKey has 26 lines for 26 alphabets\n");
    printf("/nEach line in KeyWithFile choices of substitutions for each letter");
    
   
    exit(1);
}




//this returns the pointer to a string of nums used to replace a particular character. This is picked randomly.
char *get_Number(char letter,key KeyArray)
{
    int tempr;
    char *result;
    tempr=random();
    tempr%=KeyArray[letter-'a'].n;
    result=(char*)KeyArray[letter-'a'].numbers[tempr];
    return(result);
}



//performs linear search to return correspondng character, else returns 0.
char getLetter(char *number,key KeyArray)
{
    int i,j;
    
    for(i=0;i<26;i++)
        for(j=0;j<KeyArray[i].n;j++)
            if(!(strcmp(number,KeyArray[i].numbers[j])))
                return(i+'a');
    
    return('\0');
    
}


//This reads from FileWithKey
//results are written in KeyArray

void readKey(FILE *FileWithKey,key KeyArray)
{
    int i,j,n;
    
    for(i=0;i<26;i++){
        fscanf(FileWithKey,"%d",&n);
        KeyArray[i].n=n;
        KeyArray[i].numbers=calloc(n,sizeof(num));
        for(j=0;j<n;j++)
            fscanf(FileWithKey,"%s",KeyArray[i].numbers[j]);
    }
}















