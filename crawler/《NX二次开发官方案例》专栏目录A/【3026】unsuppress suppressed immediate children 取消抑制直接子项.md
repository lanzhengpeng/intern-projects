### 【3026】unsuppress suppressed immediate children 取消抑制直接子项

#### 代码

```cpp
    /*HEAD UNSUPPRESS_SUPPRESSED_IMMEDIATE_CHILDREN CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的功能，主要用于打印系统日志信息。 */  
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
    /*  
        This function is basically the same as UF_ASSEM_ask_part_occ_children  
        except that suppressed components are included.  
    */  
    static int ask_all_immediate_children(tag_t part, tag_t **children)  
    {  
        tag_t  
            comp = NULL_TAG,  
            inst,  
            parent;  
        uf_list_p_t  
            child_list;  
        UF_CALL(UF_MODL_create_list(&child_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            if ((inst = UF_ASSEM_ask_inst_of_part_occ(comp)) != NULL_TAG)  
            {  
                parent = UF_ASSEM_ask_parent_of_instance(inst);  
                if (parent == part)  
                    UF_CALL(UF_MODL_put_list_item(child_list, comp));  
            }  
        }  
        return (make_an_array(&child_list, children));  
    }  
    static void do_it(void)  
    {  
        logical  
            suppressed;  
        int  
            *fails,  
            ii,  
            n_sup_children,  
            n_children;  
        tag_t  
            *children,  
            inst,  
            part = UF_PART_ask_display_part(),  
            *sup_array;  
        uf_list_p_t  
            sup_list;  
        UF_CALL(UF_MODL_create_list(&sup_list));  
        n_children = ask_all_immediate_children(part, &children);  
        ECHO(n_children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(children[ii]);  
            UF_CALL(UF_ASSEM_ask_suppress_state(inst, &suppressed));  
            if (suppressed) UF_CALL(UF_MODL_put_list_item(sup_list, inst));  
        }  
        if (n_children > 0) UF_free(children);  
        n_sup_children = make_an_array(&sup_list, &sup_array);  
        ECHO(n_sup_children);  
        if (n_sup_children > 0)  
        {  
            UF_CALL(allocate_memory(n_sup_children * sizeof(int), (void *)&fails));  
            UF_CALL(UF_ASSEM_unsuppress_instances(n_sup_children,sup_array,fails));  
            for (ii = 0; ii < n_sup_children; ii++)  
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

> 这段代码是NX Open API二次开发代码，主要实现了以下功能：
>
> 1. 定义了report_error函数，用于在调用UF函数出错时报告错误信息。
> 2. 定义了allocate_memory函数，用于分配内存。
> 3. 定义了make_an_array函数，用于将对象列表转换为对象数组。
> 4. 定义了ask_next_component函数，用于遍历获取当前部件中的下一个组件。
> 5. 定义了ask_all_immediate_children函数，用于获取当前部件的所有直接子组件，包括被抑制的组件。
> 6. 定义了do_it函数，用于获取当前显示部件的所有直接子组件，包括被抑制的组件，并取消抑制这些被抑制的组件。
> 7. 定义了ufusr函数，用于初始化和调用do_it函数，最后终止UF。
> 8. 定义了ufusr_ask_unload函数，用于卸载NX Open API。
>
> 总体来说，这段代码实现了在NX中获取当前显示部件的所有直接子组件，包括被抑制的组件，并取消抑制这些被抑制的组件的功能。
>
