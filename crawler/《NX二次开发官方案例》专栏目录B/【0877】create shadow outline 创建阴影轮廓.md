### 【0877】create shadow outline 创建阴影轮廓

#### 代码

```cpp
    /*HEAD CREATE_SHADOW_OUTLINE CCC UFUN */  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            *loop_counts,  
            n_loops,  
            n_solids;  
        tag_t  
            **curves,  
            part = UF_PART_ask_display_part(),  
            *solids,  
            work_view,  
            work_part = UF_ASSEM_ask_work_part();  
        double  
            tols[2];  
    /*  UF_CURVE_create_shadow_outline cannot be used in context 译:UF_CURVE_create_shadow_outline 无法在当前上下文中使用 */  
        if (work_part != part)  
            UF_CALL(UF_ASSEM_set_work_part(part));  
        UF_MODL_ask_distance_tolerance(&tols[0]);  
        UF_MODL_ask_angle_tolerance(&tols[1]);  
        n_solids = ask_all_visible_solids(&solids);  
        if (n_solids > 0)  
        {  
            UF_CALL(UF_VIEW_ask_work_view(&work_view));  
            set_undo_mark("Create Shadow Outlines");  
            if (!UF_CALL(UF_CURVE_create_shadow_outline(n_solids, solids,  
                work_view, &n_loops, &loop_counts, &curves, tols)) &&  
                (n_loops > 0))  
            {  
                for (ii = 0; ii < n_solids; ii++)  
                    UF_CALL(UF_OBJ_set_blank_status(solids[ii], UF_OBJ_BLANKED));  
                for (ii = 0; ii < n_loops; ii++)  
                {  
                    for (jj = 0; jj < loop_counts[ii]; jj++)  
                    {  
                        UF_DISP_set_highlight(curves[ii][jj], TRUE);  
                    }  
                }  
                uc1601("Shadow outlines highlighted", TRUE);  
                for (ii = 0; ii < n_loops; ii++)  
                {  
                    for (jj = 0; jj < loop_counts[ii]; jj++)  
                        UF_DISP_set_highlight(curves[ii][jj], FALSE);  
                    UF_free(curves[ii]);  
                }  
                UF_free(curves);  
                UF_free(loop_counts);  
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

> 这段NX二次开发代码的功能如下：
>
> 1. 遍历当前工作视图下所有可见的实体，将它们的tag放入一个数组中。
> 2. 对每个实体调用UF_CURVE_create_shadow_outline函数，生成其轮廓线。轮廓线以环的形式返回，每个环包含多个轮廓线段。
> 3. 将每个实体的轮廓线高亮显示，然后取消高亮。同时将实体设置为不可见，然后再设置为可见。
> 4. 使用UF_UNDO_set_mark函数设置撤销标记，以便可以撤销操作。
> 5. 释放轮廓线数组、环数量数组、轮廓线段数组和实体tag数组占用的内存。
> 6. 通过ufusr函数在NX启动时自动执行，同时提供ufusr_ask_unload函数用于卸载。
>
> 这段代码的主要功能是生成当前视图下所有可见实体的轮廓线并高亮显示，以辅助检查模型轮廓。通过设置撤销标记，可以方便地撤销操作。
>
