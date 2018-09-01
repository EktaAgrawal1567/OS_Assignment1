#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<termios.h>
#include<dirent.h>
#include <bits/stdc++.h>
#include <vector>
#include<sys/ioctl.h>
#include <iostream>
#include<time.h>
#include <unistd.h>
#include <linux/kd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

using namespace std;
int init_terminal();
static struct termios initial_settings, new_settings;
static int peek_character = -1;
int c=0,cnt;
void init_keyboard();
void close_keyboard();
char cwd[PATH_MAX];
char path[PATH_MAX];
char paths[PATH_MAX];
vector<string> v;
vector<string> v2;
stack<string> bs,fs;
char current[PATH_MAX];

char *ch=(char*)malloc(PATH_MAX*sizeof(char));

void reset()
{
  cout<<"\033c";
}
int ls_func(char cwd[])
{
 //cout<<"After ls"<<cwd<<endl;
  int y;
  
 v.clear();
// cout<<v.size();
    //cout<<"A";
    struct stat attr;
    struct passwd* own;
    struct group*gp;
    stat(cwd,&attr);
     y=S_ISREG(attr.st_mode);
    // cout<<y<<" ";
      char *pointer=NULL;
      struct dirent **dp;
       c=scandir(cwd,&dp,NULL,alphasort);
      const char* units[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
      long int size;
      FILE *f;
      //pointer = getenv("PWD");
     // dp=opendir((const char*)cwd);
      if(y==0){
      for(int i=0;i<c;i++)
      {
          //file permisssions
          //cout<<cwd<<endl;
          stat(dp[i]->d_name,&attr);
          //printf(((attr.st_mode & S_IFMT)==S_IFDIR) ? "d" : "-"); 
          printf( (S_ISDIR(attr.st_mode)) ? "d" : "-");
          printf( (attr.st_mode & S_IRUSR) ? "r" : "-");
          printf( (attr.st_mode & S_IWUSR) ? "w" : "-");
          printf( (attr.st_mode & S_IXUSR) ? "x" : "-");
          printf( (attr.st_mode & S_IRGRP) ? "r" : "-");
          printf( (attr.st_mode & S_IWGRP) ? "w" : "-");
          printf( (attr.st_mode & S_IXGRP) ? "x" : "-");
          printf( (attr.st_mode & S_IROTH) ? "r" : "-");
          printf( (attr.st_mode & S_IWOTH) ? "w" : "-");
          printf( (attr.st_mode & S_IXOTH) ? "x" : "-");
          printf("\t");
          
                 //size of the file
          own=getpwuid(attr.st_uid);
         printf("%s\t",own->pw_name);
           gp=getgrgid(attr.st_gid);
        printf("%s\t",gp->gr_name);
           printf("%ld\t",attr.st_size);
                        
           //last modified time of file
                 
           struct tm *t;
           const char * months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July","Aug", "Sep", "Oct", "Nov", "Dec"};
                 
            t=gmtime(&(attr.st_mtime));
            int y=t->tm_mon;
            printf("%s %d %d:%d \t",months[y],t->tm_mday,t->tm_hour,t->tm_min);
         
              

            //name of the file
            printf("%10.20s  \t",dp[i]->d_name);
            v.push_back(dp[i]->d_name);
            //v2.push_back(dp[i]->d_name);
            printf("\n");

            
         //   cout<<paths<<endl;
        }     
        }
        if(y==1)
        {
        pid_t pid=fork();
        if(pid==0)
        {
          execl("/usr/bin/xdg-open","xdg-open",cwd,(char*)0);
          exit(0);
        }

              }
               cnt=c;
}
int main()
{
	
	//cout<<"a"
       
         int x;   
          //cout<<"a";
   if (getcwd(cwd, sizeof(cwd)) != NULL)
         {  
          strcpy(current,cwd);    
          strcpy(path,cwd);  
         }
    //cout<<"Before ls"<<current<<endl;
//cout<<"a";
    
     ls_func(current);
     bs.push(current);
       //bs.push(current);
       //cout<<c;   
        init_keyboard();
        // int count;
         char g;
         int k;
         while(1)
         {
          g=getchar();
          if(g=='A')
          {
            printf("\033[A");
            if(cnt>1)
            	cnt--;
          }
          if(g=='B')
          {
            printf("\033[B");
            if(cnt<c)
            	cnt++;
          }
          if(g=='C')
          {
          //  printf("\033[C");
            reset();
            if(!fs.empty()){
            bs.push(fs.top());
            fs.pop();
            strcpy(ch,fs.top().c_str());
            ls_func(ch);
          }
            
          }
          if(g=='D')
          {
          //  printf("\033[D");
             int status;
            reset();
            if(current==path)
            {
               status=0;
            }
            if(!bs.empty()){
            fs.push(bs.top());
            bs.pop();
            strcpy(ch,fs.top().c_str());
            ls_func(ch);
           }
          }
          if(g==10)
          { 
           //cout<<current;

            strcat(current,"/");
            strcat(current,v[cnt].c_str());
           // cout<<cnt<<" "<<v[cnt];

            reset();
            bs.push(current);
            ls_func(current); 

          }
          if(g==':')
          {
            
          }
         if(g==127)
         {
            if(!bs.empty())
           {  
            fs.push(bs.top());
             bs.pop();
            strcpy(ch,fs.top().c_str());
            ls_func(ch);
         }
         }
         if(g=='h')
         {
          ls_func(path);
         }


        }


         close_keyboard();
 }
        void init_keyboard()
        {
          tcgetattr(0,&initial_settings);
          new_settings = initial_settings;
          new_settings.c_lflag &= ~ICANON;
          new_settings.c_lflag &= ~ECHO;
          new_settings.c_lflag &= ~ISIG;
          new_settings.c_cc[VMIN] = 1;
          new_settings.c_cc[VTIME] = 0;
          tcsetattr(0, TCSANOW, &new_settings);
        }
        void close_keyboard()
        {
          tcsetattr(0, TCSANOW, &initial_settings);
        }
