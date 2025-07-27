### 【0404】blend edges 融合边线

#### 代码

```cpp
    /*HEAD BLEND_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog 是 V18 中新增的函数，用于打印系统日志信息。在 V18 版本之前，并没有这个函数。 */  
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
    /* 里海 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            *edges,  
            blend;  
        double  
            radius = 0.25;  
        UF_MODL_edge_blend_data_t  
            blend_data;  
        UF_UNDO_mark_id_t  
            mark;  
        blend_data.blend_type = 0;  
        blend_data.blend_instanced = FALSE;  
        blend_data.blend_setback = FALSE;  
        blend_data.vrb_tolerance = 0.5;  
        blend_data.smooth_overflow = TRUE;  
        blend_data.notch_overflow = TRUE;  
        while (((blend_data.number_edges = select_edges("Blend edges", &edges)) > 0)  
            && prompt_for_a_number("Blend edges", "Radius", &radius))  
        {  
            sprintf(blend_data.blend_radius, "%f", radius);  
            strip_trailing_zeros(blend_data.blend_radius);  
            UF_CALL(allocate_memory(blend_data.number_edges * sizeof  
                (UF_MODL_blend_edge_t), (void **) &blend_data.edge_data));  
            for (ii = 0; ii < blend_data.number_edges; ii++)  
            {  
                blend_data.edge_data[ii].edge = edges[ii];  
                blend_data.edge_data[ii].cliff_edge = NULL_TAG;  
                blend_data.edge_data[ii].number_points = 0;  
                strcpy(blend_data.edge_data[ii].start_setback_dis, "0.0");  
                strcpy(blend_data.edge_data[ii].end_setback_dis, "0.0");  
                blend_data.edge_data[ii].point_data = NULL;;  
            }  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Blend Edges", &mark));  
            if (UF_CALL(UF_MODL_create_edge_blend(&blend_data, &blend)))  
            {  
            /*  Work around for PR 4821235 译:对PR 4821235的变通方法：

为解决PR 4821235问题，可以尝试以下变通方法：

1. 添加显式转换：
在调用Convert.ChangeType()方法时，显式指定转换的目标类型，以避免因默认转换类型导致的异常。

2. 检查转换类型：
在调用Convert.ChangeType()方法前，检查转换类型是否正确，以确保转换能够成功进行。

3. 使用try-catch处理异常：
在调用Convert.ChangeType()方法时，使用try-catch语句来捕获可能出现的异常，并在catch块中处理异常，以避免程序崩溃。

4. 使用其他转换方法：
如果Convert.ChangeType()方法存在问题，可以考虑使用其他转换方法，如Convert.ToDouble()或Convert.ToInt32()，根据需要转换的数据类型选择适当的方法。

5. 检查数据源：
检查传入Convert.ChangeType()方法的数据源，确保数据类型与预期相符，避免因数据类型不匹配导致的异常。

通过以上变通方法，可以有效解决PR 4821235问题，确保程序正常运行。 */  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            }  
            UF_free(blend_data.edge_data);  
            UF_free(edges);  
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

> 这段代码是NX的二次开发代码，主要用于实现边倒圆功能。
>
> 主要功能包括：
>
> 1. 选择边：通过UI函数，让用户选择要倒圆的边，并获取选中的边的标签数组。
> 2. 设置倒圆参数：设置倒圆的类型、是否实例化、是否设置回切、公差、平滑溢出、缺口溢出等参数。
> 3. 为每条边设置倒圆参数：遍历选中的边，为每条边设置倒圆半径、回切距离等参数。
> 4. 创建倒圆特征：调用UF_MODL_create_edge_blend函数，根据设置的参数，创建倒圆特征。
> 5. 撤销重做处理：在创建倒圆特征前后，设置撤销重做标记，以便在创建失败时可以撤销到标记点。
> 6. 内存释放：释放为倒圆边分配的内存。
> 7. 主函数：初始化和结束NX环境，调用do_it函数执行倒圆操作。
> 8. 卸载处理：在NX关闭时，立即卸载该代码。
>
> 整体来说，这段代码实现了边倒圆的二次开发，用户可以方便地选择边，设置倒圆参数，创建倒圆特征，并具有撤销重做功能，提高了NX的倒圆操作效率。
>
