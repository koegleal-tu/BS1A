#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define MISSINGPATH "Path to output file"
#define MYMAPSIZE 28
#define MAXWORDLENGHT 300

const char * getCipher(char);
const char getPlain(char *);
int encryptM(void);
int decryptM(void);

//Array wich contains char pointers, set MYMAPSIZE to correct amount of elements, set MAXWORDLENGHT to the longest word + 2 which is in the mapping
char * myMap[] = {
	"der","die","das","wer","wo",
	"ihr", "mir", "hin", "zur", "klo",
	"bau", "rau", "sau", "lau", "tau",
	"weg", "eck", "nah", "her", "nun",
	"rot", "tot", "bim", "nur", "mur",
	"Fuchs", "Hase", "Gans",
};

int main(int argc, char *argv[])
{

	//counts option o
	int opt_o = 0;
	//arg of o, initialized with null!
	char* opt_path = NULL;
	FILE *outpFile = NULL;
	//0 if option f occures
	int opt_f = 0;
	int opt_h = 0;

	int c;
	//-o option with argument, -f and -h without
	c = getopt(argc, argv, "o:fh");
	while(c != -1)
	{
		switch(c)
		{
		case 'f':
			opt_f = 1;
		break;
		case 'o':
			opt_o++;
			opt_path = optarg;
		break;
		case 'h':
			opt_h = 1;
		break;
		
		}
		c = getopt(argc, argv, "o:fh");
	}
	//its allowed to set either f or h
	if((opt_f && opt_h) || (opt_f != 0 && opt_h != 0)){
		(void) printf("Invalid option, either use f or h\n"); 
		return 0;
	}

	//set stdout to file
	if(opt_o == 1){
		//if argument is null, exit
		if(opt_path == NULL){
		(void) printf("Argument missing: %s\n", MISSINGPATH); 
		return 0;}
		outpFile = fopen(opt_path, "a");
		stdout = outpFile;
		if(outpFile == NULL){
		(void) printf("Couldnt open output file: %s\n", opt_path); 
		return 0;
		}
	}

	if(opt_h != 0)
	{
		(void) encryptM();
	}else{
		(void) decryptM();
	}

	//close if output file used
	if(opt_o == 1 && outpFile != NULL){
		(void) fclose(outpFile);
	}
}

const char * getCipher(char plainC)
{
	//dot and space
	if (plainC == 0x2E){return myMap[27];}
	if (plainC == 0x20){return myMap[26];}
	//non capital to capital letters, finally cast them to specific array index
	if(plainC >= 0x61 && plainC <= 0x7A){plainC-=32;}
	if(plainC >= 0x41 && plainC <= 0x5A){plainC-=65; return myMap[(int)plainC];}

	(void) printf("Character %c doesn\'t exist in mappin. %s used instead.", plainC, myMap[0]);
	return myMap[0];
}


const char getPlain(char * cipher)
{
	int i = 0;
	while(i < MYMAPSIZE)
	{
		if(strcmp(myMap[i], cipher) == 0)
		{
			//dot and space
			if(i == 27){return 0x2E;}
			if(i == 26){return 0x20;}
			//letters
			return i + 65;
		}
		i++;
	}
	const char * err = ("String \"%i\" not found in mapping, character\'?\' used instead", (int)cipher)
	(void) fputs("String \"%i\" not found in mapping, character\'?\' used instead", (int)cipher, stderr);
	return '?';
}

int encryptM(void)
{
	int cnt = 0;
	char buffer[MAXWORDLENGHT];
	int i = 0;
	while(fgets(buffer,MAXWORDLENGHT,stdin) != NULL && i > -1)
	{
		i = 0;
		while(i < MAXWORDLENGHT && buffer[i] != '\n' && buffer[i] != EOF && buffer[i] != '\0'){
			(void) fputs(getCipher(buffer[i]), stdout);
			if(i % 10 == 0){
			(void) fputs(".", stdout);
			}else{
			(void) fputs(" ", stdout);}
			i++;
		}
		//reads until EOF or ENTER-key Pressed
		if( buffer[i] == '\n' || buffer[i] == EOF)
		{
			i = -1;
		}
	}
	return cnt;
}

int decryptM(void)
{
	//return counter
	int cnt = 0;

	char buffer[MAXWORDLENGHT];	
	int bufPos = 0;
	//pointer to the beginning of current word
	char * lwStart = &buffer[0];
		
	//exits function on error
	if (fgets(buffer,MAXWORDLENGHT,stdin) == NULL){
		return 0;
	}

	while(bufPos > -1 && bufPos < MAXWORDLENGHT)
	{		
		//end of word, processed in the if statement after the following one
		if(buffer[bufPos] == ' ' || buffer[bufPos] == '.'){
			buffer[bufPos] = '\0';			
		}

		//output of word
		if(buffer[bufPos] == '\0'){
			
			if(lwStart != &buffer[bufPos]){
			(void) fputc(getPlain(lwStart), stdout);cnt++;}
			

			//if this was not the last word, the pointer is set to the begin of the following word
			if(bufPos+1 < MAXWORDLENGHT && buffer[bufPos+1] > 0){
				lwStart = &buffer[bufPos + 1];}			
		}
		
		//End of file, output of last word
		if(buffer[bufPos] == EOF){
			buffer[bufPos] = '\0';
			if(lwStart != &buffer[bufPos]){			
			(void)fputc(getPlain(lwStart), stdout);}			
			return cnt++;}
		bufPos++;
	}
	return cnt;
}
/*
h
h
h
h
h
*/
