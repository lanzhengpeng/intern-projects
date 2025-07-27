### 【2210】report modified status of components 组件的修改状态报告

#### 代码

```cpp
    /*HEAD REPORT_MODIFIED_STATUS_OF_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOL(X)   (X ? printf("%s = TRUE\n", #X) : printf("%s = FALSE\n", #X))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog是在V18版本中新增的一个功能。文档提到这个功能是新的，并且要求只回答关于翻译的内容，不要添加任何无关的评论。所以，关于UF_print_syslog的功能和作用，文档并未提供详细信息，仅仅说明了它是V18版本的一个新增功能。 */  
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
        UF_PART_load_status_t   
            error_status;  
        tag_t  
            comp = NULL_TAG,  
            comp_part = NULL_TAG,  
            part_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            comp_part_name[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        logical  
            part_is_modified;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            printf("  -------------------------------------------------\n");  
            ECHO(comp);  
            if (!UF_OBJ_ask_name(comp, name))  
                printf("  UF_OBJ_ask_name = %s\n", name);  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform));  
            printf("  UF_ASSEM_ask_component_data = %s\n", instance_name);  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            UF_PART_ask_part_name(comp_part, comp_part_name );  
            ECHOS(comp_part_name);  
            if ((UF_PART_is_loaded(comp_part_name)) != 1)  
            {  
                UF_CALL(UF_PART_open_quiet(comp_part_name, &part_tag, &error_status));  
            }  
            part_is_modified = UF_PART_is_modified(comp_part);  
            ECHOL(part_is_modified);  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用UF函数失败时的错误代码和错误消息。
> 2. 定义了一个函数ask_next_component，用于遍历组件并返回下一个组件的tag。
> 3. 定义了一个函数do_it，用于遍历组件，获取组件的名称、原型零件名称、是否已加载、是否已修改等信息，并打印出来。
> 4. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 5. 定义了一个返回卸载策略的函数ufusr_ask_unload。
> 6. 使用了宏定义来简化打印变量值、布尔值、字符串等操作。
> 7. 通过调用NX提供的UF函数来获取组件信息，如名称、原型零件、是否已加载等。
> 8. 使用了一个循环来遍历所有组件，并对每个组件执行操作。
> 9. 通过错误报告函数来处理UF函数调用失败的情况。
> 10. 实现了对组件的各种属性信息的查询，并将结果打印输出。
>
> 以上是对这段NX二次开发代码的介绍。
>
