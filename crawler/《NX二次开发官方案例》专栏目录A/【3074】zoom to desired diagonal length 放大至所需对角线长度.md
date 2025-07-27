### 【3074】zoom to desired diagonal length 放大至所需对角线长度

#### 代码

```cpp
    /*HEAD ZOOM_TO_DESIRED_DIAGONAL_LENGTH CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的函数。因此，翻译为：

注：UF_print_syslog是V18版本新增的函数。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            minimum_distance,  
            vw_scale,  
            vw_ctr[3],  
            desired_vw_diag = 10.0,  
            pt1[3],  
            pt2[3],  
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
        while (prompt_for_a_number("Enter desired view diagonal length ",  
            "View Diagonal", &desired_vw_diag))  
        {  
        #ifdef _WIN32  
            UF_CALL(UF_call_grip("z:\\grip\\view_extents.grx", 1, &arg));  
        #else  
            UF_CALL(UF_call_grip("./view_extents.grx", 1, &arg));  
        #endif  
            UF_CALL(UF_CURVE_ask_point_data(points[0], lower_left));  
            UF_CALL(UF_CURVE_ask_point_data(points[1], upper_right));  
            if (!UF_CALL(UF_MODL_ask_minimum_dist(points[0], points[1], FALSE,  
                NULL, FALSE, NULL, &minimum_distance, pt1, pt2)))  
            {  
                WRITE_F(minimum_distance);  
            }  
        /*  The UNDO puts the WCS back where it was and deletes the GRIP points 里海译:UNDO操作将WCS恢复到原来的位置并删除GRIP点。 */  
            UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            report_double_array("lower left corner", lower_left, 1, 3);  
            report_double_array("upper right corner", upper_right, 1, 3);  
            uc6430("", vw_ctr, &vw_scale);  
            vw_scale = vw_scale / desired_vw_diag * minimum_distance;  
            uc6431("",vw_ctr, vw_scale);  
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

> 这段代码主要用于实现NX中的视图缩放到期望的对角线长度功能。主要功能包括：
>
> 1. 使用GRIP程序在当前视图的左下角和右上角创建两个点，以确定当前视图的范围。
> 2. 计算这两个点之间的最小距离，即当前视图的对角线长度。
> 3. 提示用户输入期望的视图对角线长度，然后根据当前视图的中心点，将视图缩放到用户指定的对角线长度。
> 4. 在整个过程中使用UNDO功能，以便随时撤销操作。
> 5. 使用错误报告函数监控NX API调用，并使用提示对话框获取用户输入。
> 6. 定义了多个辅助函数，用于执行具体的功能，如设置WCS到视图、写入列表窗口、报告数组等。
>
> 总体来说，这段代码实现了视图缩放的核心功能，并提供了良好的用户交互和错误处理。
>
