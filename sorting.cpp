#include<iostream>
#include<vector>
#include<omp.h>
using namespace std;

void bubble(int a[],int n){
	int i,j;
	bool swapped=true;
	while(swapped){
		swapped=false;
		bool lswapped=false;
		#pragma omp parallel for shared(a,n) private(i,j) reduction(||:lswapped)
		for(int i=0;i<n-1;i++){
			if(a[i]>a[i+1]){
				swap(a[i],a[i+1]);
				lswapped=true;
			}
			//cout<<"when i="<<i<<endl;
			//cout<<omp_get_thread_num()<<" ";
			//cout<<"--"<<endl;
		}
		swapped=lswapped||swapped;
		
	}
}

void merge(int a[],int left,int mid,int right){
	int n1=mid-left+1;
	int n2=right-mid;
	int leftA[n1],rightA[n2];
	for(int i=0;i<n1;i++) leftA[i]=a[left+i];
	for(int i=0;i<n2;i++) rightA[i]=a[mid+1+i];
	
	int i=0,j=0,k=left;
	while(i<n1 && j<n2){
		if(leftA[i]<=rightA[j]){
			a[k]=leftA[i];
			i++;
		}
		else{
			a[k]=rightA[j];
			j++;
		}
		k++;
	}
	while(i<n1){
		a[k]=leftA[i];
		i++;
		k++;
	}
	while(j<n2){
		a[k]=rightA[j];
		j++;
		k++;
	}
}


void mergeSort(int a[],int left,int right){
	if(left>=right){
		return;
	}
	int mid=left+(right-left)/2;
	
	#pragma omp parallel sections
	{
		#pragma omp section
		mergeSort(a,left,mid);
		
		#pragma omp section
		mergeSort(a,mid+1,right);
		
	}
	merge(a,left,mid,right);
	
}

void okPrint(int a[],int n){
	cout<<"\nTHE SORTED ARRAY IS: "<<endl;
	for(int i=0;i<n;i++){
		cout<<a[i]<<" ";
	}
}

int main(){
	int n;
	cout<<"\nEnter number of elements into array:"; cin>>n;
	cout<<"\nEnter array elements:-\n";
	int vec[n],cores;
	for(int i=0;i<n;i++){
		cin>>vec[i];
	}
	
//	for(int i=0;i<n;i++){
//		cout<<vec[i];
//	}

	cout<<"\nEnter no of cores to use:- ";
	cin>>cores;
	while(1){
		cout<<"\n1.BUBBLE SORT";
		cout<<"\n2.MERGE SORT";
		cout<<"\n3.QUIT";
		cout<<"\nEnter choice:";int c;
		cin>>c;
		switch(c){
			case 1: 
			{
				double time=omp_get_wtime();
				bubble(vec,n);
				double sTime=omp_get_wtime()-time;
				cout<<"\nSequential Time taken is: "<<sTime;
				
				time=omp_get_wtime();
				bubble(vec,n);
				double pTime=omp_get_wtime()-time;
				cout<<"\nParallel Time taken is: "<<pTime;
				
				okPrint(vec,n);
				break;
				
			
			}
			case 2: 
			{
				
				double time=omp_get_wtime();
				mergeSort(vec,0,n-1);
				double sTime=omp_get_wtime()-time;
				cout<<"\nSequential time taken:- "<<sTime;
				
				time=omp_get_wtime();
				mergeSort(vec,0,n-1);
				double pTime=omp_get_wtime()-time;
				cout<<"\nParallem time taken:- "<<pTime;
				
				okPrint(vec,n);
				break;
			}
				
				
			case 3: exit(1);
		}
	}
	


}
