### 【1899】report all component filters 报告所有组件过滤器

#### 代码

```cpp
    /*HEAD REPORT_ALL_COMPONENT_FILTERS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_fltr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_type(part, type, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {  
        logical  
            member;  
        int  
            ii, jj,  
            n_comps,  
            n_fsets,  
            subtype,  
            type;  
        tag_t  
            *comps,  
            *fsets,  
            part = UF_PART_ask_display_part();  
        char  
            comp_name[MAX_ENTITY_NAME_SIZE+1],  
            cset_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        double  
            tol;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_MODL_ask_distance_tolerance(&tol);  
        n_comps = ask_all_of_type(part, UF_component_type, &comps);  
        n_fsets = ask_all_of_type(part, UF_zone_type, &fsets);  
        sprintf(msg, "%d Zones:\n", n_fsets);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < n_fsets; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(fsets[ii], cset_name));  
            sprintf(msg, "  %s\n", cset_name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(fsets[ii], &type, &subtype));  
            for (jj = 0; jj < n_comps; jj++)  
            {  
                switch (subtype)  
                {  
                    case UF_zone_box_subtype:  
                        UF_CALL(UF_FLTR_is_obj_inside_box_zone(tol, fsets[ii],  
                            comps[jj], &member));  
                        break;  
                    case UF_zone_plane_subtype:  
                        UF_CALL(UF_FLTR_is_obj_above_plane_zone(fsets[ii],  
                            comps[jj], &member));  
                        break;  
                }  
                if (member)  
                {  
                    UF_CALL(UF_OBJ_ask_name(comps[jj], comp_name));  
                    sprintf(msg, "    %s\n", comp_name);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
        }  
        if (n_fsets > 0) UF_free(fsets);  
        n_fsets = ask_all_of_type(part, UF_component_set_type, &fsets);  
        sprintf(msg, "\n%d Component Sets:\n", n_fsets);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < n_fsets; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(fsets[ii], cset_name));  
            sprintf(msg, "  %s\n", cset_name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < n_comps; jj++)  
            {  
                UF_CALL(UF_ASSEM_is_member_of_cset(fsets[ii], comps[jj], &member));  
                if (member)  
                {  
                    UF_CALL(UF_OBJ_ask_name(comps[jj], comp_name));  
                    sprintf(msg, "    %s\n", comp_name);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
        }  
        if (n_fsets > 0) UF_free(fsets);  
        n_fsets = ask_all_of_type(part, UF_filter_type, &fsets);  
        sprintf(msg, "\n%d Filters:\n", n_fsets);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < n_fsets; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(fsets[ii], cset_name));  
            sprintf(msg, "  %s\n", cset_name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < n_comps; jj++)  
            {  
                UF_CALL(UF_FLTR_evaluate_filter(comps[jj], fsets[ii], &member));  
                if (member)  
                {  
                    UF_CALL(UF_OBJ_ask_name(comps[jj], comp_name));  
                    sprintf(msg, "    %s\n", comp_name);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
        }  
        if (n_fsets > 0) UF_free(fsets);  
        if (n_comps > 0) UF_free(comps);  
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

> 这段代码是用于NX的二次开发，其主要功能是报告当前部件中所有组件过滤器包含的组件名称。
>
> 代码的主要逻辑如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了ask_next_of_type函数，用于遍历部件中的对象。
> 3. 定义了allocate_memory函数，用于分配内存。
> 4. 定义了make_an_array函数，用于将链表转换为数组。
> 5. 定义了ask_all_of_type函数，用于获取部件中所有指定类型的对象。
> 6. 在do_it函数中，首先获取当前显示部件，然后获取部件中所有组件、过滤器、组件集和组件过滤器的名称，并输出。
> 7. 在ufusr函数中，初始化NX，调用do_it函数，然后终止NX。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 通过以上步骤，该代码实现了遍历部件中所有组件过滤器，并报告其中包含的组件名称的功能。
>
