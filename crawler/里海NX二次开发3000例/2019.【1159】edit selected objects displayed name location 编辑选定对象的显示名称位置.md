### 【1159】edit selected objects displayed name location 编辑选定对象的显示名称位置

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_OBJECTS_DISPLAYED_NAME_LOCATION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数，用于在日志文件中记录信息。文档中指出，UF_print_syslog是V18版本新增的，只回答译文，不要添加其他内容。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        double  
            loc[3];  
        UF_CALL(UF_DISP_set_name_display_status(UF_DISP_NAME_DISPLAY_ON));  
        while (((object = select_anything("Select an object")) != NULL_TAG) &&  
            select_pos("Indicate new name location", loc))  
        {  
            UF_CALL(UF_OBJ_set_name_origin(object, loc));  
            UF_CALL(UF_DISP_add_item_to_display(object));  
        }  
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

> 这是NX二次开发代码，主要功能如下：
>
> 1. 初始化NX环境，并设置选择对话框和提示对话框。
> 2. 选择一个对象，并获取其名称。
> 3. 提示用户输入新的名称位置，并将对象名称的位置更新到新的位置。
> 4. 添加对象到显示列表，以显示新的名称位置。
> 5. 循环执行以上步骤，直到用户选择不继续。
> 6. 在用户请求退出时，立即卸载应用程序。
>
> 代码通过封装UF函数，实现了NX环境初始化、对象选择、位置获取和设置、以及显示列表管理等功能。另外，通过错误报告机制，可以记录函数调用失败的情况，便于调试。整体而言，这是一个典型的NX二次开发示例代码，涵盖了选择、位置设置、显示管理等功能。
>
