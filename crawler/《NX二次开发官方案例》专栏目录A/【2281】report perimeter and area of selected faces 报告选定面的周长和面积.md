### 【2281】report perimeter and area of selected faces 报告选定面的周长和面积

#### 代码

```cpp
    /*HEAD REPORT_PERIMETER_AND_AREA_OF_SELECTED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 新增的，只回答译文，不要废话。 */  
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
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int ask_body_edges(tag_t body, tag_p_t *body_edges)  
    {  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        return make_an_array(&edge_list, body_edges);  
    }  
    static double ask_perimeter_of_sheet(tag_t sheet)  
    {  
        int  
            ii,  
            n_edges;  
        tag_t  
            *edges;  
        double  
            edge_length,  
            total_length = 0;  
        n_edges = ask_body_edges(sheet, &edges);  
        for (ii = 0; ii < n_edges; ii++)  
        {  
            if (!UF_CALL(UF_CURVE_ask_arc_length(edges[ii], 0, 1,  
                UF_MODL_UNITS_PART, &edge_length)))  
                total_length = total_length + edge_length;  
        }  
        if (n_edges > 0) UF_free(edges);  
        return total_length;  
    }  
    static double ask_surface_area_of_sheet(tag_t sheet)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            parameter[11] = { 0.99, 1,2,3,4,5,6,7,8,9,10 },  
            results[47],  
            statistics[13];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        units = 5-(2*units); /*  UF_PART_METRIC  -> 3 = Grams and centimeters  
                                 UF_PART_ENGLISH -> 1 = Pounds and inches */  
        UF_CALL(UF_MODL_ask_mass_props_3d(&sheet, 1, 2, units, 1.0, 1, parameter,  
            results, statistics));  
        return results[0];  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_faces;  
        tag_t  
            *faces,  
            sheet;  
        double  
            area,  
            perimeter;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL_TAG, &mark));  
        while ((n_faces = select_faces("Area Analysis", &faces)) > 0)  
        {  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            area = 0;  
            perimeter = 0;  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                UF_CALL(UF_MODL_extract_face(faces[ii], 0, &sheet));  
                perimeter = perimeter + ask_perimeter_of_sheet(sheet);  
                area = area + ask_surface_area_of_sheet(sheet);  
            }  
            if (n_faces > 0) UF_free(faces);  
            WRITE_F(perimeter);  
            WRITE_F(area);  
            UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
        }  
        UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
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

> 这段NX二次开发代码的主要功能是计算和报告所选面的周长和面积。
>
> 代码流程如下：
>
> 1. 使用UF_MODL_extract_face提取选中面的实体，然后计算其周长和面积。
> 2. 周长通过遍历实体上的边并求和各边长度来计算。
> 3. 面积通过调用UF_MODL_ask_mass_props_3d来计算。
> 4. 使用UF_UI_set_sel_mask设置选择掩码，只选择实体面。
> 5. 通过UF_UI_select_with_class_dialog弹出对话框让用户选择面。
> 6. 使用UF_DISP_set_highlight高亮显示选中面。
> 7. 使用UF_UNDO进行撤销操作，以避免误操作。
> 8. 使用UF_UI_open_listing_window和UF_UI_write_listing_window在列表窗口中输出结果。
> 9. 使用UF_free释放内存。
> 10. 使用UF_initialize和UF_terminate初始化和终止NX API。
>
> 该代码通过用户选择的面，计算其周长和面积，并在列表窗口中报告结果。
>
