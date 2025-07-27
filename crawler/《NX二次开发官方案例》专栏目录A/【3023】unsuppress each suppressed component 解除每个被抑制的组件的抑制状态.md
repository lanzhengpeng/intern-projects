### 【3023】unsuppress each suppressed component 解除每个被抑制的组件的抑制状态

#### 代码

```cpp
    /*HEAD UNSUPPRESS_EACH_SUPPRESSED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的描述，UF_print_syslog 是 V18 版本新增的函数。因此，对于这个问题的回答是：

UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void do_it(void)  
    {  
        logical  
            suppressed;  
        int  
            fail;  
        tag_t  
            comp = NULL_TAG,  
            inst,  
            part = UF_PART_ask_display_part();  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
            UF_CALL(UF_ASSEM_ask_suppress_state(inst, &suppressed));  
            if (suppressed)  
            {  
                UF_CALL(UF_ASSEM_unsuppress_instances(1, &inst, &fail));  
                UF_CALL(fail);  
            }  
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

> 根据代码内容，这是一段用于NX二次开发的代码，其主要功能是取消NX部件的抑制状态。
>
> 具体来说：
>
> 1. 初始化和终止：代码首先使用UF_initialize()进行NX初始化，然后执行主要功能函数do_it()，最后使用UF_terminate()终止NX。
> 2. 取消抑制状态：do_it()函数会遍历当前打开的NX部件的所有组件，判断每个组件是否被抑制，如果被抑制，则使用UF_ASSEM_unsuppress_instances()取消抑制状态。
> 3. 错误处理：代码使用了宏UF_CALL来封装NX函数调用，并在调用失败时输出错误信息。同时定义了report_error()函数来打印错误信息。
> 4. 组件遍历：ask_next_component()函数用于遍历部件的所有组件，并返回下一个组件的tag。
> 5. 二次开发函数：ufusr()是二次开发的主要函数，用于执行取消抑制的功能。ufusr_ask_unload()函数用于返回卸载标志。
> 6. 使用NX API：代码使用了NX提供的API函数，如UF_OBJ_cycle_objs_in_part()、UF_OBJ_ask_type_and_subtype()、UF_ASSEM_ask_inst_of_part_occ()、UF_ASSEM_ask_suppress_state()、UF_ASSEM_unsuppress_instances()等。
>
> 综上所述，这段代码实现了在NX中取消部件抑制状态的功能，展示了二次开发的基本流程和错误处理方法，以及如何使用NX API进行组件遍历和状态操作。
>
