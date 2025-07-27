### 【1508】list all promoted bodies 列出所有提升的实体

#### 代码

```cpp
    /*HEAD LIST_ALL_PROMOTED_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是 V18 新增的功能。 */  
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
    static tag_t ask_next_promotion(tag_t part, tag_t obj)  
    {  
        int  
            subtype,  
            type;  
        while ((obj = UF_OBJ_cycle_all(part, obj)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype) &&  
                UF_OBJ_is_object_a_promotion(obj)) return obj;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            obj_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((obj_tag = ask_next_promotion(part, obj_tag)) != NULL_TAG)  
            printf("Promoted body eid =  %d\n", obj_tag);  
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

> 这段代码是用于NX的二次开发，主要功能是遍历当前显示部件中的所有实体，并找出被提升(promoted)的实体，然后打印它们的EID。
>
> 主要思路如下：
>
> 1. 定义了一个报告错误的函数report_error，用于打印错误信息。
> 2. 定义了一个循环遍历部件中所有实体的函数ask_next_promotion，找出被提升的实体。
> 3. 定义了主函数do_it，通过调用ask_next_promotion函数，找出所有被提升的实体，并打印它们的EID。
> 4. 定义了ufusr函数，在NX启动时调用，初始化NX，调用do_it函数，然后终止NX。
> 5. 定义了ufusr_ask_unload函数，用于卸载NX时立即卸载二次开发。
>
> 总体来说，这段代码通过遍历部件中的实体，筛选出被提升的实体，并打印它们的EID。它使用了NX提供的UF函数来实现二次开发。
>
