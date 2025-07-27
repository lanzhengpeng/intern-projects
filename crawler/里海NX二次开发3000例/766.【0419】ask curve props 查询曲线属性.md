### 【0419】ask curve props 查询曲线属性

#### 代码

```cpp
    /*HEAD ASK_CURVE_PROPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #define ECHO1(X)   (printf("%s\n\t%f\n", #X, X))  
    #define ECHO3(X)   (report_double_array(#X, X, 1, 3))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的功能。

对于这个功能，我的翻译是：

"UF_print_syslog是V18版本新增的，只回答译文，不要废话。" */  
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
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            pt;  
        double  
            b_norm[3],  
            p_norm[3],  
            parm,  
            point[3],  
            radius,  
            tangent[3],  
            torsion;  
        while  
            (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_spline_type, &curve)) &&  
            (curve != NULL_TAG))  
        {  
            for (parm = 0; parm <= 1; parm = parm + 0.1)  
            {  
                UF_CALL(UF_MODL_ask_curve_props(curve, parm, point, tangent, p_norm,  
                    b_norm, &torsion, &radius));  
                UF_CALL(UF_CURVE_create_point(point, &pt));  
                ECHO3(tangent);  
                ECHO3(p_norm);  
                ECHO3(b_norm);  
                ECHO1(torsion);  
                ECHO1(radius);  
            }  
        }  
    }  
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

> 这段NX Open C++代码用于查询NX中显示部件中的所有样条曲线的属性，并在控制台输出相关信息。具体功能如下：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历显示部件中的所有对象，并判断是否为样条曲线类型。
> 2. 使用UF_MODL_ask_curve_props函数查询样条曲线在参数值parm处的属性，包括位置、切向量、主法向量、副法向量、挠率和半径。
> 3. 使用UF_CURVE_create_point函数在查询点处创建点对象。
> 4. 在控制台输出样条曲线在参数值0到1之间的属性信息，参数值间隔为0.1。
> 5. 使用自定义的report_error函数在出错时输出错误信息。
> 6. 在ufusr函数中初始化和终止NX Open，并调用do_it函数执行查询。
> 7. ufusr_ask_unload函数设置用户子程序立即卸载。
>
> 这段代码通过UF_MODL_ask_curve_props函数，实现了对NX样条曲线属性的查询和输出，展示了NX二次开发中对象遍历、属性查询、错误处理等常用技巧。
>
