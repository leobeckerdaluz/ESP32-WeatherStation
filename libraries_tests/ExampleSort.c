#include <stdio.h>              //including stdio.h for printf and other functions

int main(){
	int a[10] = { 3,4,7,6,5,1,2,8,10,9 };           //Array declaration size-10
	int n = 10;                                     //Temporary number for array size
	



	printf("\n\nArray Data : ");                    //Printing message
	for (int i = 0; i < n; i++){
		printf(" %d ", a[i]);                   //Printing data
	}

	


	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (a[j] > a[i]){
				int tmp = a[i];         //Using temporary variable for storing last value
				a[i] = a[j];            //replacing value
				a[j] = tmp;             //storing last value
			}
		}
	}
	printf("\n\nAscending : ");                     //Printing message
	for (int i = 0; i < n; i++){
		printf(" %d ", a[i]);
	}
	




	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (a[j] < a[i]){
				int tmp = a[i];         //Using temporary variable for storing last value
				a[i] = a[j];            //replacing value
				a[j] = tmp;             //storing last value
			}
		}
	}
	printf("\n\nDescending : ");                    //Printing message
	for (int i = 0; i < n; i++){
		printf(" %d ", a[i]);                   //Printing data
	}





	return 0;
} 	