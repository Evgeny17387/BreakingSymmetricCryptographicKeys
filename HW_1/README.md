### Cipher
![](Cipher.PNG)
### Known Data
PlainText</br>
CipherText</br>
### Attack Implemented
BruteForce</br>
By implementing the cipher we run over all the possible keys upon the given PlainText and check if the CalculatedCipherText equals to the CipherText</br>
The key for success here is to use optimized algorithm for encruption and CUDA</br>
### Directory Structure
Cipher.pdf 		- Cipher specification</br>
Cipher_C 		- Cipher breaking implemented in C, build using make file</br>
Cipher_CPP 		- Cipher breaking implemented in CPP, build using Visual Studio</br>
Cipher_Cuda 	- Cipher breaking implemented in CUDA, build using Visual Studio</br>
Cipher_Python 	- Cipher implemented in Python (no breaking)</br>
### How to Run
Define testcase variable accordingly to desired key length breaking:</br>
0 – 20 bit key</br>
1 – 32 bit key</br>
Run, wait until program stops and gives the Key</br>
### Run Time
20 bit key, should take around 1 second for all implementations</br>
32 bit key, should take around 3 minutes for the C and CPP and 12 seconds for CUDA </br>
