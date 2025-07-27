### 【2992】translate selected geometry relative to wcs 基于WCS坐标系平移选定的几何体

#### 代码

```cpp
    /*HEAD TRANSLATE_SELECTED_GEOMETRY_RELATIVE_TO_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_vec.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static logical prompt_for_a_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "dX", "dY", "dZ" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static void map_wcs2abs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(wcs, abs_mx, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            off = 2,  
            orig = 0,  
            move = 1,  
            resp;  
        tag_t  
            *stuff = NULL,  
            wcs_id = NULL_TAG,  
            mtx_tag = NULL_TAG;  
        double  
            delta[3] = { 0., 0., 0. },  
            local[3] = { 0., 0., 0. },  
            origin[3] = { 0., 0., 0. },  
            mx[16];  
        UF_CALL(UF_CSYS_ask_wcs ( &wcs_id ));  
        UF_CALL( UF_CSYS_ask_csys_info ( wcs_id, &mtx_tag, origin ));  
        while ((( n = select_objects("Select objects to translate:", &stuff)) > 0)  
                  && prompt_for_a_vector("Distances to translate:", local ))  
        {  
            map_wcs2abs( local );  
            UF_VEC3_sub(local, origin, delta);  
            FTN(uf5943)(delta, mx);  
            FTN(uf5947)(mx, stuff, &n, &move, &orig, &off, NULL, NULL, &resp);  
            if( resp ) WRITE_D( resp );  
            UF_free(stuff);  
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

> 这段代码是一个NX二次开发程序，主要用于执行以下功能：
>
> 1. 选择要移动的对象：使用UF_UI_select_with_class_dialog函数让用户选择要移动的几何体对象。
> 2. 输入移动向量：使用uc1609函数让用户输入要移动的距离和方向。
> 3. 将工作坐标系(WCS)中的移动向量转换为绝对坐标系(ABS)中的向量。
> 4. 使用UF_VEC3_sub计算WCS原点到ABS原点的向量差。
> 5. 构造移动矩阵，将ABS中的移动向量差转换为移动矩阵。
> 6. 调用uf5947函数，将移动矩阵应用到用户选择的几何体对象上，实现移动。
> 7. 如果移动成功，输出响应码。
> 8. 重复执行步骤1-7，直到用户不选择对象或取消输入为止。
> 9. 程序开始时调用UF_initialize初始化NX环境，结束时调用UF_terminate终止NX环境。
> 10. 提供一个询问卸载函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 这个程序利用NX提供的几何体选择、坐标系转换、向量运算等功能，实现了相对工作坐标系移动几何体的功能。
>
