# CSE330: Operating Systems 

Please follow the below steps to test your Project-4. 

- Download the zip of this git repository.
- Unzip the repository.
- Copy your kernel module code (proc_filesys.c) to this repository.
- Run the test.sh script with the test arguments. 

bash test.sh < Test Case Number> 

## Test Cases:

| Test Case #   | Test Command  | Test Criteria  | Total Points |
| ------------- |:-------------:| :-----:|:-----:|
| 0 | NA | For all the test cases, the kernel module should init successfully and exit successfully.| 5 |
| 1 | ./test.sh 1 | The result from your kernel module in the userspace should be ```[CSE330][Fall2022][Project-4][ASUID:<Students ASUID>][<Student Name>]``` | 15 |
| 2 | ./test.sh 2 | The RSS, WSS, SWAP & time elapsed from your kernel module in the userspace is correct. The message from the kernel module should in the following format. ```[PID:<PID>]-[RSS:<RSS>]-[WSS:<WSS>]-[SWAP:<SWAP>]-[ELAPSED TIME:<HH:MM:SS>]``` | 30 |

## Note: 
- Make sure you name your kernel model as "proc_filesys" and name your module input parameter as "uuid"
- Please do not make any changes in provided test code to pass the test cases.
- You can use print statements in case you want to debug and understand the logic of the test code.
- Please get in touch with the TAs if you face issues in using the test scripts.
