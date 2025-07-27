### 【1355】group all solids 将所有实体组合成一个组

#### 代码

```cpp
    /*HEAD GROUP_ALL_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_group.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本新增的功能，用于打印系统日志。

翻译为：

UF_print_syslog 是在 V18 版本中新增的，用于打印系统日志的功能。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_solids;  
        tag_t  
            group,  
            part = UF_PART_ask_display_part(),  
            *solids;  
        char  
            msg[MAX_LINE_SIZE+1];  
        if ((n_solids = ask_all_solids(part, &solids)) > 0)  
        {  
            for (ii = 0; ii < n_solids; ii++)  
                UF_CALL(UF_GROUP_del_member_from_group(solids[ii], NULL_TAG));  
            if (!UF_CALL(UF_GROUP_create_group(solids, n_solids, &group)))  
            {  
                set_highlight_object_array(n_solids, solids, TRUE);  
                sprintf(msg, "Grouped %d solids", n_solids);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_solids, solids, FALSE);  
            }  
            UF_free(solids);  
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

> 这段代码是NX的二次开发代码，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 2. 定义了一个辅助函数ask_next_body，用于遍历部件中的实体，并返回下一个固体。
> 3. 定义了内存分配函数allocate_memory。
> 4. 定义了创建数组函数make_an_array，用于将链表转换为数组。
> 5. 定义了获取所有固体函数ask_all_solids，用于获取部件中的所有固体。
> 6. 定义了设置高亮函数set_highlight_object_array，用于设置多个对象的高亮显示。
> 7. 定义了主函数do_it，用于遍历获取所有固体，从组中删除，创建新组，并高亮显示。
> 8. 定义了ufusr函数，作为程序的入口，初始化后调用do_it函数，最后终止。
> 9. 定义了卸载函数ufusr_ask_unload。
>
> 整体来看，这段代码实现了遍历部件中的所有固体，将它们从现有组中删除，创建新的组，并高亮显示的功能。
>
