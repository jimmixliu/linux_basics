#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *printf_message_function(void *ptr)
{
	int i = 0;

	for (i = 0; i < 5; i++){
		printf("%s:%d\n",(char *)ptr,i);
		sleep(1);
	}

	return ((void *)6);
}

int main(int argc, char **argv)
{
	int ret1,ret2;
	char *message1="pthread1";
	char *message2="pthread2";
	pthread_t thread1,thread2;
	void * retval;

	printf("start pthread_create\n");

	ret1 = pthread_create(&thread1,NULL,(void *)&printf_message_function,(void *)message1);
	ret2 = pthread_create(&thread2,NULL,(void *)&printf_message_function,(void *)message2);
	if(0 != ret1){
		printf("pthread_create thread1 failed\n");
		return -1;
	}if(0 != ret2){
		printf("pthread_create thread2 failed\n");
		return -1;
	}

	printf("start waite\n");
	
	pthread_join(thread1,(void *)&retval);
	printf("thread1 return value is %ld\n",(intptr_t)retval);
	pthread_join(thread2,(void *)&retval);
	printf("thread2 return value is %ld\n",(intptr_t)retval);

	return 0;
}
