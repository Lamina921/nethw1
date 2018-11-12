#include	"unp.h"
char back[MAXLINE]="HTTP/1.1 200 OK\r\n\
Server: seyo\r\n\
Content-Type: text/html\r\n\
";
char back_jpg[MAXLINE]="HTTP/1.1 200 OK\r\n\
Server: seyo\r\n\
Content-Type: image/jpeg\r\n\
";
char mes[MAXLINE]="";
void responce(int require,int connfd){
FILE *fp;
char				buf[MAXLINE],buf2[MAXLINE],ch;

int r,w;

switch(require)
{
	case 1://web
	write(connfd,back,strlen(back));
	break;

	case 2://picture
	write(connfd,back_jpg,strlen(back_jpg));

	fp=fopen("1.jpg","rb");
	
	while(!feof(fp)){
		r=fread(buf2,sizeof(char),MAXLINE,fp);
		w=write(connfd,buf2,r); 
		}
	fclose(fp);
	break;
	default:
	;
	}
}


int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	int content;
	char				buf[MAXLINE],buf2[MAXLINE]="GET /home/%E6%A1%8C%E9%9D%A2/net/1/1.jpg";;
	FILE 				*fp;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(80);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	signal(SIGCHLD, SIG_IGN);
	
//web
fp=fopen("index.html","r");
	while(fgets(buf,MAXLINE,fp)!=NULL)strcat(mes,buf);
		fclose(fp);
		sprintf(buf,"Content-Length: %ld\r\n\r\n",strlen(mes));
		strcat(back,buf);
		strcat(back,mes);
//jpg
fp=fopen("1.jpg","rb");
	fseek(fp,0,SEEK_END);
	content=ftell(fp);
	fclose(fp);
	sprintf(buf,"Content-Length: %d\r\n\r\n",content);
	strcat(back_jpg,buf);


	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				perror("accept error");
		}
		printf("client getin\n");
		
		if ( (childpid = fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
				/* process the request */
		for ( ; ; ) {
			if ( (read(connfd, buf, sizeof(buf))) == 0){
				break;	/* connection closed by other end */	
				}
				if(strncmp(buf2,buf,strlen(buf2)))
					responce(1,connfd);
					else responce(2,connfd);		
			}
			printf(" client getout\n");
			exit(0);
		}
		close(connfd);			/* parent closes connected socket */
		
	}
}
