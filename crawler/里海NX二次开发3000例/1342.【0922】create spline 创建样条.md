### 【0922】create spline 创建样条

#### 代码

```cpp
    /*HEAD CREATE_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，它可以将系统日志打印到文件中。 */  
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
    #define POLES 384  
    #define R1 2.25  
    #define R2 0.35  
    #define ANG PI/24  
    #define WEIGHT 1.0  
    #define ORDER 4  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            k_fixup,  
            n_states,  
            p_fixup;  
        tag_t  
            spline_id;  
        double  
            delta_u = { 1.0/((double)(POLES-ORDER+1)) },  
            knotseq[POLES+ORDER],  
            pole_array[POLES*4],  
            poles[POLES][4],  
            x,y,z,t,w = WEIGHT;  
        UF_CURVE_spline_t  
            spline_data;  
        UF_CURVE_state_t  
            *states;  
        spline_data.num_poles = POLES;  
        spline_data.order = ORDER;  
        spline_data.is_rational = TRUE;  
        spline_data.knots = knotseq;  
        spline_data.poles = poles;  
        spline_data.start_param = 0.0;  
        spline_data.end_param = 1.0;  
        for (ii = 0; ii < ORDER; ii++) knotseq[ii] = 0.0;  
        for (ii = POLES; ii < POLES + ORDER; ii++) knotseq[ii] = 1.0;  
        ii = 0;  
        jj = 0;  
        t = 0.0;  
        while (ii < POLES*4)  
        {  
            x = R1*cos(t) + .001;  
            y = R1*sin(t) + .001;  
            z = R2 + (R2/PI)*t;  
            t += ANG;  
            pole_array[ii++] = x;  
            poles[jj][0] = x;  
            pole_array[ii++] = y;  
            poles[jj][1] = y;  
            pole_array[ii++] = z;  
            poles[jj][2] = z;  
            pole_array[ii++] = w;  
            poles[jj][3] = w;  
            printf("%f, %f, %f, %f, %d\n",pole_array[ii-4],pole_array[ii-3],  
            pole_array[ii-2],pole_array[ii-1],ii-4);  
            jj++;  
        }  
        for (ii = ORDER; ii < POLES; ii++) knotseq[ii] = (ii-ORDER+1)*delta_u;  
        printf("Delta u is = %f\n", delta_u);  
        printf("Knot Sequence Values:\n");  
        for (ii = 0; ii < POLES + ORDER; ii++) printf("%f\n",knotseq[ii]);  
        UF_CALL(UF_MODL_create_spline(POLES, ORDER, knotseq, pole_array,  
                            &spline_id, &k_fixup, &p_fixup));  
        ECHO(spline_id);  
        ECHO(k_fixup);  
        ECHO(p_fixup);  
        UF_CALL(UF_CURVE_create_spline(&spline_data, &spline_id, &n_states,  
            &states));  
        ECHO(spline_id);  
        ECHO(n_states);  
        if (n_states > 0) UF_free(states);  
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

> 这段代码是用于在NX中创建一个样条曲线的UFUN函数。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了样条曲线的参数，包括控制点数POLES、内半径R1、外半径R2、角度增量ANG、权重WEIGHT以及阶数ORDER。
> 3. do_it函数用于生成样条曲线的控制点，并将它们存储在数组poles中。同时，生成了对应参数空间的节点向量knotseq。
> 4. 调用UF_MODL_create_spline函数，根据控制点和节点向量创建样条曲线，并获取样条曲线的ID。
> 5. 调用UF_CURVE_create_spline函数，根据样条曲线参数创建样条曲线。
> 6. ufusr函数是NX调用的主函数，用于初始化和结束NX环境，并执行do_it函数。
> 7. ufusr_ask_unload函数用于卸载UFUN，指定立即卸载。
>
> 通过这段代码，可以在NX中创建一个特定形状的样条曲线。它通过参数化方法生成样条曲线的控制点，实现了参数化曲线的建模。
>
