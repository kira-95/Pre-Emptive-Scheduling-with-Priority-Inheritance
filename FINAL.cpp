#include<iostream>
using namespace std;

int RES=5;
int TIME=0;

class proc
{
  public:
	int no;  		//  process no
	int pri[20];  	// priority
	
	int time;  		// running time
	int status; 	// 0 is waiting, 1 is running, 2 is done
	int end; 		//  end time

	int start;  	// starting time
	int dur;  		// total time required
	int rec[10]; 		// 1 is A, 2 is B, 3 is both
	
	int crit_start[10];	//>=1 since some time is needed for request
	int crit_dur[10];
	
	int TOS;	//TOS indicates the stack of priorities
	int k;		//Number of critical section
	int count;	//Current critical section
	
	void init(int n)
	{
		time=0;
		status=0;
		end=0;
		TOS=0;
		count=1;
		
		int i,j;
		no=n;
		cout<<"\nPriority of process "<<n<<"\t";
		cin>>pri[++TOS];
		cout<<"\nStarting time of process "<<n<<"\t";
		cin>>start;
		cout<<"\nDuration of process "<<n<<"\t";
		cin>>dur;
		cout<<"\nEnter the number of critical sections \t";
		cin>>k; 
		for(j=1;j<=k;j++)
		{
			cout<<"\nTime after which critical section starts from Starting Time "<<n<<"\t";
			cin>>crit_start[j];
			cout<<"\nDuration of critical section "<<n<<"\t";
			cin>>crit_dur[j];
			cout<<"\nResources required for process "<<n<<"\t";
			cin>>rec[j];
		}
		crit_start[k+1]=dur+2;
	}
	
	void display(void)
	{
		cout<<"\nNo : "<<no<<"\nPri : "<<pri[TOS]<<"\nStart time : "<<start<<"\nDuration : "<<dur<<"\nEnd time : "<<end<<"\nResource : "<<rec<<endl;
	}
};

proc process[20];

int main()
{
	int R[RES+1], n, i, t=0, j;					// R - Locks for resources ; i,j - Loop increment variables ; n -number of processes
	int high=0;									
	int curr=0;									// curr - Current process
	int flag=0;									// Flag to check if  resource is occupied
	int m;										// For new process flag
	int temp;									// to hold temporary process while comparison 		
	int max;									// used to store max priority
	int cs;										// context switch flag
	
	
	for(j=1;j<=RES;j++)							// initialization
		R[j]=0;
	
	int cnt;
	
	cout<<"\nEnter the number of processes\t";
	cin>>n;
	cout<<"\nEnter the details\n";

	for(i=1;i<=n;i++)							// accepting process details
		process[i].init(i);  
		
	curr=1;	
	do
	{
		process[curr].status=1;					// remainder section
		
		if(process[curr].time>=process[curr].crit_start[process[curr].count] && process[curr].time<=(process[curr].crit_start[process[curr].count] + process[curr].crit_dur[process[curr].count]))
		{
			process[curr].status=3;				// critical section
		}
		
		m=0;
		flag=0;
		cnt=0;
		
		if(process[curr].status==3)
		{
			if(R[process[curr].rec[process[curr].count]]==0)
			{
				flag=1;							// Setting intial value to R array
			}
			
			if(flag==1)
			{
				R[process[curr].rec[process[curr].count]]=curr;
			}
		}
		
		
		while(process[curr].time!=process[curr].dur)		// process curr is running
		{
			cs=0;											// no context switch
			
			if(process[curr].time>=process[curr].crit_start[process[curr].count] && process[curr].time<=(process[curr].crit_start[process[curr].count] + process[curr].crit_dur[process[curr].count]))
			{												// critical section
				
				process[curr].status=3;
				flag=0;															// Setting intial value to R array
				if(R[process[curr].rec[process[curr].count]]==0 || R[process[curr].rec[process[curr].count]]==curr )		// Checking if resource is free
				flag=1;															// Flag 1 if free
				if(flag==1)
				{
					R[process[curr].rec[process[curr].count]]=curr;
				}
				else
				{	
					cs=1;
					process[curr].status=0;					// waiting	
			
					for(j=1;j<=RES;j++)
					{
						if(process[curr].rec[process[curr].count]==j)
						{	
							temp=curr;						// process priority is increased to just above the calling HP process
							curr=R[j];						
							process[curr].status=3;
							process[curr].pri[++(process[curr].TOS)]=process[temp].pri[process[temp].TOS]+1;
						}
					}
				}
			}
			
			if(cs==1)
			{
				continue;			//Context Switch for resource already occupied
			}
			
			if(process[curr].time==(process[curr].crit_start[process[curr].count] + process[curr].crit_dur[process[curr].count]))		//End of critical section
			{
				cnt=1;										// count=1 => indicates end of critical section at TIME
				
				R[process[curr].rec[process[curr].count]]=0; 					// free resources
				(process[curr].count)++;
				process[curr].status=1;
				if(process[curr].TOS!=1)
				{
					temp=0;
					max=0;
			
					for(i=1;i<=n;i++)						// End of critical section
					{
						if(process[i].status!=2 && process[i].status!=1 )
						{
							if(process[i].pri[process[i].TOS]<=process[curr].pri[process[curr].TOS] && process[i].pri[process[i].TOS]>max && process[i].start<=TIME)	//equal to
							{
								max=process[i].pri[process[i].TOS];
								temp=i;
							}
						}
					}
					process[curr].TOS=1;
					process[curr].status=0;
					curr=temp;
					goto label;
				}
			}
				
			for(i=1;i<=n;i++)
			{
				
				if(process[i].status==0 && process[i].start==TIME)		// interupts
				{
					if(process[i].pri[process[i].TOS]<process[curr].pri[process[curr].TOS])
					continue;
					else
					{
						m=1;											// high priority interupt
						break;
					}
				}
			}
			
			if(m==1)
				break;
			
			TIME++;														// TIME incremented by 1 unit
			process[curr].time++;
			cout<<"\nTIME : "<<TIME<<"\trunning : "<<curr<<"\tstatus : "<<process[curr].status<<endl;
						
		}
		
		label:
			if(cnt==1 && m==0)
				continue;				// end of critical section; no interrupts
		
		if(m==1)
		{								// higher priority interrupts waiting
			
			process[curr].status=0;		// curr process is waiting
			flag=0;
			
			if(process[i].time>=process[i].crit_start[process[i].count] && process[i].time<=(process[i].crit_start[process[i].count] + process[i].crit_dur[process[i].count]))
			{
				process[i].status=3;											//	HP Interupt in critical section
				
				if(R[process[i].rec[process[i].count]]==0 || R[process[i].rec[process[i].count]]==curr )			// Checking if resource is free
				{
					flag=1;
				}
																			
				if(flag==1)
				{	
					curr=i;
					R[process[curr].rec[process[curr].count]]=curr;			
				}
				else
				{	
					process[i].status=0;	
			
					for(j=1;j<=RES;j++)
					{
						if(process[curr].rec[process[curr].count]==j)
						{	
							temp=curr;
							curr=R[j];
							process[curr].status=3;
							process[curr].pri[++(process[curr].TOS)]=process[temp].pri[process[temp].TOS]+1;
						}
					}
				}
			}
			else
			{
				curr=i;									// HP interupt not in critical section
			}
		}
		if(m==0)
		{
			
			process[curr].status=2;					//lower priority interupts and/or next process
			process[curr].end=TIME; 				
		
			R[process[curr].rec[process[curr].count]]=0;					//free resources
			
			for(i=1;i<=n;i++)
			{	
													//End-condition
				if(process[i].status==2)
					continue;
				else
					break;
			}
			
			if(i==n+1)
			break;
						
			temp=0;
			max=0;
			
			for(i=1;i<=n;i++)						//low priority interupts
			{
				if(process[i].status!=2 && process[i].status!=1)
				{
					if(process[i].pri[process[i].TOS]<=process[curr].pri[process[curr].TOS] && process[i].pri[process[i].TOS]>max && process[i].start<=TIME)	//equal to
					{
						max=process[i].pri[process[i].TOS];
						temp=i;
					}
				}
			}
			
			
			curr=temp;
			
			int val=0;
			
			if(temp==0)								// idle CPU : no processes to run
			{
				while(1)
				{
					for(i=1;i<=n;i++)
					{
						if(process[i].status==0&&process[i].start==TIME)
						{
							val=1;					// check for a process to start
							break;
						}
					}
					if(val==1)
					break;
					TIME++;
					cout<<"\nTIME : "<<TIME<<"\trunning : "<<curr<<"\tstatus : "<<process[curr].status<<endl;
			
				}
				curr=i;			
			}
		}
	}while(1);
	
	for(i=1;i<=n;i++)
	{
		cout<<"\nProcess : "<<i<<"\t End time : "<<process[i].end<<endl;	// output
	}
}

