/* Jamfedit - Felix Kröhnert; 18.12.2021; 08:15


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "template.t"


#define VersionD "\033[1mJamfedit by Felix Kröhnert (compiled "__DATE__" "__TIME__") - edit:FSGVersion\n\033[m"
#define Version "2.4\n\033[m"

#define HELPTEXT"\
\033[mUsage: jamfedit [arguments]\n\
	 or: jamfedit -C [Filename] [Arguments]		creates a new File [Filename].mobileconf\n\
	 or: jamfedit -M [Filename] [Arguments]		modifies an existing File [Filename]\n\
	 or: jamfedit -G [Arguments]			Prints requested Attributes\n\
\n\
Arguments:\n\
	-C [FILE]				Creates a new File [FILE].mobileconfig (or overwrites old one) using the default template [FSG]\n\
	-M [FILE]				Modifies an existing File [FILE]\n\
	-G [FILE]				Retrieving instead of setting\n\
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
	-all					Gets all Attributes\n\
	-v						Verbous\n\
	-h or --help			Print Help (this message) and exit\n\
	--version				Print version information and exit\n\
"

void processArgument(char **);
void erroredArg(char *);
void createFile(void);
void insertInFile(int, char *, FILE *);
void cutFromFile(int, int, FILE *);
int findInFile(char *, FILE *, int);
int getPosUrlContentString(char *, int*, FILE*, int);
int getPosFromKey(char *, int*, FILE*, int);
int getPosFromBooleanKey(char *, int*, FILE*, int);
int inList(char, char[]);
void reader();
void writer();
int i, argc;

char *fileName=NULL;

//attr
char *CreatMod =(char*)-1;
char *userId =(char*)-1;
char *locationId =(char*)-1;
char *companyId =(char*)-1;
char *enrollmentId =(char*)-1;
char *cRem =(char*)-1;
char *accR =(char*)-1;
char *dName = (char*)-1;
char *pName = (char*)-1;
char *pDesc = (char*)-1;
char *PayPass = (char*)-1;
char *PayType =(char*)-1;
int verbous = 0;

FILE *inputFile;

int main(int dargc, char *argv[]) {
	printf(VersionD);
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
		printf("dName: %d\n", (intptr_t)dName); 
		printf("pName: %d\n", (intptr_t)pName); 
		printf("pDesc: %d\n", (intptr_t)pDesc); 
		printf("PayPass: %d\n", (intptr_t)PayPass);
		printf("PayType: %d\n", PayType);
		printf("Verbous: %d\n", verbous);
	}
	if(CreatMod==0 || CreatMod==1) {
		if(CreatMod==0) {
			ftruncate(fileno(inputFile), 0);
			createFile();
			printf("\033[1;33mCreated new File\n\033[m");
		}
		writer();
	}
	else if(CreatMod==2) {
		reader();
	}
	else
		erroredArg("\033[1;31mNo File specified\n\033[m");



	return 0;
}


void reader() {
	printf("\033[1;33mReading from %s\n\033[m", fileName);

	int pos, len;
	char *input;

	if(userId == -2) {
		pos=getPosUrlContentString("user=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'userId'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mUser ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(locationId == -2) {
		pos=getPosUrlContentString("location=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'locationId'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mLocation ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(companyId == -2) {
		pos=getPosUrlContentString("company=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'companyId'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mCompany ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(enrollmentId == -2) {
		pos=getPosUrlContentString("enrollmentid=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'enrollmentId'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mEnrollment ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(cRem == -2) {
		pos=getPosFromBooleanKey("CheckOutWhenRemoved", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'CheckOutWhenRemoved'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mCheckOutWhenRemoved: %s\n\033[m", input);
			free(input);
		}
	}
	if(accR == -2) {
		pos=getPosFromKey("AccessRights", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'AccessRights'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mAccessRights: %s\n\033[m", input);
			free(input);
		}
	}
	if(dName == -2) {
		pos=getPosFromKey("PayloadDisplayName", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'PayloadDisplayName'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mPayloadDisplayName: %s\n\033[m", input);
			free(input);
		}
	}
	if(pName == -2) {
		pos=getPosFromKey("PayloadDisplayName", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadDisplayName'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mPayloadDisplayName: %s\n\033[m", input);
			free(input);
		}
	} 
	if(pDesc == -2) {
		pos=getPosFromKey("PayloadDescription", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadDescription'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mPayloadDescription: %s\n\033[m", input);
			free(input);
		}
	}
	if(PayPass == -2) {
		pos=getPosFromKey("Password", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'Password'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mPassword: %s\n\033[m", input);
			free(input);
		}
	}
	if(PayType == -2) {
		pos=getPosFromKey("PayloadType", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadType'\n");
		}
		else {
			fseek(inputFile, pos, SEEK_SET);
			input = malloc(len+1);
			fread(input, 1, len, inputFile);
			input[len]='\0';
			printf("\033[1;33mPayloadType: %s\n\033[m", input);
			free(input);
		}
	}
	
}


void writer() {
	printf("\033[1;33mWriting to %s\n\033[m", fileName);

	int pos, len;
	char *input;

	if(userId != -1) {
		pos=getPosUrlContentString("user=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'userId'\n");
		}
		else {
			input = malloc(strlen(userId)+1);
			sprintf(input, "%s", userId);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet User ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(locationId != -1) {
		pos=getPosUrlContentString("location=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'locationId'\n");
		}
		else {
			input = malloc(strlen(locationId)+1);
			sprintf(input, "%s", locationId);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet Location ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(companyId != -1) {
		pos=getPosUrlContentString("company=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'companyId'\n");
		}
		else {
			input = malloc(strlen(companyId)+1);
			sprintf(input, "%s", companyId);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet Company ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(enrollmentId != -1) {
		pos=getPosUrlContentString("enrollmentid=", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'enrollmentId'\n");
		}
		else {
			input = malloc(strlen(enrollmentId)+1);
			sprintf(input, "%s", enrollmentId);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet Enrollment ID: %s\n\033[m", input);
			free(input);
		}
	}
	if(cRem != -1) {
		pos=getPosFromBooleanKey("CheckOutWhenRemoved", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'CheckOutWhenRemoved'\n");
		}
		else {
			input = malloc(strlen(cRem)+1);
			sprintf(input, "%s", cRem);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet CheckOutWhenRemoved: %s\n\033[m", input);
			free(input);
		}
	}
	if(accR != -1) {
		pos=getPosFromKey("AccessRights", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'AccessRights'\n");
		}
		else {
			input = malloc(strlen(accR)+1);
			sprintf(input, "%s", accR);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet AccessRights: %s\n\033[m", input);
			free(input);
		}
	}
	if(dName != -1) {
		pos=getPosFromKey("PayloadDisplayName", &len, inputFile, 1);
		if(pos<0) {
			printf("Cannot read 'PayloadDisplayName'\n");
		}
		else {
			input = malloc(strlen(dName)+1);
			sprintf(input, "%s", dName);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet PayloadDisplayName: %s\n\033[m", input);
			free(input);
		}
	}
	if(pName != -1) {
		pos=getPosFromKey("PayloadDisplayName", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadDisplayName'\n");
		}
		else {
			input = malloc(strlen(pName)+1);
			sprintf(input, "%s", pName);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet PayloadDisplayName: %s\n\033[m", input);
			free(input);
		}
	} 
	if(pDesc != -1) {
		pos=getPosFromKey("PayloadDescription", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadDescription'\n");
		}
		else {
			input = malloc(strlen(pDesc)+1);
			sprintf(input, "%s", pDesc);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet PayloadDescription: %s\n\033[m", input);
			free(input);
		}
	}
	if(PayPass != -1) {
		pos=getPosFromKey("Password", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'Password'\n");
		}
		else {
			input = malloc(strlen(PayPass)+1);
			sprintf(input, "%s", PayPass);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet Password: %s\n\033[m", input);
			free(input);
		}
	}
	if(PayType != -1) {
		pos=getPosFromKey("PayloadType", &len, inputFile, -1);
		if(pos<0) {
			printf("Cannot read 'PayloadType'\n");
		}
		else {
			input = malloc(strlen(PayType)+1);
			sprintf(input, "%s", PayType);
			cutFromFile(pos, pos+len, inputFile);
			insertInFile(pos, input, inputFile);
			printf("\033[1;33mSet PayloadType: %s\n\033[m", input);
			free(input);
		}
	}
	
}

void processArgument(char **argv) {
	int d=1;
	if(argc>i+1 || CreatMod==2) {
		if(strcmp(argv[i], "-C") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '-C' after '-M' or '-G'\n\033[m");
			}
			CreatMod=0;
			char *str=malloc(strlen(argv[i+1])+13);
			if(strstr(argv[i+1], ".mobileconfig")!=NULL)
				sprintf(str, "%s", argv[i+1]);
			else
				sprintf(str, "%s.mobileconfig", argv[i+1]);
			fileName = malloc(strlen(str));
			strcpy(fileName, str);
			inputFile = fopen(str, "r+");
			free(str);
			if(inputFile==NULL)
				erroredArg("Could not create File");
			i++;
				
		}
		else if(strcmp(argv[i], "-M") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '-M' after '-C' or '-G'\n\033[m");
			}
			CreatMod=1;
			fileName = malloc(strlen(argv[i+1]));
			strcpy(fileName, argv[i+1]);
			inputFile = fopen(argv[i+1], "r+");
			if(inputFile==NULL)
				erroredArg("Could not open File");
			i++;
		}
		else if(strcmp(argv[i], "-G") == 0) {
			if(CreatMod!=-1) {
				erroredArg("\033[1;31mInvalid option '-G' after '-M' or '-C'\n\033[m");
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
				userId = (argv[i+1]);
				i++;
			}
			else {
				userId = -2;
			}
		}
		else if(strcmp(argv[i], "-l") == 0) {
			if(CreatMod!=2) {
				locationId = (argv[i+1]);
				i++;
			}
			else {
				locationId = -2;
			}
		}
		else if(strcmp(argv[i], "-c") == 0) {
			if(CreatMod!=2) {
				companyId = (argv[i+1]);
				i++;
			}
			else {
				companyId = -2;
			}
		}
		else if(strcmp(argv[i], "-e") == 0) {
			if(CreatMod!=2) {
				enrollmentId = (argv[i+1]);
				i++;
			}
			else {
				enrollmentId = -2;
			}
		}
		else if(strcmp(argv[i], "-accR") == 0) {
			if(CreatMod!=2) {
				accR = (argv[i+1]);
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
			PayType = -2;
		}
		else if(strcmp(argv[i], "-all") == 0) {
			if(CreatMod!=2) {
				erroredArg("Cannot use -all without first specifying -G");
			}
			userId = -2;
			locationId = -2;
			companyId = -2;
			enrollmentId = -2;
			cRem = -2;
			accR = -2;
			dName = -2;
			pName = -2;
			pDesc = -2;
			PayPass = -2;
			PayType = -2;
		}
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf(HELPTEXT);
			exit(0);
		}
		else if(strcmp(argv[i], "--version") == 0) {
			printf(Version);
			exit(0);
		}
		else {
			printf("Unknown argument '%s'\nMore info with: 'jamfedit -h'\n", argv[i]);
			exit(2);
		}
	}
}



void erroredArg(char *str) {
	printf("\033[1;31m%s\n\033[m", str);
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

int findInFile(char *str, FILE *fp, int dir) {
	fseek(fp, -strlen(str), SEEK_END);
	int pos=ftell(fp);
	int len = strlen(str);
	char buf[len];
	int val=-1;

	while(pos==ftell(fp)) {
		int rl=fread(buf, 1, len, fp);
		pos+=rl;
		buf[rl]='\0';
		if(strcmp(buf, str) == 0) {
			if(dir<0)
				return ftell(fp);
			else
				val=ftell(fp);
		}
		fseek(fp, - len - 1, SEEK_CUR);
		pos = pos - len - 1;
	}
	return val;
}

int inList(char chr, char list[]) {

	for(int i=0; i<strlen(list); i++) {
		if(list[i]==chr) return 1;
	}
	return 0;
}


int getPosUrlContentString(char *str, int* len, FILE *fp, int dir) {
	*len=0;
	int pos=findInFile(str, fp, dir);
	if(pos<0) return -1;

	fseek(fp, pos, SEEK_SET);

	char list[] = "<&;", buf;
	while(fread(&buf, 1, 1, fp)>0 && !inList(buf, list)) {
		*len+=1;
	}
	return pos;
}

int getPosFromKey(char *str, int* len, FILE *fp, int dir) {
	*len=0;
	char keyBuilder[12+strlen(str)];
	strcpy(keyBuilder, "<key>");
	strcat(keyBuilder, str);
	strcat(keyBuilder, "</key>");

	int pos = findInFile(keyBuilder, fp, dir);
	fseek(fp, pos, SEEK_SET);

	char buf;
	int rl;
	while((rl=fread(&buf, 1, 1, fp))>0&&buf!='>') pos++;
	if(rl<=0) return -1;
	pos++;
	while((rl=fread(&buf, 1, 1, fp))>0&&buf!='<') *len+=1;
	if(rl<=0) return -1;

	return pos;
}

int getPosFromBooleanKey(char *str, int* len, FILE *fp, int dir) {
	*len=0;
	char keyBuilder[12+strlen(str)];
	strcpy(keyBuilder, "<key>");
	strcat(keyBuilder, str);
	strcat(keyBuilder, "</key>");

	int pos = findInFile(keyBuilder, fp, dir);
	fseek(fp, pos, SEEK_SET);

	char buf;
	int rl;
	while((rl=fread(&buf, 1, 1, fp))>0&&buf!='<') pos++;
	if(rl<=0) return -1;
	pos++;
	while((rl=fread(&buf, 1, 1, fp))>0&&buf!='/') *len+=1;
	if(rl<=0) return -1;

	return pos;
}