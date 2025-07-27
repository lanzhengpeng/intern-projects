### 【0914】create smart xform from three points and report xform data 创建三点智能变换并报告变换数据

#### 代码

```cpp
    /*HEAD CREATE_SMART_XFORM_FROM_THREE_POINTS_AND_REPORT_XFORM_DATA CCC UFUN */  
    /*==========================================================  
    As of NX5, the documentation for the Open C function  
    UF_SO_create_xform_three_points does not clearly explain that  
    the three points used represent the origin, the X direction,  
    and the Y direction of the xform object that is created.  
    This example demonstrates that this is the case.  
    ==========================================================*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object_in_part = NULL_TAG,  
            part = NULL_TAG,  
            point0 = NULL_TAG,  
            point1 = NULL_TAG,    
            point2 = NULL_TAG,    
            scale = NULL_TAG,    
            xform1 = NULL_TAG;   
        double  
            scale_double = 2.0,  
            output_scale = -1.,  
            point[3] = { -1., -1., -1. },  
            p0[3] = { 0., 0., 0. },  
            p1[3] = { 0., 100., 0. },  
            p2[3] = { -100., 0., 0. },  
            matrix[16],  
            x_direction [ 3 ] = { -1., -1., -1. },  
            y_direction [ 3 ] = { -1., -1., -1. },  
            z_direction [ 3 ] = { -1., -1., -1. };  
       UF_SO_update_option_t   
            update_option = UF_SO_update_within_modeling;  
        part = UF_PART_ask_display_part();  
        if( NULL_TAG == part ) return;  
        object_in_part = UF_OBJ_cycle_all( part, object_in_part );  
        UF_CALL( UF_CURVE_create_point( p0, &point0 ));  
        UF_CALL( UF_CURVE_create_point( p1, &point1 ));  
        UF_CALL( UF_CURVE_create_point( p2, &point2 ));  
        UF_CALL( UF_OBJ_set_name( point0, "POINT_0" ));  
        UF_CALL( UF_OBJ_set_name( point1, "POINT_1" ));  
        UF_CALL( UF_OBJ_set_name( point2, "POINT_2" ));  
        UF_VIEW_fit_view( NULL_TAG, 0.9 );  
        UF_CALL( UF_SO_create_scalar_double( object_in_part,   
                         update_option, scale_double, &scale ));  
        UF_CALL( UF_SO_create_xform_three_points( object_in_part, update_option,  
                 point0, point1, point2, scale, &xform1 ));  
        WRITENZ( xform1 );  
        UF_CALL( UF_SO_ask_point_of_xform( xform1, point ));  
        WRITE3F( point );  
        UF_CALL( UF_SO_ask_x_direction_of_xform( xform1, x_direction ));  
        UF_CALL( UF_SO_ask_y_direction_of_xform( xform1, y_direction ));  
        UF_CALL( UF_SO_ask_z_direction_of_xform( xform1, z_direction ));  
        WRITE3F( x_direction );  
        WRITE3F( y_direction );  
        WRITE3F( z_direction );  
        UF_CALL( UF_SO_ask_matrix_of_xform( xform1, matrix ));  
        WRITE16F( matrix );  
        UF_CALL( UF_SO_ask_scale_of_xform( xform1, &output_scale ));  
        WRITE_F( output_scale );  
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

> 这段NX二次开发代码的主要功能是：
>
> 1. 在当前部件中创建三个点，分别作为坐标变换的原始点、X轴方向点和Y轴方向点。
> 2. 根据这三个点创建一个坐标变换，并获取变换的属性，包括：变换的原点变换的X轴、Y轴、Z轴方向变换的4x4矩阵变换的缩放因子
> 3. 变换的原点
> 4. 变换的X轴、Y轴、Z轴方向
> 5. 变换的4x4矩阵
> 6. 变换的缩放因子
> 7. 在列表窗口中输出这些变换属性，以便观察和验证。
> 8. 提供了错误报告和输出到列表窗口的函数，用于调试和验证。
> 9. 在ufusr函数中初始化和终止NX Open C库，调用do_it函数执行主体功能。
> 10. 提供了卸载函数，允许立即卸载用户函数。
>
> 总体来说，这段代码的目的是演示如何使用三个点创建坐标变换，并获取变换的详细属性，对于理解坐标变换的创建和属性获取非常有帮助。
>
