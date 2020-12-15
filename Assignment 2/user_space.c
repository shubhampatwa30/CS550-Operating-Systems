#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

#define BUF_SIZE 256

//Get the state using the value 
char* getcurrstate(int state){
        switch(state) {
                case 0x0000:
                        return "TASK_RUNNING";
                case 0x0001:
                        return "TASK_INTERRUPTIBLE";
                case 0x0002:
                        return "TASK_UNINTERRUPTIBLE";
                case 0x0004:
                        return "__TASK_STOPPED";
                case 0x0008:
                        return "__TASK_TRACED";
                case 0x0010:
                        return "EXIT_DEAD";
                case 0x0020:
                        return "EXIT_ZOMBIE";
	        case (0x0020 | 0x0010):
		        return "EXIT_TRACE";
                case 0x0040:
                        return "TASK_PARKED";
                case 0x0080:
                        return "TASK_DEAD";
                case 0x0100:
                        return "TASK_WAKEKILL";
                case 0x0200:
		        return "TASK_WAKING";
                case 0x0400:
                        return "TASK_NOLOAD";
                case 0x0800:
                        return "TASK_NEW";
                case 0x1000:
                        return "TASK_STATE_MAX";
                case (0x0100 | 0x0002):
                        return "TASK_KILLABLE";
                case (0x0100 | 0x0004):
                        return "TASK_STOPPED";
                case (0x0100 | 0x0008):
                        return "TASK_TRACED";
                case (0x0002 | 0x0400):
                        return "TASK_IDLE";
                case (0x0001 | 0x0002):
                        return "TASK_NORMAL";
                case (0x0000 | 0x0001 | 0x0002 | 0x0004 | 0x0008 | 0x0010 | 0x0020 | 0x0040):
                        return "TASK_REPORT";
                default:
                        return "INVALID STATE";
        }
}



int main() {
	int fp,status;
	char buf[BUF_SIZE];  // initialise buffer
	//Open the Process_List character device
	fp = open("/dev/process_list",O_RDONLY);
	//Check for errors
	if(fp<0) {
		perror("ERROR in opening device Process_List");
		return 0;
	}
	int i;
	//Display all processes using the return value from kernel space.
	while((status = read(fp,buf,strlen(buf)))  > 0) {
	  //Get length of buffer
	  int len = strlen(buf);
	  //CHeck the last '=' sign
	for(i=len-1;i>0;i--){
	  if(buf[i] == '='){
	    break;
	  }
	}
	int x=0;
	char c[12];
	//Copy the state value in c string and make that values in buf as null for concatenation
	while(i<len-1){
	  i++;
	  c[x] = buf[i];
	  buf[i] = '\0';
	  x++;
	}
	//Get State from the value from c 
	char *state = getcurrstate(atoi(c));
	//Concatenate the state string with the previous result
	char *result  = malloc(strlen(buf) + strlen(state) +1 );
	strcpy(result,buf);
	strcat(result,state);
	//Print the result string
	printf("%s\n",result);
	//Set all values to null again
	memset(c,'\0',sizeof(char)*12);
	memset(result,'\0',sizeof(char)*strlen(result));
	memset(buf,0,sizeof(char)*BUF_SIZE);
	}
	//Check for errors again
	if(status < 0) {
		perror("ERROR in reading from  Process List character device !");
		return 0;
	}
	printf("\n");
	//Close the device
	close(fp);
	return 0;
}
