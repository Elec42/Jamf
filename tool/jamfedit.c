/* Jamfedit - Felix Kröhnert; 18.12.2021; 08:15


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "template.t"


#define VersionD "\033[1mJamfedit by Felix Kröhnert (compiled "__DATE__" "__TIME__") - edit:FSGVersion\n\033[m"
#define Version "2.0\n\033[m"

#define HELPTEXT VersionD"\
\033[mUsage: jamfedit [arguments]\n\
	 or: jamfedit -C [Filename] [Arguments]		creates a new File [Filename].mobileconf\n\
	 or: jamfedit -M [Filename] [Arguments]		modifies an existing File [Filename]\n\
	 or: jamfedit --get [Arguments]			Prints requested Attributes\n\
\n\
Arguments:\n\
	-C [FILE]				Creates a new File [FILE].mobileconfig (or overwrites old one) using the default template [FSG]\n\
	-M [FILE]				Modifies an existing File [FILE]\n\
	--get [FILE]			Retrieving instead of setting\n\
	-u [userId]				Specifies the userId to be set [in the CheckIn URL]\n\
	-l [locationId]			Specifies the locationId to be set [in the CheckIn URL]\n\
	-c [companyId]			Specifies the companyId to be set [in the CheckIn URL]\n\
	-e [enrollmentId]		Specifies the enrollmentId to be set [in the ChekIn URL]\n\
	-cRem [boolean]			Specifies whether to log out when profile is removed\n\
	-accR [Id]				Specifies the access rights\n\
	-dName [displayName]	Sets the displayName -> Defaults to [FILE]\n\
	-pName [displayName]	Sets the payload display name\n\
	-pDesc [description]	Sets the payload description\n\
	-PayPass [password]		Sets current payload password\n\
	-PayType				Gets payload type\n\
	-v						Verbous\n\
	-h or --help			Print Help (this message) and exit\n\
	--version				Print version information and exit\n\
"

void processArgument(char **);
void erroredArg(char *);
void createFile(void);
void insertInFile(int, char *, FILE *);
void cutFromFile(int, int, FILE *);
int findInFile(char *, FILE *);

int i, argc;

char *fileName=NULL;

//attr
int CreatMod = -1;
int userId = -1;
int locationId = -1;
int companyId = -1;
int enrollmentId = -1;
int cRem = -1;
int accR = -1;
char *dName = (char*)-1;
char *pName = (char*)-1;
char *pDesc = (char*)-1;
char *PayPass = (char*)-1;
int PayType = -1;
int verbous = 0;

FILE *inputFile;

int main(int dargc, char *argv[]) {
	
	argc = dargc;

	if(argc<2) {
		printf("\033[1;31mExpecting at least on parameter\nUse '%s -h' for more information\n\033[m", argv[0]);
		return 1;	
	}

	for(i=1; i<argc; i++) {
		processArgument(argv);
	}

	if(verbous) {
		printf("CreatMod: %d\n", CreatMod);
		printf("UserId: %d\n", userId);
		printf("LocationId: %d\n", locationId);
		printf("CompanyId: %d\n", companyId);
		printf("EnrollmentId: %d\n", enrollmentId);
		printf("cRem: %d\n", cRem);
		printf("accR: %d\n", accR);
		printf("dName: %ld\n", (intptr_t)dName); 
		printf("pName: %ld\n", (intptr_t)pName); 
		printf("pDesc: %ld\n", (intptr_t)pDesc); 
		printf("PayPass: %ld\n", (intptr_t)PayPass);
		printf("PayType: %d\n", PayType);
		printf("Verbous: %d\n", verbous);
	}
	if(CreatMod==0)
		printf("Writing to %s\n", fileName);
	else if(CreatMod==1)
		printf("Writing to %s\n", fileName);
	else if(CreatMod==2)
		printf("Reading from %s\n", fileName);
	else
		erroredArg("\033[1;31mNo File specified\n\033[m");

	

	return 0;
}


void processArgument(char **argv) {
	int d=1;
	if(argc>i+1 || CreatMod==2) {
		if(strcmp(argv[i], "-C") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '-C' after '-M' or '--get'\n\033[m");
			}
			CreatMod=0;
			char *str=malloc(strlen(argv[i+1])+13);
			sprintf(str, "%s.mobileconfig", argv[i+1]);
			fileName = malloc(strlen(str));
			strcpy(fileName, str);
			inputFile = fopen(str, "w+");
			if(inputFile==NULL)
				erroredArg("Could not create File");
			createFile();
			i++;
				
		}
		else if(strcmp(argv[i], "-M") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '-M' after '-C' or '--get'\n\033[m");
			}
			CreatMod=1;
			fileName = malloc(strlen(argv[i+1]));
			strcpy(fileName, argv[i+1]);
			inputFile = fopen(argv[i+1], "r+");
			if(inputFile==NULL)
				erroredArg("Could not open File");
			i++;
		}
		else if(strcmp(argv[i], "--get") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '--get' after '-M' or '-C'\n\033[m");
			}
			if(i!=1)
				erroredArg("\033[1;31mInvalid option - Has to be the first argument\n\033[m");
			CreatMod=2;
			fileName = malloc(strlen(argv[i+1]));
			strcpy(fileName, argv[i+1]);
			inputFile = fopen(argv[i+1], "r");
			if(inputFile==NULL)
				erroredArg("Could not open File");
			i++;
		}

		else if(strcmp(argv[i], "-u") == 0) {
			if(CreatMod!=2) {
				userId = atoi(argv[i+1]);
				i++;
			}
			else {
				userId = -2;
			}
		}
		else if(strcmp(argv[i], "-l") == 0) {
			if(CreatMod!=2) {
				locationId = atoi(argv[i+1]);
				i++;
			}
			else {
				locationId = -2;
			}
		}
		else if(strcmp(argv[i], "-c") == 0) {
			if(CreatMod!=2) {
				companyId = atoi(argv[i+1]);
				i++;
			}
			else {
				companyId = -2;
			}
		}
		else if(strcmp(argv[i], "-e") == 0) {
			if(CreatMod!=2) {
				enrollmentId = atoi(argv[i+1]);
				i++;
			}
			else {
				enrollmentId = -2;
			}
		}
		else if(strcmp(argv[i], "-accR") == 0) {
			if(CreatMod!=2) {
				accR = atoi(argv[i+1]);
				i++;
			}
			else {
				accR = -2;
			}
		}
		else if(strcmp(argv[i], "-cRem") == 0) {
			if(CreatMod!=2) {
				if(strcmp(argv[i+1], "true")) cRem=1;
				else cRem=0;
				i++;
			}
			else {
				cRem = -2;
			}
		}
		else if(strcmp(argv[i], "-dName") == 0) {
			if(CreatMod!=2) {
				dName = malloc(strlen(argv[i+1]));
				strcpy(dName, argv[i+1]);
				i++;
			}
			else {
				dName = (char*)-2;
			}
		}
		else if(strcmp(argv[i], "-pName") == 0) {
			if(CreatMod!=2) {
				pName = malloc(strlen(argv[i+1]));
				strcpy(pName, argv[i+1]);
				i++;
			}
			else {
				pName = (char*)-2;
			}
		}
		else if(strcmp(argv[i], "-pDesc") == 0) {
			if(CreatMod!=2) {
				pDesc = malloc(strlen(argv[i+1]));
				strcpy(pDesc, argv[i+1]);
				i++;
			}
			else {
				pDesc = (char*)-2;
			}
		}
		else if(strcmp(argv[i], "-PayPass") == 0) {
			if(CreatMod!=2) {
				PayPass = malloc(strlen(argv[i+1]));
				strcpy(PayPass, argv[i+1]);
				i++;
			}
			else {
				PayPass = (char*)-2;
			}
		}
		else
			d=0;
	}
	else d=0;
	if(d==0) {
		if(strcmp(argv[i], "-v") == 0) {
			verbous = 1;	
		}
		else if(strcmp(argv[i], "-PayType") == 0) {
			PayType = 1;
		}
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf(HELPTEXT);
			exit(0);
		}
		else if(strcmp(argv[i], "--version") == 0) {
			printf(VersionD Version);
			exit(0);
		}
		else {
			printf(VersionD"Unknown argument '%s'\nMore info with: 'jamfedit -h'\n", argv[i]);
			exit(2);
		}
	}
}



void erroredArg(char *str) {
	printf("%s\n", str);
	exit(1);
}



void createFile() {
	
	for(int d=0; d<sizeof(template); d++) {
		fwrite(&template[d], 1, 1, inputFile);
	}
	fseek(inputFile, SEEK_SET, 0);
}



void insertInFile(int index, char *str, FILE *fp) {
	fseek(fp, 0, SEEK_END);
	if(index<0 || index > ftell(fp)) return;
	fseek(fp, index, SEEK_SET);
	
	char buf1[strlen(str)], buf2[strlen(str)];
	int rl, sl=strlen(str);
	strcpy(buf1, str);

	do {
		rl=fread(buf2, 1, sl, fp);
		buf2[rl]='\0';
		fseek(fp, -rl, SEEK_CUR);
		fwrite(buf1, 1, strlen(buf1), fp);
		strcpy(buf1, buf2);
	} while(rl>0);	

	fseek(fp, index+sl, SEEK_SET);
}


void cutFromFile(int start, int end, FILE *fp) {

	if(start>=end||start<0) return;
	
	int rl=0, len=end-start;
	char buf[len];

	fseek(fp, start+len, SEEK_SET);
	
	do {
		int b=ftell(fp);
		rl = fread(buf, 1, len, fp); 
		fseek(fp, b-len, SEEK_SET);
		fwrite(buf, 1, rl, fp);
		fseek(fp, b+rl, SEEK_SET);
	} while(rl>0);
	fseek(fp, 0, SEEK_END);
	ftruncate(fileno(fp), ftell(fp)-len);
	fseek(fp, -len, SEEK_CUR);

}

int findInFile(char *str, FILE *fp) {
	fseek(fp, -strlen(str), SEEK_END);
	int pos=ftell(fp);
	int len = strlen(str);
	char buf[len];

	while(pos==ftell(fp)) {
		int rl=fread(buf, 1, len, fp);
		pos+=rl;
		buf[rl]='\0';
		if(strcmp(buf, str) == 0) return ftell(fp);
		fseek(fp, - len - 1, SEEK_CUR);
		pos = pos - len - 1;
	}
	return -1;
}
