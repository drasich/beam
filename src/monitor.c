#include "monitor.h"
#include "Ecore.h"
#include "Ecore_File.h"

#define BUFFER_SIZE 1024

static Eina_Bool already = EINA_FALSE;

static Eina_Bool
_msg_from_child_handler(void *data, int type EINA_UNUSED, void *event)
{
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
     if (data){
       Display* d = data;
       d->text_add(d, msg);
      }
    }

   return ECORE_CALLBACK_DONE;

}


static void chris_free(void * data, const Ecore_Exe* exe)
{
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

    Monitor* m = data;
    printf("event : %d, path %s, command %s \n", event, path, m->command);


    pid_t childPid;
    Ecore_Exe *childHandle;

    childHandle = ecore_exe_pipe_run(m->command,
          //ECORE_EXE_PIPE_WRITE |
          //ECORE_EXE_PIPE_READ_LINE_BUFFERED |
          ECORE_EXE_PIPE_ERROR |
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
   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _msg_from_child_handler, m->display);
   ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _msg_from_child_handler, m->display);
   ecore_exe_callback_pre_free_set(childHandle, chris_free);

  }


}


Monitor* monitor_new(const char* path, Eina_List* extensions, const char* cmd)
{
  Monitor* m = calloc(1, sizeof *m);

  m->command = eina_stringshare_add(cmd);
  //m->path = path;

  Ecore_File_Monitor * efm = ecore_file_monitor_add( path,
        monitor_callback,
        m
        );

  return m;
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
