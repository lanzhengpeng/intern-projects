### 【2395】report top level assemblies 报告顶层装配体

#### 代码

```cpp
    /*HEAD REPORT_TOP_LEVEL_ASSEMBLIES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog是V18版本新增的函数。 */  
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
    static int ask_top_parents(tag_t child, tag_t **top_parents)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        uf_list_p_t  
            rel,  
            rel_list,  
            top_list;  
        UF_CALL(UF_MODL_create_list(&rel_list));  
        UF_CALL(UF_MODL_create_list(&top_list));  
        UF_CALL(UF_MODL_put_list_item(rel_list, child));  
        for (rel = rel_list; rel != NULL; rel = rel->next)  
        {  
            n = UF_ASSEM_where_is_part_used(rel->eid, &parents);  
            if (n == 0)  
                UF_CALL(UF_MODL_put_list_item(top_list, rel->eid));  
            else  
            {  
                for (ii = 0; ii < n; ii++)  
                    UF_CALL(UF_MODL_put_list_item(rel_list, parents[ii]));  
                UF_free(parents);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&rel_list));  
        return make_an_array(&top_list, top_parents);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents,  
            part = UF_PART_ask_display_part();  
        char  
            name[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, name);  
        if ((n = ask_top_parents(part, &parents)) > 0)  
        {  
            printf("%s is used in top level assembly:\n", name);  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_PART_ask_part_name(parents[ii], name);  
                printf("\t%s\n", name);  
            }  
            UF_free(parents);  
        }  
        else  
            printf("%s is the top level assembly\n", name);  
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

> 这段代码是一个用于NX CAD系统的二次开发示例，主要功能是查询当前显示的部件在顶级装配中的位置。
>
> 主要逻辑如下：
>
> 1. 定义了UF_CALL宏，用于封装NX API调用，并处理错误。
> 2. 实现了allocate_memory函数，用于申请内存。
> 3. 实现了make_an_array函数，用于将列表转换为数组。
> 4. 实现了ask_top_parents函数，用于查询部件的顶级父装配。首先创建一个包含当前部件的列表，然后遍历该列表，查询每个部件所在的父装配，直到找到顶级父装配为止。
> 5. 实现了do_it函数，用于执行查询。首先获取当前显示的部件，然后调用ask_top_parents函数查询其顶级父装配，并打印结果。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。在ufusr中，初始化NX系统，调用do_it函数执行查询，然后终止NX系统。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发模块。
>
> 这段代码利用NX的部件、装配和模型模块的API，实现了查询当前显示部件在顶级装配中的位置的功能，展示了NX二次开发的基本流程。
>
