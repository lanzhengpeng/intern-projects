### 【0816】create ordinate dimensions 创建坐标尺寸

#### 代码

```cpp
    /*HEAD CREATE_ORDINATE_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
            d_line,  
            h_dim[3],  
            h_line,  
            h_marg,  
            origin,  
            o_point,  
            v_dim[3],  
            v_marg,  
            v_line;  
        double  
            h_org0[3] = { 4, 1, 0 },  
            h_org1[3] = { 10, 1, 0 },  
            h_org2[3] = { 20, 1, 0 },  
            o_data[3] = { 5, 5, 0 },  
            v_org0[3] = { 1, 4, 0 },  
            v_org1[3] = { 1, 10, 0 },  
            v_org2[3] = { 1, 20, 0 };  
        UF_CURVE_line_t  
            d_data = { 10,10,0, 20,20,0 },  
            h_data = { 4,2,0, 24,2,0 },  
            v_data = { 2,4,0, 2,24,0 };  
        UF_CALL(UF_CURVE_create_point(o_data, &o_point));  
        UF_CALL(UF_CURVE_create_line(&d_data, &d_line));  
        UF_CALL(UF_CURVE_create_line(&h_data, &h_line));  
        UF_CALL(UF_CURVE_create_line(&v_data, &v_line));  
        UF_CALL(UF_DRF_create_ordinate_origin(o_point, 1, 1, 5, 1, 2, "origin",  
            &origin));  
        UF_CALL(UF_DRF_create_ordinate_margin(1, origin, h_line, NULL, NULL, 0,  
            &h_marg));  
        UF_CALL(UF_DRF_create_ordinate_margin(2, origin, v_line, NULL, NULL, 0,  
            &v_marg));  
        UF_CALL(UF_DRF_create_ordinate_dim(h_marg, 1, o_point, 1, 1, 0, 0, NULL,  
            0, NULL, 1, h_org0, &h_dim[0]));  
        UF_CALL(UF_DRF_create_ordinate_dim(h_marg, 1, d_line, 1, 1, 0, 0, NULL,  
            0, NULL, 1, h_org1, &h_dim[1]));  
        UF_CALL(UF_DRF_create_ordinate_dim(h_marg, 1, d_line, 1, 2, 0, 0, NULL,  
            0, NULL, 1, h_org2, &h_dim[2]));  
        UF_CALL(UF_DRF_create_ordinate_dim(v_marg, 2, o_point, 1, 1, 0, 0, NULL,  
            0, NULL, 1, v_org0, &v_dim[0]));  
        UF_CALL(UF_DRF_create_ordinate_dim(v_marg, 2, d_line, 1, 1, 0, 0, NULL,  
            0, NULL, 1, v_org1, &v_dim[1]));  
        UF_CALL(UF_DRF_create_ordinate_dim(v_marg, 2, d_line, 1, 2, 0, 0, NULL,  
            0, NULL, 1, v_org2, &v_dim[2]));  
        UF_CALL(UF_OBJ_delete_object(h_line));  
        UF_CALL(UF_OBJ_delete_object(v_line));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 定义了错误处理函数report_error，用于打印错误信息。
> 2. 定义了do_it函数，用于创建一个坐标系，并在该坐标系下创建水平和垂直方向的尺寸标注。
> 3. 在do_it函数中，首先创建一个点、一条水平和一条垂直线段。
> 4. 使用UF_DRF_create_ordinate_origin函数创建坐标原点。
> 5. 使用UF_DRF_create_ordinate_margin函数创建水平和垂直方向的尺寸标注边距。
> 6. 使用UF_DRF_create_ordinate_dim函数在指定位置创建尺寸标注，包括原点、水平线段和垂直线段上的位置。
> 7. 最后删除水平和垂直线段。
> 8. ufusr函数作为NX Open C++的入口函数，调用UF_initialize初始化NX系统，调用do_it函数执行功能，然后调用UF_terminate结束NX系统。
> 9. ufusr_ask_unload函数用于设置在卸载NX Open C++应用程序时的行为。
>
> 总体来说，这段代码实现了一个简单的创建尺寸标注的功能，展示了如何使用NX Open C++ API进行二次开发。
>
