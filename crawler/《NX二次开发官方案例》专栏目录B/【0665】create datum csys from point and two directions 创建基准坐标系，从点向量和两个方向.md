### 【0665】create datum csys from point and two directions 创建基准坐标系，从点向量和两个方向

#### 代码

```cpp
    /*HEAD CREATE_DATUM_CSYS_FROM_POINT_AND_TWO_DIRECTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
    #include <uf_so.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是一个在V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            pt = NULL_TAG,  
            xdir = NULL_TAG,  
            ydir = NULL_TAG,  
            scale = NULL_TAG,  
            xform = NULL_TAG,  
            datum_csys_feature = NULL_TAG;  
        double point[ 3 ] = { 1., 1., 0. },  
               x_direction[ 3 ] = { 1., 0., 0. },  
               y_direction[ 3 ] = { 0., 1., 0. };  
        UF_CALL(UF_CURVE_create_point( point, &pt ));   
        UF_CALL(UF_SO_create_dirr_doubles( pt, UF_SO_update_within_modeling,  
                                           x_direction, &xdir ));  
        UF_CALL(UF_SO_create_dirr_doubles( pt, UF_SO_update_within_modeling,  
                                           y_direction, &ydir ));  
        UF_CALL(UF_SO_create_scalar_double( pt, UF_SO_update_within_modeling,  
                                            1.0, &scale ));  
        UF_CALL(UF_SO_create_xform_pnt_xy_dirs ( pt, UF_SO_update_within_modeling,   
                pt, xdir, ydir, scale, &xform ));  
        UF_CALL(UF_MODL_create_datum_csys ( pt, xform, TRUE,   
                                            &datum_csys_feature ));    
        UF_CALL(UF_OBJ_delete_object( pt ));  
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

> 这段代码是用于创建坐标系基准特征的NX二次开发代码。主要步骤包括：
>
> 1. 包含NX提供的二次开发头文件。
> 2. 定义错误报告函数report_error，用于输出函数调用错误。
> 3. 定义执行函数do_it，用于实现创建坐标系基准特征的主要逻辑。
> 4. 在do_it函数中，首先创建一个点对象pt。
> 5. 然后基于点pt，创建两个方向对象xdir和ydir。
> 6. 接着创建一个缩放对象scale。
> 7. 利用点pt、方向xdir、ydir和缩放scale，创建一个变换对象xform。
> 8. 最后利用点pt和变换对象xform，创建一个坐标系基准特征。
> 9. 在ufusr函数中，初始化NX环境，调用do_it函数执行创建操作，然后终止NX环境。
> 10. 定义卸载函数ufusr_ask_unload，用于在卸载时立即卸载该代码。
>
> 总体来说，该代码实现了从点、两个方向和缩放创建坐标系基准特征的功能。
>
