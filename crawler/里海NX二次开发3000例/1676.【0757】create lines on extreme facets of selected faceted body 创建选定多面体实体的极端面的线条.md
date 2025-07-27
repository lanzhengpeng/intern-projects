### 【0757】create lines on extreme facets of selected faceted body 创建选定多面体实体的极端面的线条

#### 代码

```cpp
    /*HEAD CREATE_LINES_ON_EXTREME_FACETS_OF_SELECTED_FACETED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_facet.h>  
    #include <uf_curve.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_faceted_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_faceted_model_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_faceted_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select faceted body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faceted_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void draw_facet(tag_t model, int index)  
    {  
        int  
            jj,  
            n_verts;  
        tag_t  
            line;  
        double  
            (*verts)[3];  
        UF_CURVE_line_t  
            line_data;  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        UF_CALL(allocate_memory(n_verts * 3 * sizeof(double), (void *)&verts));  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, index, &n_verts, verts));  
        for (jj = 0; jj < (n_verts - 1); jj++)  
        {  
            UF_VEC3_copy(verts[jj], line_data.start_point);  
            UF_VEC3_copy(verts[jj+1], line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &line));  
        }  
        UF_free(verts);  
    }  
    static void create_lines_on_extreme_facets(tag_t model, tag_t csys)  
    {  
        logical  
            first = TRUE;  
        int  
            ii,  
            facet_id = UF_FACET_NULL_FACET_ID,  
            max_verts,  
            n;  
        double  
            maxs[3],  
            mins[3],  
            point[3],  
            *verts;  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &max_verts));  
        UF_CALL(allocate_memory(max_verts * 3 * sizeof(double), (void **)&verts));  
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet_id)) &&  
            (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet_id, &n,  
                (double (*)[3]) verts));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(&verts[ii*3], point);  
                map_abs2csys(csys, point);  
                if (first || ( point[0] > maxs[0] )) maxs[0] = point[0];  
                if (first || ( point[0] < mins[0] )) mins[0] = point[0];  
                if (first || ( point[1] > maxs[1] )) maxs[1] = point[1];  
                if (first || ( point[1] < mins[1] )) mins[1] = point[1];  
                if (first || ( point[2] > maxs[2] )) maxs[2] = point[2];  
                if (first || ( point[2] < mins[2] )) mins[2] = point[2];  
                first = FALSE;  
            }  
        }  
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet_id)) &&  
            (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet_id, &n,  
                (double (*)[3]) verts));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(&verts[ii*3], point);  
                map_abs2csys(csys, point);  
                if (( point[0] == maxs[0] ) ||  
                    ( point[0] == mins[0] ) ||  
                    ( point[1] == maxs[1] ) ||  
                    ( point[1] == mins[1] ) ||  
                    ( point[2] == maxs[2] ) ||  
                    ( point[2] == mins[2] ))  
                {  
                    draw_facet(model, facet_id);  
                    break;  
                }  
            }  
        }  
        UF_free(verts);  
        return;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            model,  
            wcs;  
        while ((model = select_a_faceted_body("Create extreme facets")) != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_ask_wcs(&wcs));  
            create_lines_on_extreme_facets(model, wcs);  
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

> 这段NX二次开发代码的主要功能是在NX中选择一个实体模型，然后在该模型的最外侧面上创建线条。具体步骤如下：
>
> 1. 定义了错误报告函数report_error，用于处理UF函数调用失败的情况。
> 2. 定义了mask_for_faceted_bodies函数，用于在NX中选择时过滤出实体模型。
> 3. 定义了select_a_faceted_body函数，用于让用户选择一个实体模型，并返回模型的tag。
> 4. 定义了map_abs2csys函数，用于将绝对坐标系下的点转换到指定的坐标系下。
> 5. 定义了allocate_memory函数，用于分配内存。
> 6. 定义了draw_facet函数，用于根据顶点创建线条。
> 7. 定义了create_lines_on_extreme_facets函数，用于遍历模型的所有面，找到最外侧的面，并在其上创建线条。
> 8. 定义了do_it函数，用于让用户选择模型，然后调用create_lines_on_extreme_facets函数。
> 9. 定义了ufusr函数，是NX二次开发的入口函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 10. 定义了ufusr_ask_unload函数，用于处理NX卸载二次开发模块。
>
> 总的来说，这段代码通过二次开发的方式，实现了在NX中选择实体模型，并绘制其最外侧面的线条的功能。
>
