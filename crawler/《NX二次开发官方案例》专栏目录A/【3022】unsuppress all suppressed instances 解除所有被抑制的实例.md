### 【3022】unsuppress all suppressed instances 解除所有被抑制的实例

#### 代码

```cpp
    /*HEAD UNSUPPRESS_ALL_SUPPRESSED_INSTANCES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释中的提示，UF_print_syslog是V18版本中新增的函数。因此，只需翻译注释，而不需要添加其他内容。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        logical  
            suppressed;  
        int  
            *fails,  
            ii,  
            n;  
        tag_t  
            comp = NULL_TAG,  
            inst,  
            part = UF_PART_ask_display_part(),  
            *sup_array;  
        uf_list_p_t  
            sup_list;  
        UF_CALL(UF_MODL_create_list(&sup_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
            UF_CALL(UF_ASSEM_ask_suppress_state(inst, &suppressed));  
            if (suppressed) UF_CALL(UF_MODL_put_list_item(sup_list, inst));  
        }  
        n = make_an_array(&sup_list, &sup_array);  
        if (n > 0)  
        {  
            UF_CALL(allocate_memory(n * sizeof(int), (void *)&fails));  
            UF_CALL(UF_ASSEM_unsuppress_instances(n, sup_array, fails));  
            for (ii = 0; ii < n; ii++)  
            {  
                if (fails[ii] != 0)  
                {  
                    ECHO(ii);  
                    UF_CALL(fails[ii]);  
                }  
            }  
            UF_free(sup_array);  
            UF_free(fails);  
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

> 这段代码是用于NX二次开发的一个示例，其主要功能是遍历当前显示的部件中的所有组件实例，并将被抑制的组件实例解抑制。
>
> 主要功能包括：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate初始化和终止NX API。
> 2. 遍历组件：使用UF_OBJ_cycle_objs_in_part和UF_OBJ_ask_type_and_subtype遍历当前显示的部件中的所有组件实例。
> 3. 获取抑制状态：使用UF_ASSEM_ask_inst_of_part_occ和UF_ASSEM_ask_suppress_state获取组件实例的抑制状态。
> 4. 解抑制：使用UF_ASSEM_unsuppress_instances解抑制被抑制的组件实例。
> 5. 错误处理：使用自定义的report_error函数来报告API调用错误。
> 6. 内存管理：使用UF_allocate_memory和UF_free来管理内存。
> 7. 创建和解构列表：使用UF_MODL_create_list、UF_MODL_put_list_item、UF_MODL_ask_list_count和UF_MODL_delete_list来创建和解构组件实例列表。
> 8. 用户自定义函数：提供了ufusr和ufusr_ask_unload函数供用户自定义调用。
>
> 总的来说，这段代码是一个典型的NX二次开发示例，通过遍历组件实例并调用NX API来实现解抑制的功能。
>
