### 【0918】create spline fit to specified points 创建指定点拟合样条

#### 代码

```cpp
    /*HEAD CREATE_SPLINE_FIT_TO_SPECIFIED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18版本中新增的，请只提供翻译，不要添加其他内容。 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical pick_one_of_four_choices(char *prompt, char *option_one,  
        char *option_two, char *option_three, char *option_four, int *choice)  
    {  
        int  
            deflt = (*choice)+1,  
            resp;  
        char  
            options[4][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        strncpy(&options[2][0], option_three, 37);  
        strncpy(&options[3][0], option_four, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        options[2][37] = '\0';  
        options[3][37] = '\0';  
        resp = uc1603(prompt, deflt, options, 4);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 5;  /* returns 0, 1, 2, or 3 里海译:根据上下文，翻译 "returns 0, 1, 2, or 3" 为：

返回 0、1、2 或 3。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            max_err_pt,  
            ii,  
            n,  
            resp;  
        tag_t  
            spline;  
        double  
            junk[3],  
            max_err;  
        UF_UI_chained_points_t  
            *points;  
        SPLINE_FIT_t  
            fit_data;  
        UF_UNDO_mark_id_t  
            mark;  
        fit_data.points = NULL;  
        fit_data.slopes = NULL;  
        fit_data.weights = NULL;  
        UF_MODL_ask_distance_tolerance(&fit_data.tolerance);  
        fit_data.num_of_points = 0;  
        fit_data.slope_flag = 0;  
        fit_data.num_of_weights = 0;  
        fit_data.weight_positions = NULL;  
        fit_data.num_of_segments = 0;  
        fit_data.degree = 3;  
        while (!UF_CALL(UF_UI_select_point_collection("Select points", FALSE,  
               &points, &n, &resp)) && (resp == UF_UI_OK) &&  
               pick_one_of_four_choices("Specify slopes?", "No slopes",  
                "Start slope only", "End slope only", "Both start and end slopes",  
                &fit_data.slope_flag))  
        {  
            switch (fit_data.slope_flag)  
            {  
                case 1:  
                    fit_data.slopes = (double *)UF_allocate_memory(  
                        3*sizeof(double), &ii);  
                    specify_vector("Start slope", fit_data.slopes, junk);  
                    break;  
                case 2:  
                    fit_data.slopes = (double *)UF_allocate_memory(  
                        3*sizeof(double), &ii);  
                    specify_vector("End slope", fit_data.slopes, junk);  
                    break;  
                case 3:  
                    fit_data.slopes = (double *)UF_allocate_memory(  
                        6*sizeof(double), &ii);  
                    specify_vector("Start slope", fit_data.slopes, junk);  
                    specify_vector("End slope", &fit_data.slopes[3], junk);  
                    break;  
                default:  
                    break;  
            }  
            fit_data.num_of_points = n;  
            fit_data.points = (double *)UF_allocate_memory(3*n*sizeof(double),&ii);  
            for (ii = 0; ii < n; ii++)  
                UF_VEC3_copy(points[ii].pt, &fit_data.points[ii*3]);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Create fitted spline",  
                &mark));  
            if (!UF_CALL(UF_MODL_create_fitted_spline(&fit_data, &max_err,  
                &max_err_pt, &spline)))  
            {  
                WRITE_F(max_err);  
                WRITE_D(max_err_pt);  
                WRITE_D(spline);  
            }  
            UF_free(points);  
            UF_free(fit_data.points);  
            if (fit_data.slope_flag > 0) UF_free(fit_data.slopes);  
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

> 这段代码是一个NX Open C++程序，主要功能是创建一个通过指定点的拟合样条曲线。以下是代码的详细介绍：
>
> 1. 包含必要的NX Open头文件，定义了一些宏来简化错误处理和日志打印。
> 2. 定义了一个report_error函数，用于打印出错误信息。
> 3. 定义了一个write_integer_to_listing_window函数，用于在日志窗口中打印整数值。
> 4. 定义了一个write_double_to_listing_window函数，用于在日志窗口中打印浮点数值。
> 5. 定义了一个pick_one_of_four_choices函数，用于在4个选项中选择一个。
> 6. 定义了一个specify_vector函数，用于让用户指定一个向量。
> 7. 定义了一个do_it函数，是程序的主逻辑：
>
> 1. 定义了ufusr函数，作为程序的入口点。在该函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 2. 定义了一个ufusr_ask_unload函数，用于在卸载时立即释放内存。
>
> 综上所述，这是一个简单的NX Open C++程序，实现了通过指定点的拟合样条曲线的创建功能。代码结构清晰，利用了NX Open提供的API来实现所需功能。
>
