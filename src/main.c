#include <Elementary.h>
#include "monitor.h"

 static void
 on_done(void *data, Evas_Object *obj, void *event_info)
 {
    // quit the mainloop (elm_run function will return)
    elm_exit();
 }

Evas_Object*
create_panes(Evas_Object* win, Eina_Bool hor)
{
  Evas_Object* panes = elm_panes_add(win);
  elm_panes_horizontal_set(panes, hor);
  evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(panes);

  return panes;
}


 EAPI_MAIN int
 elm_main(int argc, char **argv)
 {
    Evas_Object *win, *box, *lab, *btn;

    // new window - do the usual and give it a name (hello) and title (Hello)
    win = elm_win_util_standard_add("hello", "Hello");
    // when the user clicks "close" on a window there is a request to delete
    elm_win_autodel_set(win, EINA_TRUE);
    evas_object_smart_callback_add(win, "delete,request", on_done, NULL);

    // add a box object - default is vertical. a box holds children in a row,
    // either horizontally or vertically. nothing more.
    box = elm_box_add(win);
    // make the box horizontal
    elm_box_horizontal_set(box, EINA_TRUE);
    // add object as a resize object for the window (controls window minimum
    // size as well as gets resized if window is resized)
    //elm_win_resize_object_add(win, box);
    evas_object_show(box);

    // add a label widget, set the text and put it in the pad frame
    lab = elm_label_add(win);
    // set default text of the label
    elm_object_text_set(lab, "Hello out there world!");
    // pack the label at the end of the box
    elm_box_pack_end(box, lab);
    evas_object_show(lab);

    // add an ok button
    btn = elm_button_add(win);
    // set default text of button to "OK"
    elm_object_text_set(btn, "OK");
    // pack the button at the end of the box
    elm_box_pack_end(box, btn);
    evas_object_show(btn);
    // call on_done when button is clicked
    evas_object_smart_callback_add(btn, "clicked", on_done, NULL);

    // now we are done, show the window
    evas_object_resize(win, 220, 80);
    evas_object_show(win);

    monitor_init();
    //const char* path = "/home/chris/code/slime/src";
    const char* path = "/home/chris/code/slime";
    const char* command = "cd /home/chris/code/slime && waf";
    //const char* command = "cd /home/chris/code/slime && ls";

    Monitor* m = monitor_new(path, NULL, command);
    Display* d = display_console_new();
    m->display = d;


    // run the mainloop and process events and callbacks
    elm_run();
    elm_shutdown();
    monitor_shutdown();
    return 0;
 }
 ELM_MAIN()

