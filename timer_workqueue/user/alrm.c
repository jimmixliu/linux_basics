#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>



void timer_prof_function(int signo)
{
	printf("SIGPROF = %d\n",signo);
}

void timer_alarm_function(int signo)
{
	printf("alarm = %d\n",signo);
	alarm(2);
}

void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = timer_prof_function;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);

}

void init_time(void)
{
        struct itimerval val;

	val.it_value.tv_sec = 2;
	val.it_value.tv_usec = 0;
	val.it_interval = val.it_value;
	setitimer(ITIMER_PROF,&val,NULL);
}

void init_alrm(void)
{
	signal(SIGALRM,timer_alarm_function);
	alarm(2);
}


int main(int argc, char *argv[])
{

	init_time();
	init_sigaction();

//	init_alrm();
	
	while(1);

	return 0;
}
