### 【2224】report new parts after save as 在保存后报告新增零件

#### 代码

```cpp
    /*HEAD REPORT_NEW_PARTS_AFTER_SAVE_AS CCC UFUN */  
    /*  This program demonstrates how to capture any new part names generated  
        during a Save As operation.  It is designed to be installed as both a PRE  
        and POST action on the File-> Save As...  
        This is the corresponding menufile:  
        VERSION 120  
        EDIT UG_GATEWAY_MAIN_MENUBAR  
        MODIFY  
            BUTTON UG_FILE_SAVE_AS  
            ACTIONS/PRE report_new_parts_after_save_as  
            ACTIONS/POST report_new_parts_after_save_as  
        END_OF_MODIFY  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，主要用于打印系统日志。 */  
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
    static int n_parts = 0;  
    static tag_t *parts = NULL;  
    static char **names = NULL;  
    static void add_parents_to_list(tag_t component, uf_list_p_t parent_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        n = UF_ASSEM_where_is_part_used(component, &parents);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_put_list_item(parent_list, parents[ii]));  
            add_parents_to_list(parents[ii], parent_list);  
        }  
        if (n) UF_free(parents);  
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
    static int ask_where_used_in_session(tag_t part, tag_t **using_parts)  
    {  
        uf_list_p_t  
            p_list;  
        UF_CALL(UF_MODL_create_list(&p_list));  
        UF_CALL(UF_MODL_put_list_item(p_list, part));  
        add_parents_to_list(part, p_list);  
        return make_an_array(&p_list, using_parts);  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        logical  
            tceng;  
        int  
            ii;  
        tag_t  
            work = UF_ASSEM_ask_work_part();  
        char  
            name[MAX_FSPEC_SIZE + 1];  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (n_parts == 0)  
        {  
            n_parts = ask_where_used_in_session(work, &parts);  
            UF_CALL(allocate_string_array(n_parts, MAX_FSPEC_SIZE+1, &names));  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                UF_PART_ask_part_name(parts[ii], names[ii]);  
                if (tceng) UF_PART_name_for_display(names[ii], names[ii]);  
            }  
        }  
        else  
        {  
        /*  Clear the listing window 里海译:清除列表窗口 */  
            UF_CALL(UF_UI_exit_listing_window());  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                UF_PART_ask_part_name(parts[ii], name);  
                if (tceng) UF_PART_name_for_display(name, name);  
                if (strcmp(name, names[ii]))  
                {  
                    WRITE(names[ii]);  
                    WRITE(" renamed to ");  
                    WRITE(name);  
                    WRITE("\n");  
                }  
            }  
            UF_free(parts);  
            UF_free_string_array(n_parts, names);  
            n_parts = 0;  
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
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，主要功能是在保存NX文件时报告任何新生成的部件名称。代码在保存文件前后被调用，具体实现步骤如下：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了一个递归函数add_parents_to_list，用于获取组件在装配中的所有上级组件，并将它们添加到列表中。
> 3. 定义了两个内存分配函数allocate_memory和allocate_string_array，用于分配内存。
> 4. 定义了一个函数make_an_array，用于将对象列表转换成对象数组。
> 5. 定义了一个函数ask_where_used_in_session，用于查询组件在当前会话中的使用情况，并返回使用该组件的所有组件数组。
> 6. 在主函数do_it中，首先判断是否是首次调用，如果是，则查询当前工作部件的所有上级组件，并保存它们的名称；如果不是，则对比保存的名称和当前名称，打印出变化的名称。
> 7. 在ufusr函数中，初始化NX Open，调用do_it函数，然后终止NX Open。
> 8. 定义了一个询问卸载函数ufusr_ask_unload，返回卸载标志。
>
> 总体来说，该代码通过保存和比较部件名称，实现了在保存文件时检测和报告任何新生成的部件名称的功能。
>
