### 【0919】create spline thru points from file 创建通过文件的点生成样条

#### 代码

```cpp
    /*HEAD CREATE_SPLINE_THRU_POINTS_FROM_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文意，UF_print_syslog是V18版本中新增的函数，请只提供翻译，不要添加其他内容。

翻译如下：

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
    static logical ask_true_or_false(char *prompt, int *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "True", "False" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if (resp > 4)  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_point_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.dat" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Point File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_points_from_file(char *fspec, double (**points)[3])  
    {  
        int  
            ii = 0,  
            n_points = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_points++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_points * sizeof(double[3]), (void **)points));  
            for (ii = 0; ii < n_points; ii++)  
            {  
                fscanf(the_file, "%lf %lf %lf",  
                    &(*points)[ii][0], &(*points)[ii][1], &(*points)[ii][2]);  
            }  
            fclose(the_file);  
        }  
        return n_points;  
    }  
    static logical ask_params_from_file(char *fspec, int n_points, double **params)  
    {  
        int  
            ii = 0;  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            UF_CALL(allocate_memory(n_points * sizeof(double), (void *)params));  
            for (ii = 0; ii < n_points; ii++)  
                fscanf(the_file, "%lf", &(*params)[ii]);  
            fclose(the_file);  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            err,  
            flag = FALSE,  
            ii,  
            n;  
        tag_t  
            spline;  
        double  
            (*spts)[3],  
            *parms = NULL;  
        char  
            data_file[MAX_FSPEC_SIZE+1],  
            parm_file[MAX_FSPEC_SIZE+1];  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        if (prompt_for_point_file_name("Spline Data", data_file) &&  
            ((n = ask_points_from_file(data_file, &spts)) > 0))  
        {  
            if (prompt_for_point_file_name("Parameter Data", parm_file))  
            {  
                ask_params_from_file(parm_file, n, &parms);  
            }  
            ask_true_or_false("Closed curve?", &flag);  
            point_data = (UF_CURVE_pt_slope_crvatr_t *)UF_allocate_memory(n *  
                sizeof(UF_CURVE_pt_slope_crvatr_t), &err);  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(spts[ii], point_data[ii].point);  
                if ((ii == 0) || (ii == (n-1)))  
                    point_data[ii].slope_type = UF_CURVE_SLOPE_AUTO;  
                else  
                    point_data[ii].slope_type = UF_CURVE_SLOPE_NONE;  
                point_data[ii].crvatr_type = UF_CURVE_CRVATR_NONE;  
            }  
            UF_CALL(UF_CURVE_create_spline_thru_pts(3, flag, n, point_data,  
                    parms, FALSE, &spline));  
            UF_free(point_data);  
            UF_free(spts);  
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

> 这段代码是用于创建经过给定点的样条曲线的NX二次开发代码，主要包含以下内容：
>
> 1. 包含所需的头文件，定义了报告错误的宏，以及用于询问用户布尔值的函数。
> 2. 定义了用于提示用户输入点文件和参数文件的函数。
> 3. 定义了用于从文件中读取点的函数，以及用于从文件中读取参数的函数。
> 4. 定义了主函数 do_it，用于获取用户输入的点文件和参数文件，并创建样条曲线。
> 5. 定义了ufusr函数，用于初始化和调用主函数，以及卸载函数。
>
> 该代码通过文件获取点坐标和参数，然后创建经过这些点的样条曲线。用户可以通过交互式对话框选择点文件和参数文件，同时可以指定曲线是否闭合。样条曲线的斜率自动计算，参数可从文件获取或默认为0。
>
> 代码结构清晰，注释详细，具有良好的可读性，适用于NX的二次开发，实现了创建经过给定点的样条曲线的功能。
>
