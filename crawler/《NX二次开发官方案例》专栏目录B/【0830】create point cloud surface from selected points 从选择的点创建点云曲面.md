### 【0830】create point cloud surface from selected points 从选择的点创建点云曲面

#### 代码

```cpp
    /*HEAD CREATE_POINT_CLOUD_SURFACE_FROM_SELECTED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_points(char *prompt, tag_t **points)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select points", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*points)[ii], FALSE));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            max_err_index,  
            n_pts;  
        tag_t  
            *pt_tags,  
            surface;  
        double  
            avg_err,  
            max_err,  
            (*points)[3];  
        if ((n_pts = select_points("Surface From Point Cloud", &pt_tags)) > 0)  
        {  
            UF_CALL(allocate_memory(n_pts * sizeof(double[3]), (void **)&points));  
            for (ii = 0; ii < n_pts; ii++)  
                UF_CALL(UF_CURVE_ask_point_data(pt_tags[ii], points[ii]));  
            UF_CALL(UF_MODL_create_surf_from_cloud(n_pts, points, NULL, NULL, 3,  
                3, 1, 1, 0, &avg_err, &max_err, &max_err_index, &surface));  
            UF_free(points);  
            UF_free(pt_tags);  
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

> 这段代码是NX的二次开发代码，其主要功能是创建一个曲面，从用户选择的一系列点云中。
>
> 代码的主要流程如下：
>
> 1. 错误处理函数：定义了一个错误处理函数report_error，用于打印错误代码和错误消息。
> 2. 选择点：定义了一个选择点的函数select_points，它使用NX的UI接口，提示用户选择点，并返回点的标签数组。
> 3. 内存分配：定义了一个内存分配函数allocate_memory，用于申请内存空间。
> 4. 创建曲面：定义了do_it函数，它是主函数，先调用select_points选择点，然后调用allocate_memory分配内存，获取点的坐标数据，最后调用NX的UF_MODL_create_surf_from_cloud函数，根据点云数据创建曲面。
> 5. 主函数：定义了ufusr主函数，初始化NX环境后调用do_it函数，然后终止NX环境。
> 6. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来看，这段代码实现了从用户选择的点云数据中创建曲面的功能，是典型的NX二次开发代码。
>
