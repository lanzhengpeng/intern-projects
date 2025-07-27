### 【0574】create and edit bsurface 创建并编辑B曲面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t ask_body_first_face(tag_t body)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(n,X) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void report_face_bsurf_data(tag_t face, logical terse)  
    {  
        logical  
            untrimmed;  
        UF_MODL_bsurface_t  
            bsurf;  
        if (UF_CALL(UF_MODL_ask_bsurf(face, &bsurf))) return;  
        WRITE_D(bsurf.num_poles_u);  
        WRITE_D(bsurf.num_poles_v);  
        WRITE_D(bsurf.order_u);  
        WRITE_D(bsurf.order_v);  
        WRITE_L(bsurf.is_rational);  
        if (!terse)  
        {  
            WRITEnF(bsurf.num_poles_u + bsurf.order_u, bsurf.knots_u);  
            WRITEnF(bsurf.num_poles_v + bsurf.order_v, bsurf.knots_v);  
            WRITEnF4(bsurf.num_poles_u * bsurf.num_poles_v, (double *)bsurf.poles);  
        }  
        UF_CALL(UF_MODL_free_bsurf_data(&bsurf));  
        untrimmed = uf5411(&face);  
        WRITE_L(untrimmed);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        double  
            points[3][9] = {{0, 0, 0,  1, 0, 0,  2, 0, 0},  
                            {0, 1, 0,  1, 1, 0,  2, 1, 0},  
                            {0, 2, 0,  1, 2, 0,  2, 2, 0} },  
            weight[3][3] = {  {1, 1, 1}, {1, 1, 1},{1, 1, 1} };  
        UF_MODL_bsurface_t  
            bsurf;  
        tag_t  
            bsurf_face,  
            bsurf_sheet;  
        UF_MODL_bsurf_row_info_t  
            pts_info_per_row[3];  
    /*  load point and other information for each row of points 译:每一行的点加载点和其它信息 */  
        for (ii=0; ii<3; ii++)  
        {  
            pts_info_per_row[ii].num_points = 3;  
            pts_info_per_row[ii].points = points[ii];  
            pts_info_per_row[ii].weight = weight[ii];  
        }  
    /* create B-surface 译:创建B曲面 */  
        UF_CALL(UF_MODL_create_bsurf_thru_pts(3, 0, 0, 2, 2, 3, pts_info_per_row,  
            &bsurf_sheet));  
        bsurf_face = ask_body_first_face(bsurf_sheet);  
        ECHO("Before editing:\n");  
        report_face_bsurf_data(bsurf_face, FALSE);  
    /* ask the B-surface information 译:询问B曲面信息 */  
        UF_CALL(UF_MODL_ask_bsurf(bsurf_face, &bsurf));  
    /* Lift-up the mid-point in z 译:提升z方向中点。 */  
        bsurf.poles[4][2] += 1;  
    /* Edit the surface and update. 译:编辑表面并更新。 */  
        UF_CALL(UF_MODL_edit_bsurf(bsurf_face, &bsurf));  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_MODL_free_bsurf_data(&bsurf));  
        ECHO("After editing:\n");  
        report_face_bsurf_data(bsurf_face, FALSE);  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 定义了一个宏UF_CALL，用于执行NX的UF函数，并报告错误信息。
> 2. 定义了一个ask_body_first_face函数，用于获取一个体的第一个面。
> 3. 定义了多个WRITE_XXX宏，用于将变量输出到日志窗口。
> 4. 定义了一个write_double_array_to_listing_window函数，用于将二维数组输出到日志窗口。
> 5. 定义了一个report_face_bsurf_data函数，用于报告面的B曲面数据。
> 6. 定义了一个do_it函数，用于实现主要功能：创建一个3x3的B曲面网格输出创建的B曲面面的数据获取B曲面数据，修改z方向中点的值更新B曲面，并输出更新后的数据
> 7. 创建一个3x3的B曲面网格
> 8. 输出创建的B曲面面的数据
> 9. 获取B曲面数据，修改z方向中点的值
> 10. 更新B曲面，并输出更新后的数据
> 11. 定义了ufusr函数，作为NX二次开发的入口函数，调用do_it函数。
> 12. 定义了ufusr_ask_unload函数，用于卸载二次开发。
>
> 总体来说，这段代码实现了通过NX API创建B曲面，修改B曲面，并输出B曲面数据的功能。
>
