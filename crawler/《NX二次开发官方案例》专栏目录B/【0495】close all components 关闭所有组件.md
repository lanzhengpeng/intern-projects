### 【0495】close all components 关闭所有组件

#### 代码

```cpp
    /*HEAD CLOSE_ALL_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog在V18版本中是新增的。 */  
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
    static logical close_component(tag_t comp, void *user_data)  
    {  
        tag_t  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp),  
            *part = (tag_t *)user_data;  
        if ((comp_part != NULL_TAG) && (comp_part != *part))  
            UF_CALL(UF_PART_close(comp_part, 1, 1));  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            all_cset,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &all_cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(all_cset, close_component, &part));  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 初始化和终止NX API：使用UF_initialize()和UF_terminate()来初始化和终止NX API。
> 2. 错误处理：定义了report_error()函数，用于打印出代码执行过程中的错误信息。
> 3. 关闭组件：定义了close_component()函数，用于遍历装配体中的所有组件并关闭它们。
> 4. 主函数：do_it()函数用于获取当前显示的零件，查询该零件的所有组件集合，然后遍历该集合，关闭每个组件。
> 5. NX启动和退出：在ufusr()函数中，先调用UF_initialize()初始化NX API，然后调用do_it()执行主功能，最后调用UF_terminate()终止NX API。
> 6. 卸载请求：ufusr_ask_unload()函数返回立即卸载，表示该代码不需要保留在NX内存中。
>
> 整体来看，这段代码通过NX API实现了关闭当前显示零件的所有组件的功能。其中，错误处理和遍历组件集合的逻辑较为关键。
>
