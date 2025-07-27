### 【0077】add post action to file new 在“文件”-“新建”中添加POST操作

#### 代码

```cpp
    /*  
    This program demonstrates adding a POST action to File->New.    
    The use of a corresponding PRE action enables the POST action to differentiate between when the user selected OK and/or Cancel.  
    The compiled DLL and the corresponding menufile containing the following lines should be in a custom "startup" folder, e.g. UGII_USER_DIR\startup  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MODIFY  
        BUTTON UG_FILE_NEW  
        ACTIONS/PRE pre_new_callback  
        BUTTON UG_FILE_NEW  
        ACTIONS/POST post_new_callback  
    END_OF_MODIFY  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_attr.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static tag_t original_part = NULL_TAG;  
    static UF_MB_cb_status_t my_pre_cb(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        original_part = UF_PART_ask_display_part();  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    static UF_MB_cb_status_t my_post_cb(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            now_part;  
        char  
            *display_name,  
            part_type[UF_ATTR_MAX_STRING_LEN],  
            part_fspec[MAX_FSPEC_SIZE+1];  
        UF_ATTR_value_t  
            value;  
    /*  Reading a UF_ATTR_string so assign the character array to the pointer 译:翻译：读取一个UF_ATTR字符串，所以将字符数组赋值给指针 */  
        value.value.string = part_type;  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        now_part = UF_PART_ask_display_part();  
        if (now_part != original_part)  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            UF_CALL(UF_PART_ask_part_name(now_part, part_fspec));  
            UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
            if (!is_ugmgr)  
            {  
                ECHO("A new part was created : %s\n", part_fspec);  
            }  
            else  
            {  
                UF_CALL(UF_ATTR_read_value(now_part, "DB_PART_TYPE",  
                    UF_ATTR_string, &value));  
                UF_CALL(UF_PART_file_name_for_display(part_fspec, &display_name));  
                ECHO("A new %s was created : %s\n", part_type, display_name);  
                UF_free(display_name);  
            }  
        }  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "pre_new_callback", my_pre_cb, NULL },  
                             { "post_new_callback", my_post_cb, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要实现了在NX的“文件”菜单下为“新建”添加一个POST动作。具体包括以下功能：
>
> 1. 在“文件”->“新建”的按钮上添加了一个PRE动作和一个POST动作。PRE动作会在“新建”被点击时执行，获取当前显示的部件，而POST动作会在“新建”操作后执行，判断是否创建了新部件。
> 2. 在PRE动作中，调用UF_PART_ask_display_part()获取当前显示的部件，并保存在全局变量original_part中。
> 3. 在POST动作中，再次调用UF_PART_ask_display_part()获取当前显示的部件，与original_part比较，以判断是否创建了新部件。
> 4. 如果创建了新部件，则调用UF_PART_file_name_for_display()获取新部件的显示名称，并输出“创建了一个新部件”。
> 5. 如果创建的是UG管理器中的部件，则还会调用UF_ATTR_read_value()读取新部件的类型，并输出“创建了一个新XX类型的部件”。
> 6. ufsta()函数将PRE和POST动作注册到NX中。
> 7. 通过修改NX的菜单文件，将PRE和POST动作关联到“文件”->“新建”按钮上。
> 8. 当用户点击“文件”->“新建”时，会先执行PRE动作，然后NX执行“新建”操作，最后执行POST动作。通过比较PRE和POST动作中获取的部件，可以判断出用户是否真的创建了新部件。
>
