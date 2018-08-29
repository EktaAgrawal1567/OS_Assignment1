
#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<termios.h>
#include<dirent.h>
#include<sys/ioctl.h>
#include<time.h>
#include <unistd.h>
#include <linux/kd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int init_terminal();
static struct termios initial_settings, new_settings;
static int peek_character = -1;
int c=1;
void init_keyboard();
void close_keyboard();
char cwd[PATH_MAX];
       char path[PATH_MAX];
       char paths[PATH_MAX];
struct Node{
  char name[1000];
  Node *next;
  Node *prev;
};
struct Node *head=NULL;
void insert(struct Node** head,char *name1)
{
        struct Node* new_n=(struct Node*)malloc(sizeof(struct Node));
        struct Node* last=*head;
        char *name;
          name=(char*)malloc(1000*sizeof(char));
        strcpy(new_n->name,name1);
        new_n->next=NULL;
        if(*head==NULL)
        {
             new_n->prev=NULL;
             *head=new_n;
             return;
        }
        while(last->next!=NULL)
        {
            last=last->next;
        }
        last->next=new_n;
        new_n->prev=last;
}
void printl(struct Node* head)
{
        
        struct Node* ptr=head;
        while(ptr->next!=NULL)
        {
          printf("%s\n",ptr->name);
          ptr=ptr->next;
        }
}

int ls(char *cwd)
{
 
  
    printf( "\033[2J" );
    printf("cwd%s\n", cwd);
    struct passwd* own;
      struct group* gp;
      char *pointer=NULL;
      DIR *dp=NULL;
      const char* units[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
      float size;
      FILE *f;
      struct dirent *sd=NULL;
      //pointer = getenv("PWD");
      dp=opendir((const char*)cwd);
       
      while((sd=readdir(dp))!=NULL)
      {
        printf("inside while");
          //file permisssions
          struct stat attr;
          stat(sd->d_name,&attr);
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
          c++;
              //size of the file
          own=getpwuid(attr.st_uid);
           //printf("%s\t",own->pw_name);
           gp=getgrgid(attr.st_gid);
           //printf("%s\t",gp->gr_name);
           printf("%ld\t",attr.st_size);
                        
           //last modified time of file
                 
           struct tm *t;
           const char * months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July","Aug", "Sep", "Oct", "Nov", "Dec"};
                 
            t=gmtime(&(attr.st_mtime));
            int y=t->tm_mon;
            printf("%s %d %d:%d \t",months[y],t->tm_mday,t->tm_hour,t->tm_min);
         
              

            //name of the file
            printf("%10.20s  \t",sd->d_name);
            printf("\n");

            strcpy(paths,path);
            strcat(paths,"/");
            strcat(paths,sd->d_name);
             insert(&head,paths);

        }

}
int main()
{
     
  if (getcwd(cwd, sizeof(cwd)) != NULL)
         {   strcpy(path,cwd);    
         }

       ls(cwd);
        init_keyboard();
         char g;
         int k;
         int count=c;
         while(1)
         {
          g=getchar();
          if(g=='A')
          {
            printf("\033[A");
            if(count>1) count--;
          }
          if(g=='B')
          {
            printf("\033[B");
            if(count<c)count++;
          }
          if(g=='C')
          {
            printf("\033[C");
          }
          if(g=='D')
          {
            printf("\033[D");
          }
          if(g==10)
          { 
            printf("Count: %d",count);
             //printf( "\033[2J" );
             struct Node *ptr=head;
             printf("abcd\n");
             int c1=1;
             while(c1!=count)
             {
              printf("loop %d\n",c1);
              ptr=ptr->next;
              c1++;

             }
             printf("before ls\n");
             ls(ptr->name);
            // printl(head);
           }
          if(g==':')
            break;
         
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


  

   
   

