### 【1504】list all local sketches 列出所有本地草图

#### 代码

```cpp
    /*HEAD LIST_ALL_LOCAL_SKETCHES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的函数，用于打印系统日志消息。 */  
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
    static tag_t ask_next_local_sketch(tag_t part, tag_t sketch)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch))  
            && (sketch != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(sketch)) return sketch;  
        }  
        return NULL_TAG;  
    }  
    #define SHOW_NAME(X) report_object_name(#X, X)  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        if (!UF_OBJ_ask_name(object, obj_name))  
            sprintf(msg, "%s (tag %d) is named %s\n", name, object, obj_name);  
        else  
            sprintf(msg, "%s (tag %d) is not named\n", name, object);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            sketch = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((sketch = ask_next_local_sketch(part, sketch)) != NULL_TAG)  
            SHOW_NAME(sketch);  
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

> 这段代码是一个NX二次开发示例，其主要功能是遍历当前打开零件中的所有本地草图，并打印出它们的名称和标签。
>
> 主要步骤包括：
>
> 1. 引入NX相关的头文件。
> 2. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义ask_next_local_sketch函数，用于遍历零件中的下一个本地草图。
> 4. 定义report_object_name函数，用于打印出对象名称。
> 5. 定义do_it函数，用于遍历零件中的所有本地草图，并打印名称。
> 6. 定义ufusr函数作为二次开发的入口，初始化后调用do_it函数，结束后终止。
> 7. 定义ufusr_ask_unload函数，用于卸载二次开发，这里设置为立即卸载。
>
> 通过这段代码，开发者可以学习到NX二次开发的基本流程，包括错误处理、对象遍历、对象名称查询等常用操作。
>
