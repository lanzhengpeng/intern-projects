### 【1531】list immediate components 列出立即组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
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
        int  
            ii,  
            n_children;  
        tag_t  
            *children,  
            comp = NULL_TAG,  
            comp_part,  
            inst,  
            parent,  
            part = UF_ASSEM_ask_work_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        ECHO("By cycling...\n");  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
            parent = UF_ASSEM_ask_parent_of_instance(inst);  
            if (parent == UF_ASSEM_ask_work_part())  
            {  
                comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
                UF_PART_ask_part_name(comp_part, part_name);  
                ECHO("%s is in the work part\n", part_name);  
            }  
        }  
        ECHO("Directly...\n");  
        n_children = UF_ASSEM_ask_part_occ_children(root_part_occ, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(children[ii]);  
            UF_PART_ask_part_name(comp_part, part_name);  
            ECHO("%s is in the work part\n", part_name);  
        }  
        if (n_children > 0) UF_free(children);  
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

> 这段代码是NX的二次开发代码，其主要功能是遍历装配中的所有组件，并打印出它们的名称。
>
> 关键点包括：
>
> 1. 错误处理：定义了错误处理函数report_error，用于打印出错误代码和相关信息。
> 2. 组件遍历：定义了组件遍历函数ask_next_component，用于按组件类型遍历装配中的所有组件。
> 3. 组件信息查询：在do_it函数中，首先通过组件遍历函数遍历装配中的所有组件，并获取组件的实例、父组件等信息，然后打印出组件名称。
> 4. 直接获取组件：通过直接获取装配根组件的所有子组件，并打印出它们的名称。
> 5. 初始化和终止：在ufusr函数中，首先进行NX的初始化，然后执行do_it函数，最后终止NX。
> 6. 卸载提示：在ufusr_ask_unload函数中，返回立即卸载的提示。
>
> 总的来说，这段代码通过遍历装配中的所有组件，并打印出它们的名称，实现了查询装配结构的目的。
>
