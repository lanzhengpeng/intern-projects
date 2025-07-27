### 【0876】create shadow curves 创建阴影曲线

#### 代码

```cpp
    /*HEAD CREATE_SHADOW_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_assem.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t ask_next_visible_solid(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &body)) && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                return body;  
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
    static int ask_all_visible_solids(tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_visible_solid(body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    #include <uf_undo.h>  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
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
            ii,  
            n_curves,  
            n_solids;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *shadow_curves,  
            *solids,  
            work_part = UF_ASSEM_ask_work_part(),  
            work_view;  
    /*  UF_CURVE_create_shadow_curves cannot be used in context 译:创建阴影曲线的函数 UF_CURVE_create_shadow_curves 不能在此上下文中使用。 */  
        if (work_part != part)  
            UF_CALL(UF_ASSEM_set_work_part(part));  
        n_solids = ask_all_visible_solids(&solids);  
        if (n_solids > 0)  
        {  
            UF_CALL(UF_VIEW_ask_work_view(&work_view));  
            set_undo_mark("Create Shadow Curves");  
            UF_CALL(UF_CURVE_create_shadow_curves(n_solids, solids,  
                work_view, &n_curves, &shadow_curves));  
            if (n_curves > 0)  
            {  
                for (ii = 0; ii < n_solids; ii++)  
                    UF_CALL(UF_OBJ_set_blank_status(solids[ii], UF_OBJ_BLANKED));  
                for (ii = 0; ii < n_curves; ii++)  
                    UF_CALL(UF_DISP_add_item_to_display(shadow_curves[ii]));  
                set_highlight_object_array(n_curves, shadow_curves, TRUE);  
                uc1601("Shadow curves highlighted", TRUE);  
                set_highlight_object_array(n_curves, shadow_curves, FALSE);  
                UF_free(shadow_curves);  
                for (ii = 0; ii < n_solids; ii++)  
                    UF_CALL(UF_OBJ_set_blank_status(solids[ii],  
                        UF_OBJ_NOT_BLANKED));  
            }  
            UF_free(solids);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发代码，主要功能是创建选定实体的阴影曲线。
>
> 代码的主要流程如下：
>
> 1. 包含了NX API的头文件。
> 2. 定义了错误报告函数report_error，用于输出错误信息。
> 3. 定义了函数ask_next_visible_solid，用于循环获取当前视图下可见的实体。
> 4. 定义了内存分配函数allocate_memory。
> 5. 定义了创建数组函数make_an_array。
> 6. 定义了获取所有可见实体函数ask_all_visible_solids。
> 7. 定义了设置撤销标记函数set_undo_mark。
> 8. 定义了设置高亮显示函数set_highlight_object_array。
> 9. 定义了主要函数do_it，该函数执行以下步骤：获取当前显示的部件。获取当前工作部件和工作视图。获取所有可见实体，并创建它们的阴影曲线。将实体的显示状态设置为隐藏。将阴影曲线添加到显示列表。高亮显示阴影曲线。取消高亮显示。恢复实体的显示状态。
> 10. 获取当前显示的部件。
> 11. 获取当前工作部件和工作视图。
> 12. 获取所有可见实体，并创建它们的阴影曲线。
> 13. 将实体的显示状态设置为隐藏。
> 14. 将阴影曲线添加到显示列表。
> 15. 高亮显示阴影曲线。
> 16. 取消高亮显示。
> 17. 恢复实体的显示状态。
> 18. 定义了ufusr函数，作为二次开发的入口函数，调用do_it函数执行操作。
> 19. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了在当前视图下为所有可见实体创建阴影曲线的功能，并支持撤销操作和高亮显示阴影曲线。
>
