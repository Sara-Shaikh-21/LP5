#include<iostream>
#include<omp.h>
#include<vector>
using namespace std;

#define Array_Size 1000;

void Sequential(vector<double> arr,int n){
	double sum=0.0,avg=0.0,min,max;
	sum=0.0;
	avg=0.0;
	min=arr[0];
	max=arr[0];
	for(int i=0;i<n;i++){
		sum+=arr[i];
		if(arr[i]<min){
			min=arr[i];
		}
		if(arr[i]>max){
			max=arr[i];
		}
	}
	avg=sum/n;
	cout<<"Sequential sum="<<sum<<endl;
	cout<<"Sequential average="<<avg<<endl;
	cout<<"Sequential minimum="<<min<<endl;
	cout<<"Sequential maximum="<<max<<endl;
	
}

void Parallel(vector<double> arr,int n,int cores){
	double sum=0.0,avg=0.0,min,max;
	sum=0.0;
	min=arr[0];
	max=arr[0];	
	#pragma omp parallel num_threads(cores) shared(arr,min,max) 
	{
		double local_sum=0;
		double local_min=arr[0];
		double local_max=arr[0];
		
		#pragma omp for
		for(int i=0;i<n;i++){
			local_sum+=arr[i];
			if(arr[i]<min){
				local_min=arr[i];
			}
			if(arr[i]>max){
				local_max=arr[i];
			}
		}
		
		#pragma omp atomic
		sum+=local_sum;
		
		#pragma omp critical 
		if(local_min<min){
			min=local_min;
		}
		
		#pragma omp critical
		if(local_max>max){
			max=local_max;
		}
	}
	avg=sum/n;
	cout<<endl;
	cout<<"Parallel Sum="<<sum<<endl;
	cout<<"Parallel average="<<avg<<endl;
	cout<<"Parallel minimum="<<min<<endl;
	cout<<"Parallel maximum="<<max<<endl;
	

}

int main(){
	int i,cores,n;
	vector<double> arr;

	double ss,sp,es,ep;
	cout<<"\nEnter size of array:- \n";
	cin>>n;
	if(n<=0){
		cout<<"\nEnter valid size!";
		return 1;
	}
	arr.resize(n);
	cout<<"\nEnter array elements"<<endl;
	for(int i=0;i<n;i++){
		cin>>arr[i];
	}
	cout<<"\nEnter no of threads to use: ";
	cin>>cores;
		
	ss=omp_get_wtime();
	Sequential(arr,n);
	es=omp_get_wtime();
	cout<<"Sequential execution time="<<es-ss<<" secs "<<endl;
	
		
	//RESET VARIABLES
	sp=omp_get_wtime();
	Parallel(arr,n,cores);
	ep=omp_get_wtime();
	cout<<"Parallel execution time="<<ep-sp<<" secs "<<endl;
	
	
	return 0;
	
	
	
	

}
