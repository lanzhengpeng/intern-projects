### 【1447】report curve mesh surface 报告曲面网格

#### 代码

```cpp
    /*HEAD REPORT_CURVE_MESH_SURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_emphasis(int emphasis)  
    {  
        switch (emphasis)  
        {  
            case 1:  
                ECHO("emphasis = 1 (Primary curves)\n");  
                break;  
            case 2:  
                ECHO("emphasis = 2 (Cross curves)\n");  
                break;  
            case 3:  
                ECHO("emphasis = 3 (Both)\n");  
                break;  
            default:  
                ECHO("unknown ");  
                WRITE_D(emphasis);  
                break;  
        }  
    }  
    static void report_spline_pts(int spline_pts)  
    {  
        switch (spline_pts)  
        {  
            case 0:  
                ECHO("spline_pts = 0 (Do not reparameterize curves)\n");  
                break;  
            case 1:  
                ECHO("spline_pts = 1 (Reparameterize through spline points)\n");  
                break;  
            default:  
                ECHO("unknown ");  
                WRITE_D(spline_pts);  
                break;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            body_type = 99,  
            c_flag[4],  
            emphasis = 99,  
            spline_pts = 99;  
        tag_t  
            face_id[4],  
            mesh;  
        double  
            tol[3] = { 0,0,0 };  
        UF_STRING_t  
            s_prim,  
            s_cross,  
            s_spine;  
        while ((mesh = select_a_feature("Curve Mesh")) != NULL_TAG)  
        {  
            ECHO("Using UF_MODL_ask_curve_mesh:\n");  
            if (UF_MODL_ask_curve_mesh(mesh, &s_prim, &s_cross, &s_spine,  
                &emphasis, &body_type, &spline_pts, tol, face_id, c_flag))  
                continue;  
        /*  These values do not match documentation - See PR 2133933 里海译:这些值与文档不匹配 - 请参阅PR 2133933 */  
            report_emphasis(emphasis);  
            report_spline_pts(spline_pts);  
            UF_MODL_free_string_list(&s_prim);  
            UF_MODL_free_string_list(&s_cross);  
            UF_MODL_free_string_list(&s_spine);  
            ECHO("\nUsing UF_MODL_ask_curve_mesh1:\n");  
            emphasis = 99,  
            spline_pts = 99;  
            if (UF_MODL_ask_curve_mesh1(mesh, &s_prim, &s_cross, &s_spine,  
                &emphasis, &body_type, &spline_pts, tol, face_id, c_flag))  
                continue;  
        /*  These values do not match documentation - See PR 2133933 里海译:翻译结果：这些值与文档不匹配 - 请参阅PR 2133933。 */  
            report_emphasis(emphasis);  
            report_spline_pts(spline_pts);  
            UF_MODL_free_string_list(&s_prim);  
            UF_MODL_free_string_list(&s_cross);  
            UF_MODL_free_string_list(&s_spine);  
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

> 这段代码是一个NX二次开发示例，其主要功能是查询和报告曲线网格特征的信息。
>
> 代码的主要组成部分包括：
>
> 1. 头文件包含：引入了必要的NX Open API头文件，如uf.h、uf_ui.h和uf_modl.h等。
> 2. 错误报告宏定义：定义了报告错误的宏函数report_error，用于在调用NX API失败时输出错误信息。
> 3. 选择特征函数：定义了选择曲线网格特征的函数select_a_feature。
> 4. 报告参数函数：定义了报告曲线网格参数的函数report_emphasis和report_spline_pts。
> 5. 主逻辑函数：定义了主逻辑函数do_it，用于遍历选择每个曲线网格特征，调用UF_MODL_ask_curve_mesh和UF_MODL_ask_curve_mesh1 API查询特征信息，并报告查询结果。
> 6. ufusr函数：作为NX的二次开发入口函数，调用UF_initialize初始化NX，执行do_it函数，然后调用UF_terminate终止NX。
> 7. 卸载函数：定义了卸载函数ufusr_ask_unload，用于在二次开发模块卸载时立即卸载。
>
> 总体而言，这段代码主要用于查询和报告曲线网格特征的信息，同时包含了错误处理、特征选择等辅助功能。
>
