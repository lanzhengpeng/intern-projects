### 【0615】create block and put variable radius blend on two edges 创建块并在两个边线上放置可变半径倒圆

#### 代码

```cpp
    /*HEAD CREATE_BLOCK_AND_PUT_VARIABLE_RADIUS_BLEND_ON_TWO_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，可以得知UF_print_syslog是V18版本新增的功能。

根据要求，这里仅提供翻译：

UF_print_syslog是V18版本新增的功能。 */  
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
    static void get_edge_points(tag_t edge, double points[3][3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, limits[0], &points[0][0], NULL));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, ((limits[0]+limits[1])/2), &points[1][0],  
            NULL));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, limits[1], &points[2][0], NULL));  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    /* 里海 */  
    static int vrb_routine(tag_t obj_id, double points[100][3],  
        char radii[100][256], int smooth_overflow, int cliff_overflow,  
        int notch_overflow, double vrb_tol, int *number_pts)  
    {  
        get_edge_points(obj_id, points);  
        strcpy(&radii[0][0], "5");  
        strcpy(&radii[1][0], "15");  
        strcpy(&radii[2][0], "5");  
        *number_pts = 3;  
        return 0;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            blend_feat,  
            block_feat,  
            edge;  
        double  
            block_orig[3] = { 0,0,0 };  
        char  
            *block_len[3] = { "200", "600", "30" };  
        uf_list_p_t  
            blend_edges,  
            block_edges;  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, block_orig, block_len,  
            &block_feat));  
        UF_CALL(UF_MODL_ask_feat_edges(block_feat, &block_edges));  
        UF_CALL(UF_MODL_create_list(&blend_edges));  
        UF_CALL(UF_MODL_ask_list_item(block_edges, 0, &edge));  
        UF_CALL(UF_MODL_put_list_item(blend_edges, edge));  
        UF_CALL(UF_MODL_ask_list_item(block_edges, 8, &edge));  
        UF_CALL(UF_MODL_put_list_item(blend_edges, edge));  
        UF_CALL(UF_MODL_delete_list(&block_edges));  
        UF_CALL(UF_MODL_register_var_blend(vrb_routine));  
        UF_CALL(UF_MODL_create_blend("1", blend_edges,  
            UF_MODL_BLEND_SMOOTH_OVERFLOW, UF_MODL_BLEND_CLIFF_OVERFLOW,  
            UF_MODL_BLEND_NOTCH_OVERFLOW, 0.00001, &blend_feat));  
        UF_CALL(UF_MODL_unregister_var_blend());  
        UF_CALL(UF_MODL_delete_list(&blend_edges));  
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

> 这段代码是一个NX Open API的二次开发示例，用于在两个边缘上创建一个可变半径倒圆角特征。主要步骤如下：
>
> 1. 包含必要的NX Open API头文件，定义错误报告函数report_error，用于打印错误信息。
> 2. 定义一个获取边线端点坐标的函数get_edge_points，用于获取边线起点、中点和终点的坐标。
> 3. 定义变量半径倒圆角函数vrb_routine，设置边线的三个点的半径分别为5、15和5，并返回点数。
> 4. 定义函数do_it，首先创建一个块特征，然后获取块特征的边缘列表。
> 5. 从边缘列表中取出两个边缘，创建一个可变半径倒圆角特征，并设置倒圆角参数，如平滑溢出、悬崖溢出等。
> 6. 调用vrb_routine函数，在两个边缘上创建可变半径倒圆角。
> 7. ufusr函数用于初始化NX Open API，调用do_it函数执行操作，并终止API。
> 8. ufusr_ask_unload函数用于卸载用户函数。
>
> 总的来说，这段代码展示了如何在NX Open API中创建可变半径倒圆角特征，并提供了获取边线点坐标、设置倒圆角参数等函数的实现。
>
