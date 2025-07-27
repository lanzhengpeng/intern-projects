### 【2414】report unloaded components 报告未加载的组件

#### 代码

```cpp
    /*HEAD REPORT_UNLOADED_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <stdarg.h>  
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
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform));  
        /*  This will show the full path for the component based on the current  
            assembly load options.  */  
            if ((UF_PART_is_loaded(part_name)) == 0)  
                ECHO("%s - is not loaded\n", part_name);  
        }  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前显示部件中的所有组件，并检查它们是否被加载。如果某个组件未被加载，则输出其名称。
>
> 主要内容包括：
>
> 1. 定义了ECHO宏，用于在列表窗口和系统日志中输出信息。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 3. ask_next_component函数用于遍历当前部件中的组件，并返回下一个组件的tag。
> 4. do_it函数用于遍历所有组件，并检查它们是否被加载。未被加载的组件会被输出。
> 5. ufusr函数是NX的二次开发入口函数，调用UF_initialize初始化NX，然后调用do_it执行主要功能，最后调用UF_terminate终止NX。
> 6. ufusr_ask_unload函数用于设置组件卸载的选项。
>
> 总体来说，这段代码通过遍历组件并检查加载状态，实现了报告未加载组件的功能。
>
