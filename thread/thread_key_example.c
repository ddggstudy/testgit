#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

static pthread_key_t thread_log_key;

void write_to_thread_log(const char* msg)
{
	FILE* thread_log = (FILE*) pthread_getspecific(thread_log_key);
	fprintf(thread_log, "%s\n", msg);
}

void close_thread_log(void* thread_log_fd)
{
	fclose((FILE*)thread_log_fd);
}

void* thread_function(void* args)
{
	char thread_log_filename[20] = { 0 };
	FILE* thread_log_fd;
	
	sprintf(thread_log_filename, "thread%d.log", (int)pthread_self());
	
	thread_log_fd = fopen(thread_log_filename, "w");
	
	pthread_setspecific(thread_log_key, thread_log_fd);
	
	write_to_thread_log("Thread starting.");

	return NULL;
}

int main()
{
	int i;
	
	pthread_t threads[5];
	
	pthread_key_create(&thread_log_key, close_thread_log);

	for(i = 0; i < 5; ++i)
	{
		pthread_create(&(threads[i]), NULL, thread_function, NULL);
	}

	for(i = 0; i < 5; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;

}

