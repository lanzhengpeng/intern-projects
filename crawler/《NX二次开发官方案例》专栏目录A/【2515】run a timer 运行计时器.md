### 【2515】run a timer 运行计时器

#### 代码

```cpp
    /*HEAD RUN_A_TIMER CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to TIMER  
        2.  Assign the constructor "const_cb" on the Callbacks tab  
        3.  Add a Radio Box  
        4.  Set the Radio Box items to "Start" and "Stop"  
        5.  Set the intial integer value to 1  
        6.  Assign the value changed "radio_cb" on the Callbacks tab  
        7.  Add a Push Button  
        8.  Assign the value changed "action_cb" on the Callbacks tab  
        9.  Save the dialog to your application directory as timer.dlg  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <stdarg.h>  
    #define TIMER_RADIO_0                  ("RADIO_0")  
    #define TIMER_ACTION_1                 ("ACTION_1")  
    typedef struct timer_data_s  
    {  
        double  
            elapsed;  
        UF_timer_t  
            timer;  
        char  
            record[MAX_FSPEC_SIZE+1],  
            filter[MAX_FSPEC_SIZE+1];  
    } timer_data_t, *timer_data_p_t;  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void TIMER_set_title(int dialog_id, timer_data_p_t data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        char  
            title[133];  
        sprintf(title, "Total time = %.2f hours", data->elapsed/3600);  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = UF_STYLER_DIALOG_INDEX;  
        set_value.value.string = title;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    }  
    static void TIMER_set_sensitivity(int dialog_id, int sensitivity)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_SENSITIVITY;  
        set_value.item_id = TIMER_ACTION_1;  
        set_value.value.integer = sensitivity;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_id = UF_STYLER_NAV_INDEX;  
        set_value.subitem_index = UF_STYLER_CANCEL_INDEX;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_SENSITIVITY;  
        set_value.item_id = TIMER_RADIO_0;  
        set_value.subitem_index = 0;  
        set_value.value.integer = sensitivity;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.subitem_index = 1;  
        set_value.value.integer = abs(sensitivity - 1);  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    }  
    static void TIMER_start(int dialog_id, timer_data_p_t data)  
    {  
        UF_CALL(UF_begin_timer(&data->timer));  
        TIMER_set_sensitivity(dialog_id, FALSE);  
    }  
    static void TIMER_stop(int dialog_id, timer_data_p_t data)  
    {  
        UF_timer_values_t  
            values;  
        FILE  
            *data_file;  
        UF_CALL(UF_end_timer(data->timer, &values));  
        data->elapsed = data->elapsed + values.real_time;  
        data_file = fopen(data->record, "w");  
        fprintf(data_file, "%lf", data->elapsed);  
        fclose(data_file);  
        TIMER_set_title(dialog_id, data);  
        TIMER_set_sensitivity(dialog_id, TRUE);  
    }  
    static void TIMER_load(timer_data_p_t data)  
    {  
        FILE  
            *data_file;  
        data_file = fopen(data->record, "r");  
        if (data_file != NULL)  
        {  
            fscanf(data_file, "%lf", &data->elapsed);  
            fclose(data_file);  
        }  
        else data->elapsed = 0;  
    }  
    static int TIMER_radio_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        timer_data_p_t  
            data = client_data;  
        if (callback_data->value.integer == 0)  
            TIMER_start(dialog_id, data);  
        else  
            TIMER_stop(dialog_id, data);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int TIMER_action_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            resp;  
        timer_data_p_t  
            data = client_data;  
        UF_STYLER_item_value_type_t  
            set_value;  
        if (!UF_CALL(UF_UI_create_filebox("Enter timer data filespec", "Timer File",  
            data->filter, NULL, data->record, &resp)) && (resp == UF_UI_OK))  
        {  
            set_value.item_attr = UF_STYLER_LABEL;  
            set_value.item_id = TIMER_ACTION_1;  
            set_value.value.string = data->record;  
            UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
            TIMER_load(data);  
            TIMER_set_title(dialog_id, data);  
            TIMER_set_sensitivity(dialog_id, TRUE);  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            TIMER_cbs[4] = {  
                {UF_STYLER_DIALOG_INDEX,UF_STYLER_CONSTRUCTOR_CB,0,TIMER_action_cb},  
                {TIMER_RADIO_0, UF_STYLER_VALUE_CHANGED_CB, 0, TIMER_radio_cb},  
                {TIMER_ACTION_1, UF_STYLER_ACTIVATE_CB, 0, TIMER_action_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        timer_data_t  
            data = { 0, 0, "timer.dat", "*.dat" };  
        UF_CALL(UF_STYLER_create_dialog("timer.dlg", TIMER_cbs, 4, &data, &resp));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要实现了一个定时器功能。
>
> 主要功能包括：
>
> 1. 创建一个定时器对话框，包含一个单选框用来控制定时器的开始和停止，以及一个按钮用来选择定时器数据文件。
> 2. 单选框的回调函数TIMER_radio_cb会根据选择来启动或停止定时器。
> 3. 按钮的回调函数TIMER_action_cb会弹出一个文件选择对话框，让用户选择定时器数据文件。
> 4. 数据结构timer_data_t用于存储定时器的相关数据，如累计时间、数据文件路径等。
> 5. TIMER_start和TIMER_stop函数用于控制定时器的开始和停止，并更新对话框标题。
> 6. TIMER_load函数用于从文件加载累计时间数据。
> 7. TIMER_set_title和TIMER_set_sensitivity函数用于更新对话框标题和控件灵敏度。
> 8. ufusr函数是程序的入口，负责初始化和调用do_it函数。
> 9. ufusr_ask_unload函数用于卸载程序。
>
> 总体来说，这段代码实现了一个基本的定时器功能，用户可以通过对话框来控制定时器的开始、停止，并保存定时器的累计时间。
>
