### 【0195】ask extrusion curves 请问挤压曲线

#### 代码

```cpp
    /*HEAD ASK_EXTRUSION_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    static void report_curve_data(tag_t curve)  
    {  
        int  
            type;  
        double  
            *curve_data;  
        UF_CURVE_struct_p_t  
            curve_struct;  
        if (UF_CALL(UF_CURVE_ask_curve_struct(curve, &curve_struct))) return;  
        UF_CALL(UF_CURVE_ask_curve_struct_data(curve_struct,&type,&curve_data));  
        switch (type)  
        {  
            case UF_point_type:  
                WRITE("Point:\n ");  
                WRITE(" Absolute coordinates:");  
                WRITE3F(curve_data);  
                break;  
            case UF_line_type:  
                WRITE("Line:\n");  
                WRITE(" Start coordinates:  ");  
                WRITE3F(&curve_data[3]);  
                WRITE(" End coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                break;  
            case UF_circle_type:  
                WRITE("Arc:\n");  
                WRITE(" Start angle (radians):  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" End angle (radians):  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                WRITE(" X axis of construction plane:  ");  
                WRITE3F(&curve_data[9]);  
                WRITE(" Y axis of construction plane:  ");  
                WRITE3F(&curve_data[12]);  
                break;  
            case UF_conic_type:  
                WRITE("Conic:\n");  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[8]);  
                WRITE(" axis1:  ");  
                WRITE3F(&curve_data[11]);  
                WRITE(" axis2:  ");  
                WRITE3F(&curve_data[14]);  
                break;  
            case UF_spline_type:  
                WRITE("Spline:\n");  
                WRITE(" Number of poles:  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" Order:  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Knots:  ");  
                WRITEnF((int)(curve_data[3]+curve_data[4]),&curve_data[5]);  
                WRITE(" Poles:  ");  
                WRITEnF4((&curve_data[5])+(int)(curve_data[3]+curve_data[4]),  
                    curve_data[3]);  
                break;  
            default:  
                WRITE("Unknown curve type ");  
                WRITE_D(type);  
                break;  
        }  
        UF_CALL(UF_CURVE_free_curve_struct(curve_struct));  
        UF_free(curve_data);  
    }  
    static void do_it(void)  
    {  
        logical  
            region_specified,  
            solid_creation;  
        int  
            count,  
            ii,  
            jj,  
            n_curves;  
        tag_t  
            body = NULL_TAG,  
            *curves,  
            feat,  
            part = UF_PART_ask_display_part();  
        double  
            direction[3],  
            region_point[3];  
        uf_list_p_t  
            list;  
        char  
            *limits[2],  
            *offsets[2],  
            *feat_name,  
            *feat_type,  
            *taper_angle;  
        UF_MODL_SWEEP_TRIM_object_p_t  
            trim_ptr;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_ask_body_feats( body, &list));  
            UF_CALL( UF_MODL_ask_list_count( list, &count));  
            printf("Body %d has %d features\n", body, count);  
            for (ii = 0; ii < count; ii++)  
            {  
                UF_CALL( UF_MODL_ask_list_item( list, ii, &feat));  
                UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
                UF_CALL( UF_MODL_ask_feat_name( feat, &feat_name));  
                printf("\n  %d.  %s - %s\n", ii+1, feat_name, feat_type);  
                if (!strcmp( feat_type, "SWP104") &&  
                    !UF_CALL( UF_MODL_ask_extrusion( feat, &n_curves, &curves,  
                        &trim_ptr, &taper_angle, limits, offsets, region_point,  
                        &region_specified, &solid_creation, direction)))  
                {  
                    for (jj = 0; jj < n_curves; jj++)  
                        report_curve_data(curves[jj]);  
                    UF_free(curves);  
                    if (trim_ptr) UF_free(trim_ptr);  
                    UF_free(taper_angle);  
                    for (jj = 0; jj < 2; jj++) UF_free(limits[jj]);  
                    for (jj = 0; jj < 2; jj++) UF_free(offsets[jj]);  
                }  
                UF_free( feat_name);  
                UF_free( feat_type);  
            }  
            UF_CALL(UF_MODL_delete_list(&list));  
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

> 这段代码是一个NX二次开发示例，主要功能是遍历当前零件中的每个实体，并打印出每个实体上拉伸特征的相关信息。
>
> 具体来说，该代码主要包括以下几个部分：
>
> 1. 头文件：包含了一些标准库和NX提供的库，以支持代码的执行。
> 2. 错误处理函数：用于处理代码执行中的错误，并输出相应的错误信息。
> 3. 实体遍历函数：用于遍历当前零件中的每个实体。
> 4. 输出函数：用于输出实体的名称、类型、拉伸曲线信息等。
> 5. 主函数：遍历每个实体，查询其特征，并打印拉伸特征的相关信息。
> 6. NX初始化和终止函数：在NX加载和卸载时调用，以初始化和清理NX环境。
> 7. 卸载函数：用于处理NX卸载时的相关操作。
>
> 通过这段代码，可以实现对零件中拉伸特征信息的查询和输出，从而辅助用户进行相关的二次开发工作。
>
