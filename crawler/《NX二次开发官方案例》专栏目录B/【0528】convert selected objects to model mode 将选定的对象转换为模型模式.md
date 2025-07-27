### 【0528】convert selected objects to model mode 将选定的对象转换为模型模式

#### 代码

```cpp
    /*HEAD CONVERT_SELECTED_OBJECTS_TO_MODEL_MODE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog 是 V18 版本中新增的一个功能，用于打印系统日志。在 V18 版本之前，需要使用其他方法来查看系统日志。该功能提供了更方便的方式来查看系统日志。 */  
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
    static int select_drf_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static void convert_to_model(tag_t object)  
    {  
        int  
            view_dep;  
        tag_t  
            view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!uc6409(object, &view_dep, view_name) && view_dep)  
        {  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
            UF_CALL(UF_VIEW_convert_to_model(view, object));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objs;  
        tag_t  
            *objs;  
        while ((n_objs = select_drf_objects("Convert to model", &objs)) > 0)  
        {  
            for (ii = 0; ii < n_objs; ii++) convert_to_model(objs[ii]);  
            UF_free(objs);  
        }  
    }  
    /* 里海 */  
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

> 根据代码内容，这是一个NX的二次开发示例代码，主要实现了以下功能：
>
> 1. 错误报告：定义了一个报告错误的函数，用于在函数调用出错时打印错误信息到系统日志和列表窗口。
> 2. 选择图形对象：定义了一个选择图形对象的函数，用于弹出一个对话框让用户选择图形对象，并高亮显示所选对象。
> 3. 转换图形对象到模型：定义了一个转换图形对象到模型的函数，如果图形对象依赖于视图，则先获取视图，然后调用UF_VIEW_convert_to_model接口将其转换到模型。
> 4. 执行流程：在do_it函数中，循环调用选择图形对象的函数，获取用户所选对象，然后遍历调用转换函数。
> 5. 初始化和终止：在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it函数执行流程，最后调用UF_terminate终止。
> 6. 卸载要求：定义了一个卸载要求函数，要求立即卸载。
>
> 综上所述，该代码实现了在NX中通过二次开发选择图形对象并转换为模型的功能。
>
