### 【0603】create arc using specified center start and end positions 创建弧线需要指定圆心、起始点和终止点的位置

#### 代码

```cpp
    /*HEAD CREATE_ARC_USING_SPECIFIED_CENTER_START_AND_END_POSITIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static tag_t create_arc_using_center_start_and_end_locations(double pos[3][3])  
    {  
        int  
            ii;  
        tag_t  
            arc,  
            points[3];  
        uf_list_p_t  
            arc_list;  
        for (ii = 0; ii < 3; ii++)  
            UF_CALL(UF_CURVE_create_point(&pos[ii][0], &points[ii]));  
        UF_CALL(UF_SO_create_arc_center_2_pnts(points[0],  
            UF_SO_update_within_modeling, points, &arc));  
        UF_CALL(UF_SO_set_visibility_option(arc, UF_SO_visible));  
        UF_CALL(UF_MODL_create_list(&arc_list));  
        UF_CALL(UF_MODL_put_list_item(arc_list, arc));  
        UF_CALL(UF_MODL_delete_object_parms(arc_list));  
        UF_CALL(UF_MODL_delete_list(&arc_list));  
        UF_CALL(UF_OBJ_delete_array_of_objects(3, points, NULL));  
        return arc;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            arc;  
        double  
            pos[3][3];  
        char  
            *msg[3] = { "Pick arc center",  
                        "Pick arc start",  
                        "Pick arc end" };  
        for (ii = 0; ii < 3; ii++)  
        {  
            if (!specify_point(msg[ii], &pos[ii][0])) ii--;  
            if (ii < 0) return;  
        }  
        arc = create_arc_using_center_start_and_end_locations(pos);  
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

> 这段代码是一个用于在NX中创建弧的二次开发示例。主要功能包括：
>
> 1. 包含了必要的NX二次开发头文件，如uf.h、uf_ui.h、uf_curve.h等。
> 2. 定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义了一个函数specify_point，用于提示用户拾取点，并获取点的坐标。
> 4. 定义了一个函数create_arc_using_center_start_and_end_locations，用于根据中心点、起点和终点坐标创建弧。
> 5. 定义了一个函数do_it，用于提示用户拾取弧的中心点、起点和终点，然后调用create_arc_using_center_start_and_end_locations函数创建弧。
> 6. 定义了ufusr函数，作为二次开发的入口函数。在该函数中，初始化NX环境，调用do_it函数执行创建弧的操作，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 总体来说，这段代码通过二次开发实现了在NX中通过指定中心点、起点和终点来创建弧的功能。
>
