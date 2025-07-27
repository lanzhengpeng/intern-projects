### 【3046】validate all solids Validate all solids

#### 代码

```cpp
    /*HEAD VALIDATE_ALL_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的功能。 */  
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
    static void ask_all_solids_and_solid_occs(tag_t part, tag_t **solids, int  
        *n_solids, tag_t **solid_occs, int *n_occs)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list,  
            occ_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            if (UF_ASSEM_is_occurrence(body))  
                UF_CALL(UF_MODL_put_list_item(occ_list, body));  
            else  
                UF_CALL(UF_MODL_put_list_item(body_list, body));  
        *n_solids = make_an_array(&body_list, solids);  
        *n_occs = make_an_array(&occ_list, solid_occs);  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0,  
            ii,  
            n_occs,  
            n_solids,  
            valid_flag,  
            *valid_info;  
        tag_t  
            *occs,  
            orig_part,  
            part = UF_PART_ask_display_part(),  
            proto,  
            proto_part,  
            *solids;  
        char  
            msg[133];  
        ask_all_solids_and_solid_occs(part, &solids, &n_solids, &occs, &n_occs);  
        if (n_solids > 0)  
        {  
            UF_CALL(allocate_memory(n_solids * sizeof(int), (void **)&valid_info));  
            UF_CALL(UF_MODL_validate_body(n_solids, solids, valid_info));  
            for (ii = 0; ii < n_solids; ii++)  
            {  
                if (valid_info[ii] == 2)  
                {  
                    cnt++;  
                    UF_CALL(UF_DISP_set_highlight(solids[ii], TRUE));  
                }  
            }  
            UF_free(solids);  
            UF_free(valid_info);  
        }  
        /*  UF_MODL_validate_body does not work on occurrences, so you have to  
            get the prototypes, and set the work part to their owning parts */  
        if (n_occs > 0)  
        {  
            for (ii = 0; ii < n_occs; ii++)  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(occs[ii]);  
                UF_CALL(UF_OBJ_ask_owning_part(proto, &proto_part));  
                UF_CALL(UF_ASSEM_set_work_part_quietly(proto_part, &orig_part));  
                UF_CALL(UF_MODL_validate_body(1, &proto, &valid_flag));  
                UF_CALL(UF_ASSEM_set_work_part_quietly(orig_part, &proto_part));  
                if (valid_flag == 2)  
                {  
                    cnt++;  
                    UF_CALL(UF_DISP_set_highlight(occs[ii], TRUE));  
                }  
            }  
            UF_free(occs);  
        }  
        if (cnt > 0)  
        {  
            sprintf(msg, "%d of %d solids are invalid", cnt, n_solids + n_occs);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
        else  
        {  
            sprintf(msg, "All %d solids are valid", n_solids + n_occs);  
            uc1601(msg, TRUE);  
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

> 这段NX二次开发代码的主要功能是验证当前显示部件中的所有实体和实体实例的有效性，并通过高亮显示无效的实体和实体实例。
>
> 具体步骤如下：
>
> 1. 定义了一些辅助函数，如report_error()用于打印错误信息，ask_next_body()用于遍历部件中的实体，allocate_memory()用于申请内存，make_an_array()用于从链表转换为数组等。
> 2. ask_all_solids_and_solid_occs()函数用于获取当前显示部件中的所有实体和实体实例，分别存储在solids数组和occs数组中。
> 3. do_it()函数是主函数，首先验证所有实体，并将无效实体高亮显示。对于实体实例，需要先获取原型，然后设置工作部件为原型所在部件，再进行验证，同样将无效实体实例高亮显示。
> 4. ufusr()是NX调用的主函数，初始化NX环境，调用do_it()执行功能，然后终止NX环境。
> 5. ufusr_ask_unload()函数表示该用户函数不需要卸载。
>
> 通过这段代码，可以方便地检查当前部件中的实体和实体实例的有效性，并通过高亮显示的方式直观地呈现出来，有助于发现潜在问题。
>
