### 【3163】list all view names 列出所有视图的名称

#### 代码

```cpp
    /*HEAD LIST_ALL_VIEW_NAMES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            obj_name[UF_OBJ_NAME_LEN + 1];  
        printf("%s - tag %d is ", name, object);  
        if (!UF_OBJ_ask_name(object, obj_name)) printf("named %s\n", obj_name);  
        else printf("not named\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            view = NULL_TAG;  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
            report_object_name("view", view);  
    }  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是列出当前零件中的所有视图名称。
>
> 代码的关键部分包括：
>
> 1. 错误报告函数：用于在发生错误时打印错误信息，包括文件名、行号、错误码和错误描述。
> 2. 获取下一个视图函数：利用UF_OBJ_cycle_objs_in_part函数在零件中循环获取视图，并返回下一个视图的tag。
> 3. 报告对象名称函数：用于打印视图的名称，如果未命名则打印“未命名”。
> 4. 执行函数：获取当前显示的零件，然后循环获取每个视图，并调用report_object_name函数打印视图名称。
> 5. ufusr主函数：在NX启动时调用，初始化UF模块，执行do_it函数，然后终止UF模块。
> 6. 卸载请求函数：返回立即卸载标志，表示应用程序可以被立即卸载。
>
> 整体而言，这段代码通过遍历零件中的所有视图，打印出每个视图的名称，实现了列出零件中所有视图名称的功能。
>
