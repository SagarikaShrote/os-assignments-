#include<iostream>
#include<string>
#include<stdlib.h> 
#include <boost/algorithm/string.hpp> 
#include <bits/stdc++.h> 
using namespace std; 
typedef struct node_type		//array
{
	int size;
	int next;
	int status;				//0=middle,1=free,2=allocated
}node;
typedef struct alloc_type		//alloc list
{
	string str;
	int index;
	struct alloc_type* next;
}alloc;

alloc* malloc1(node* a,int &head_p,int d,string s,alloc* bptr)
{
	alloc*temp_alloc=NULL;
	temp_alloc=(alloc*)malloc(sizeof(alloc));
	alloc* b=bptr;
	int check=0,count=0,temp1,temp2,head=head_p,temp_prev;
	int temp=head;
	while(b!=NULL)			//check if name already exist
	{
		if(b->str==s)
		{
			check=1;
		}
		b=b->next;
	}
	if(check==0)
	{
		if(head!=-1)
		{
			while((a[temp].next!=head)&&(a[temp].size<(d+3)))		//finding a block of size>required
			{
				//cout << "here\n";
				temp=a[temp].next;
				count++;
			}
			if(((temp+d+3)==100))
			{
				temp=a[temp].next;
				while((a[temp].next!=head)&&(a[temp].size<(d+3)))
				{
					//cout << "here\n";
					temp=a[temp].next;
					count++;
				}
			}
			if(a[temp].size>=(d+3)&&((temp+d+3)!=100))     
			{
				if(a[temp].size>(d+3))
				{
					temp2=temp+d+3;
					if(a[temp].next!=temp)
					{
						temp1=a[temp].next;
					}
					else
					{
						temp1=temp2;
					}
					
					temp_prev=temp;
					while(a[temp_prev].next!=temp)
					{
						temp_prev=a[temp_prev].next;
					}
					if(temp_prev==temp)
					{
						temp_prev=temp2;
					}
					a[temp_prev].next=temp2;
					a[temp2].status=1;
					a[temp2].size=(a[temp].size)-d-3;
					a[temp2].next=temp1;
					
					
					//cout << "hello" << endl;
					
				}
				else if(a[temp].size==(d+3))
				{
					if(a[temp].next!=temp)
					{
						temp2=a[temp].next;
						d=d+3;
					}
					else
					{
						temp2=-1;
					}
					temp_prev=temp;
					while(a[temp_prev].next!=temp)
					{
						temp_prev=a[temp_prev].next;
					}
					if(temp_prev==temp)
					{
						temp_prev=temp2;
					}
					a[temp_prev].next=a[temp].next;
				}
				a[temp].size=d;
				a[temp].status=2;
				a[temp].next=-1;
				temp_alloc->str=s;
				temp_alloc->index=temp;
				temp_alloc->next=bptr;
				head_p=temp2;
			}
			else
			{
				cout << "not enough space" << endl;
			}
		}
		else
		{
			cout << "sorry not enough space available" << endl;
		}
	}
	else
	{
		cout << "variable name already exists" << endl;
	}
	return temp_alloc;
}

alloc* free1(node* a,string s,int head,alloc *bptr)
{
	alloc *b=bptr,*temp_b=NULL,*del_b=NULL;
	int flag=0,del_index,correct=0;
	int temp_head;
	if((b!=NULL)&&(flag==0))
	{
		if(b->str==s)
		{
			flag=1;
			temp_b=b;
			del_index=b->index;
			correct=1;
		}
		else
		{
			while(b->next!=NULL)
			{
				if(b->next->str==s)
				{
					flag=2;
					temp_b=b;
					del_index=b->next->index;
					correct=1;
				}
				b=b->next;
			}	
		}
		
		//cout << "hello\n";
		
		if(flag>0)
		{
			a[del_index].status=1;
			//a[del_index].next=head;
			if(head!=-1)
			{
				a[del_index].next=head;
				temp_head=head;
				while(a[temp_head].next!=head)
				{
					temp_head=a[temp_head].next;
				}
				a[temp_head].next=del_index;
			}
			else
			{
				a[del_index].next=del_index;
			}
		}
		if(flag==2)
		{
			del_b=temp_b->next;
			temp_b->next=del_b->next;
		}
		if(flag==1)
		{
			bptr=temp_b->next;
		}
	}
	if(correct==0)
	{
		cout << "Wrong variable name \n";
	}
	return bptr;
}

void display(node* a,alloc* b)
{
	for(int i=0;i<100;i++)
	{
		if(a[i].status==1)
		{
			cout << "\tfree size = " << a[i].size << "\tnext= " << a[i].next << "\tmy index is= " <<i << endl;
		}
	}
	while(b!=NULL)
	{
		cout << "\talloc size = " << a[b->index].size << "\tvariable= " << b->str << endl;
		b=b->next;
	}
	//cout << "inside display" << endl;
}

//coalesce memory
int cm(node* a,int head)
{
	int temp,flag,temp_prev=-1,flag1=0;
	for(int i=0;i<100;i++)
	{
		flag=1;
		while(flag==1)
		{
			flag=0;
			//cout << "hello" << i << endl;
			if(a[i].status==1)
			{
				if(temp_prev!=-1)
				{
					a[temp_prev].next=i;
				}
				else
				{
					head=i;
				}
				temp_prev=i;
				a[i].next=head;
				temp=a[i].size;
				if((i+temp+3)<=99)
				{
					if(a[i+temp+3].status==1)
					{
						flag=1;
						a[i].size+=3+a[i+temp+3].size;
						a[i+temp+3].status=0;
						a[i+temp+3].size=0;
						a[i+temp+3].next=-1;
					}
				}
			}
		}
	}
	//cout << "head=" << head;
	cout << "space coalesced" << endl;
	return head;
}

int main()
{
	node a[100];
	alloc* b=NULL;
	int i,size,flag;
	string temp_s="";
	for(i=0;i<100;i++)
	{
		a[i].size=0;
		a[i].next=-1;
		a[i].status=0;
	}
	a[3].size=97;
	a[3].next=3;
	a[3].status=1;
	
	/*a[3].size=10;
	a[3].next=13;
	a[3].status=1;
	a[13].size=84;
	a[13].next=3;
	a[13].status=1;*/
	
	
	
	string str;
	int head=3;
	//int alloc_head=-1;
	do { 
		flag=0;
		cout << "virtualbox @ sagarika -> shell $";
		getline (cin, str); 
		vector<string> word; 
    	boost::split(word, str, boost::is_any_of(" "));
    	if(word.size()>2)
    	{ 
    		if(word[2]=="malloc")
    		{
    			flag=1;
    			size=word[3].size();
    			char c[size+1];
    			strcpy(c, word[3].c_str());
    			temp_s="";
    			for (int i = 1; i < size-1; i++) 
        		{
        			temp_s=temp_s+c[i];
        		}
    			int d=stoi(temp_s);
    			b=malloc1(a,head,d,word[0],b);
    		}
    	}
    	if(word[0]=="free")
    	{
    		flag=1;
    		size=word[1].size();
    		char c[size+1];
    		strcpy(c, word[1].c_str());
    		temp_s="";
   			for (int i = 1; i < size-1; i++) 
        	{
        		temp_s=temp_s+c[i];
       		}
   			b=free1(a,temp_s,head,b);
   		}
    	if(word[0]=="ds")
    	{
    		flag=1;
    		display(a,b);
    	}
    	if(word[0]=="cm")
    	{
    		flag=1;
    		head=cm(a,head);
    		/*if(b!=NULL)
    		{
    			cout << "hello \n";
    		}*/
    	}
    	if((flag==0)&&(str!="exit"))
    	{
    		cout << "unrecognised command" << endl;
    	}
    	if(str=="exit"){
    		cout<<"bye"<<endl;
    	}
	} while(str!="exit");
	return 0;
}
