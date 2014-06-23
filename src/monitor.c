#include "monitor.h"
#include "Ecore.h"
#include "Ecore_File.h"

#define BUFFER_SIZE 1024

static Eina_Bool already = EINA_FALSE;

static Eina_Bool
_msg_from_child_handler(void *data EINA_UNUSED, int type EINA_UNUSED, void *event)
{
  printf("COME TO THIS FCCCCCCCCCCCCCI received a message from my child \n");
   Ecore_Exe_Event_Data *dataFromProcess = (Ecore_Exe_Event_Data *)event;
   char msg[BUFFER_SIZE];

   if (dataFromProcess->size >= (BUFFER_SIZE - 1))
     {
        fprintf(stdout, "Data too big for bugger. error\n");
        return ECORE_CALLBACK_DONE;
     }

   strncpy(msg, dataFromProcess->data, dataFromProcess->size);
   msg[dataFromProcess->size] = 0;

   if (strcmp(msg, "quit") == 0)
     {
        fprintf(stdout, "My child said to me, QUIT!\n");
        ecore_main_loop_quit();
     }
   else
    {
     //fprintf(stdout, "I received a message from my child: %s\n", msg);
     printf("I received a message from my child: %s\n", msg);
    }

   return ECORE_CALLBACK_DONE;

}


static void chris_free(void * data, const Ecore_Exe* exe)
{
  printf("DID YOU CALL ME !!!!!!!!!!!!!!!!!!!\n");
  already = EINA_FALSE;
  
}



static void
monitor_callback(void *data, Ecore_File_Monitor *em, Ecore_File_Event event, const char *path)
{
  if (already) return;
  //resource_texture_update(data, path);
  if (eina_str_has_extension(path, ".c")
        ||eina_str_has_extension(path, ".h")
        ) {
    const char* command = data;
    printf("event : %d, path %s, command %s \n", event, path, command);


    pid_t childPid;
    Ecore_Exe *childHandle;

    childHandle = ecore_exe_pipe_run(command,
          //ECORE_EXE_PIPE_WRITE |
          //ECORE_EXE_PIPE_READ_LINE_BUFFERED |
          ECORE_EXE_PIPE_READ, NULL);
          //ECORE_EXE_NONE, NULL);
          //ECORE_EXE_PIPE_AUTO, NULL);

   if (childHandle == NULL){
     fprintf(stderr, "Could not create a child process!\n");
     return;
   }

   childPid = ecore_exe_pid_get(childHandle);

   if (childPid == -1)
     fprintf(stderr, "Could not retrive the PID!\n");
   else {
     //fprintf(stdout, "The child process has PID:%u\n", (unsigned int)childPid);
     fprintf(stdout, "The child process has PID:%u\n", (unsigned int)childPid);
    already = EINA_TRUE;
   }
   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _msg_from_child_handler, NULL);
   ecore_exe_callback_pre_free_set(childHandle, chris_free);

  }


}


void monitor_new(const char* path, Eina_List* extensions, const char* cmd)
{
  Ecore_File_Monitor * efm = ecore_file_monitor_add( path,
        monitor_callback,
        (void*) eina_stringshare_add(cmd)
        );
}


void
monitor_init()
{
  ecore_file_init(); 
}

void
monitor_shutdown()
{
  ecore_file_shutdown();
}
