### 【0861】create relative datum plane 创建相对基准面

#### 代码

```cpp
    /*HEAD CREATE_RELATIVE_DATUM_PLANE CCC UFUN */  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_defs.h>  
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
    static void do_it(void)  
    {  
        double  
            four[3] = { 4,0,0 },  
            length = 1.5,  
            ref_point[3] = { 0,0,0 },  
            zero[3] = { 0,0,0 };  
        int  
            junk = 0,  
            num_dplanes,  
            point_select[3] = { 0,0,0 },  
            which_plane = 1;  
        tag_t  
            dplanes[2],  
            obj_eids[3];  
        uf_list_p_t  
            list;  
        FTN(uf5300)(zero, four, &length, &junk, obj_eids);  
        if (obj_eids[0])  
        {  
            UF_CALL(UF_MODL_ask_body_faces(obj_eids[0], &list));  
            UF_CALL(UF_MODL_ask_list_item(list, 0, &obj_eids[0]));  
            UF_free(list);  
            UF_CALL(UF_MODL_create_relative_dplane(1, obj_eids, point_select,  
                which_plane, ref_point, "0", "0", &num_dplanes, dplanes));  
            obj_eids[1] = obj_eids[0];  
            obj_eids[0] = dplanes[0];  
            UF_CALL(UF_MODL_create_relative_dplane(2, obj_eids, point_select,  
                which_plane, ref_point, "0", "0", &num_dplanes, dplanes));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个用于创建相对基准平面的NX Open C++二次开发示例代码，其主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误码和错误信息。
> 2. 定义了主函数do_it，其中：
>
> 1. 定义了ufusr函数，在NX启动时调用do_it函数，在NX关闭时释放资源。
> 2. 定义了ufusr_ask_unload函数，在NX卸载时立即卸载该库。
>
> 该代码的主要目的是演示如何在NX中通过二次开发创建相对基准平面，以及如何处理错误和资源管理。代码结构清晰，注释详细，是一个很好的二次开发示例。
>
