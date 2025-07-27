### 【1332】get current view extents 获取当前视图范围

#### 代码

```cpp
    /*HEAD GET_CURRENT_VIEW_EXTENTS CCC UFUN */  
    /* This program demonstrates how to determine the current view's extents  
       taking into account any zooming or panning the user may have done.  A  
       GRIP program is used to create points at the current view's lower left  
       and upper right corners.  
       This is the GRIP code used:  
            entity/ pt(2), tpt(3), tln(3)  
            ufargs/ pt  
            tpt(1) = point/0,0,0  
            tln(1) = line/ tpt(1), atangl, 90  
            tpt(2) = point/ ENDOF, YSMALL, tln(1)  
            tln(2) = line/ tpt(2), atangl, 0  
            tpt(3) = point/ ENDOF, XLARGE, tln(2)  
            tln(3) = line/ tpt(3), atangl, 90  
            pt(1) = point/ ENDOF, xsmall, tln(2)  
            pt(2) = point/ ENDOF, ylarge, tln(3)  
            delete/ tpt,tln  
            halt  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void set_wcs_to_view(char *view_name)  
    {  
        tag_t  
            matobj,  
            csys;  
        double  
            origin[3],  
            matrix[9],  
            scale;  
        UF_CALL(uc6430(view_name, origin, &scale));  
        UF_CALL(uc6433(view_name, matrix));  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            points[2];  
        double  
            lower_left[3],  
            upper_right[3];  
        UF_args_t  
            arg;  
        UF_UNDO_mark_id_t  
            mark;  
        arg.type    = UF_TYPE_TAG_T_ARRAY;  
        arg.length  = 2;  
        arg.address = points;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        set_wcs_to_view("");  
    #ifdef _WIN32  
        UF_CALL(UF_call_grip("n:\\grip\\view_extents.grx", 1, &arg));  
    #else  
        UF_CALL(UF_call_grip("grip/view_extents.grx", 1, &arg));  
    #endif  
        UF_CALL(UF_CURVE_ask_point_data(points[0], lower_left));  
        UF_CALL(UF_CURVE_ask_point_data(points[1], upper_right));  
        uc1601("See the points?", TRUE);  
    /*  The UNDO puts the WCS back where it was and deletes the GRIP points 里海译:UNDO 将 WCS 恢复到之前的位置并删除 GRIP 点。 */  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
        report_double_array("lower left corner", lower_left, 1, 3);  
        report_double_array("upper right corner", upper_right, 1, 3);  
    }  
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

> 这是段NX二次开发代码，主要功能是获取当前视图的显示范围。代码的主要步骤包括：
>
> 1. 定义了一个报告错误函数，用于在调用UF函数出错时打印错误信息。
> 2. 定义了一个报告数组函数，用于打印输出double数组的内容。
> 3. 定义了一个设置WCS到视图函数，根据视图名称获取视图的变换矩阵，并设置WCS到该视图。
> 4. 在主函数do_it中，首先获取当前视图的显示范围，通过调用一个GRIP程序来实现。GRIP程序会在视图的左下角和右上角创建两个点。
> 5. 获取这两个点的坐标，即为当前视图的显示范围。
> 6. 使用UNDO恢复WCS，并删除GRIP创建的点。
> 7. 在ufusr函数中调用do_it函数，并初始化和终止NX。
> 8. 定义了卸载函数，使NX可以立即卸载该程序。
>
> 总体而言，该代码实现了获取当前视图的显示范围，并使用GRIP程序来简化了实现。
>
