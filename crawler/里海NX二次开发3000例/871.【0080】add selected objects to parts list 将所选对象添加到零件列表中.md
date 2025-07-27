### 【0080】add selected objects to parts list 将所选对象添加到零件列表中

#### 代码

```cpp
    /*HEAD ADD_SELECTED_OBJECTS_TO_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_objects;  
        tag_t  
            note,  
            *objects,  
            plist = UF_PLIST_ask_tag();  
        double  
            loc[3] = { 0,0,0 };  
        if ((n_objects = select_objects("Select objects", &objects)) > 0)  
        {  
            UF_CALL(UF_PLIST_add_entities(plist, n_objects, objects));  
            UF_CALL(UF_PLIST_create_note(plist, loc, &note));  
            UF_free(objects);  
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

> 这段代码是用于NX的二次开发，其主要功能是让用户选择对象，并将其添加到NX的部件列表中。代码主要包含以下几个部分：
>
> 1. 引入必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API出错时输出错误信息。
> 3. 定义了一个选择对象函数select_objects，用于让用户通过选择对话框选择对象，并返回选择的对象数组。
> 4. 定义了一个执行函数do_it，用于获取当前部件列表的标签，调用select_objects函数让用户选择对象，将对象添加到部件列表中，并在部件列表中添加一个注释。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数，在初始化NX API后调用do_it函数执行操作，在操作完成后终止NX API。
> 6. 定义了一个卸载函数ufusr_ask_unload，用于立即卸载二次开发程序。
>
> 综上所述，这段代码实现了让用户选择对象并添加到NX部件列表的功能，通过NX的二次开发接口实现了NX的扩展功能。
>
