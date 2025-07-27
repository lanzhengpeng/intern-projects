### 【1367】highlight all midsurface sheets 高亮所有中面片

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_MIDSURFACE_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t ask_next_midsurface_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        char  
            *f_type;  
        uf_list_p_t  
            feat_list,  
            tmp;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY)  
                {  
                    UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
                    for (tmp = feat_list; tmp != NULL; tmp = tmp->next)  
                    {  
                        UF_CALL(UF_MODL_ask_feat_type(tmp->eid, &f_type));  
                        if (!strcmp(f_type, "MIDSURFACE"))  
                        {  
                            UF_free(f_type);  
                            UF_CALL(UF_MODL_delete_list(&feat_list));  
                            return body;  
                        }  
                        UF_free(f_type);  
                    }  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                }  
            }  
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
    static int ask_all_midsurface_sheets(tag_t part, tag_t **sheets)  
    {  
        tag_t  
            sheet = NULL_TAG;  
        uf_list_p_t  
            sheet_list;  
        UF_CALL(UF_MODL_create_list(&sheet_list));  
        while ((sheet = ask_next_midsurface_sheet_body(part, sheet)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(sheet_list, sheet));  
        return (make_an_array(&sheet_list, sheets));  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *midsurfaces;  
        char  
            msg[133];  
        if ((n = ask_all_midsurface_sheets(part, &midsurfaces)) > 0)  
        {  
            set_highlight_object_array(n, midsurfaces, TRUE);  
            sprintf(msg, "%d midsurfaces highlighted", n);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n, midsurfaces, FALSE);  
        }  
        else  
            uc1601("No midsurfaces in part.", TRUE);  
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

> 这段代码是NX的二次开发代码，其主要功能是高亮显示零件中的所有中面片(sheet)特征。
>
> 代码的主要流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败的信息。
> 2. 定义了一个函数ask_next_midsurface_sheet_body，用于遍历零件中的实体，并返回下一个中面片(sheet)体。
> 3. 定义了内存分配函数allocate_memory和创建数组函数make_an_array。
> 4. 定义了函数ask_all_midsurface_sheets，用于获取零件中的所有中面片(sheet)体，并将它们放入数组。
> 5. 定义了函数set_highlight_object_array，用于设置数组中对象的高亮显示。
> 6. 在do_it函数中，获取当前显示的零件，调用ask_all_midsurface_sheets获取中面片(sheet)数组，然后高亮显示它们，最后取消高亮。
> 7. ufusr是二次开发的入口函数，调用UF_initialize初始化NX环境，调用do_it执行功能，最后调用UF_terminate结束NX环境。
> 8. ufusr_ask_unload用于卸载二次开发，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 综上所述，这段代码的主要功能是遍历零件中的实体，找到所有中面片(sheet)特征，并高亮显示它们。
>
