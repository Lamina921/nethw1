/* include fig01 */
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
	int					i, maxi, maxfd, listenfd, connfd, sockfd,cli=0;
	FILE *fp;
	int content=0;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE],buf2[MAXLINE]="GET /home/%E6%A1%8C%E9%9D%A2/net/1/1.jpg";
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(80);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
/* end fig01 */


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


/* include fig02 */
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
			cli++;		
			
			
			
			printf("%d client getin\n",cli);
#if NODEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				exit(1);

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}


		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {	
		
				if ( (read(sockfd, buf, MAXLINE)) == 0) {
						/*4connection closed by client */
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
					cli--;
					printf(" client getout\n");
				} else
					{
					fp=fopen("log.txt","a+");
					fputs(buf,fp);
					fclose(fp);	
					if(strncmp(buf2,buf,strlen(buf2)))
					responce(1,connfd);
					else responce(2,connfd);
				
}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */

