### 【0824】create point 3 scalars 创建3标量点

#### 代码

```cpp
    /*HEAD CREATE_POINT_3_SCALARS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_point.h>  
    #include <uf_so.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            n = 0;  
        tag_t  
            xyz[3] = { NULL_TAG, NULL_TAG, NULL_TAG },  
            smart_point_object = NULL_TAG,  
            smart_point_feature = NULL_TAG,  
            obj_in_part = NULL_TAG,  
            *things = NULL,  
            part = UF_PART_ask_display_part();  
        const double  
            x = 75.,  
            y = 50.,  
            z = 25.;  
        if( NULL_TAG == part ) return;  
        obj_in_part = UF_OBJ_cycle_all( part, obj_in_part );  
        UF_CALL( UF_SO_create_scalar_double ( obj_in_part,   
                 UF_SO_update_within_modeling, x, &xyz[0] ));  
        UF_CALL( UF_SO_create_scalar_double ( obj_in_part,   
                 UF_SO_update_within_modeling, y, &xyz[1] ));  
        UF_CALL( UF_SO_create_scalar_double ( obj_in_part,   
                 UF_SO_update_within_modeling, z, &xyz[2] ));  
        UF_CALL( UF_POINT_create_3_scalars( xyz, &smart_point_feature ));  
        UF_CALL( UF_SO_set_visibility_option( smart_point_feature,   
                                                     UF_SO_visible ));  
        UF_CALL( UF_MODL_ask_feat_object( smart_point_feature, &n, &things ));  
        if( n == 1 )  
        {  
            smart_point_object = things[0];     
            UF_CALL( UF_DISP_set_name_display_status( UF_DISP_NAME_DISPLAY_ON ));  
            UF_CALL( UF_OBJ_set_name( smart_point_object, "POINT_3_SCALARS" ));  
            UF_CALL( UF_VIEW_fit_view( NULL_TAG, 0.75 ));  
            UF_free( things );  
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

> 这段代码是一个用于在NX中创建一个点特征的二次开发示例。
>
> 主要步骤包括：
>
> 1. 包含必要的NX头文件，定义错误处理宏。
> 2. 在do_it函数中，首先获取当前显示的部件，并循环获取部件中的对象。
> 3. 使用UF_SO_create_scalar_double函数创建3个标量，分别代表点的X、Y、Z坐标，并设置它们的值。
> 4. 调用UF_POINT_create_3_scalars函数，传入3个标量，创建一个点特征。
> 5. 设置点特征的显示名称为"POINT_3_SCALARS"。
> 6. 调用UF_VIEW_fit_view函数，调整视图，使新创建的点特征可见。
> 7. ufusr函数初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载用户函数，返回立即卸载。
>
> 总体来说，这段代码通过UF函数实现了在NX中创建一个点特征，并设置其名称和可见性，同时包含了错误处理和视图调整功能。
>
