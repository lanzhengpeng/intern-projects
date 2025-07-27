### 【2282】report perimeter and area of selected sheet body 报告所选钣金件的周长和面积

#### 代码

```cpp
    /*HEAD REPORT_PERIMETER_AND_AREA_OF_SELECTED_SHEET_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a sheet", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        tag_t  
            sheet;  
        double  
            area,  
            perimeter;  
        while ((sheet = select_a_sheet("Report perimeter and area")) != NULL_TAG)  
        {  
            perimeter = ask_perimeter_of_sheet(sheet);  
            WRITE_F(perimeter);  
            area = ask_surface_area_of_sheet(sheet);  
            WRITE_F(area);  
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

> 这段代码主要用于在NX中实现选择一个薄片实体，并报告其周长和面积的功能。
>
> 主要功能包括：
>
> 1. 错误报告：使用宏UF_CALL和函数report_error实现错误报告功能。
> 2. 选择薄片实体：通过函数select_a_sheet实现选择薄片实体的功能。
> 3. 周长计算：通过函数ask_body_edges、ask_perimeter_of_sheet等计算薄片实体的周长。
> 4. 面积计算：通过函数ask_surface_area_of_sheet计算薄片实体的面积。
> 5. 结果输出：使用函数WRITE_F将计算结果输出到列表窗口。
> 6. 内存管理：使用函数allocate_memory、UF_free进行内存管理。
> 7. 主函数：在函数do_it中实现主要逻辑，循环选择薄片实体并报告周长和面积。
> 8. 初始化和终止：在函数ufusr中进行初始化和终止调用。
> 9. 卸载处理：在函数ufusr_ask_unload中处理卸载逻辑。
>
> 该代码实现了在NX中选择薄片实体并计算其周长和面积的功能，通过错误报告、内存管理等实现稳定运行。
>
