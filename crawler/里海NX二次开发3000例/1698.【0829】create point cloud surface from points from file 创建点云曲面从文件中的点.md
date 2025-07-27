### 【0829】create point cloud surface from points from file 创建点云曲面从文件中的点

#### 代码

```cpp
    /*HEAD CREATE_POINT_CLOUD_SURFACE_FROM_POINTS_FROM_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的。 */  
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
    #define WRITEnF3(X,n) (write_double_array_to_listing_window(#X, X, n, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
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
    static void do_it(void)  
    {  
        int  
            max_err_index,  
            n;  
        tag_t  
            surface;  
        double  
            avg_err,  
            max_err,  
            (*points)[3];  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        if (prompt_for_point_file_name("From Point Cloud", fspec) &&  
            ((n = ask_points_from_file(fspec, &points)) > 0))  
        {  
            WRITEnF3((double *)points, n);  
            UF_CALL(UF_MODL_create_surf_from_cloud(n, points, NULL, NULL, 3, 3,  
            1, 1, 0, &avg_err, &max_err, &max_err_index, &surface));  
            UF_free(points);  
        }  
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

> 这段NX二次开发代码主要实现了从点云文件创建曲面的功能，具体包括以下关键步骤：
>
> 1. 引入头文件：引入了必要的NX API头文件，如uf.h、uf_ui.h、uf_modl.h等。
> 2. 错误处理函数：定义了report_error函数，用于输出错误信息。
> 3. 文件对话框：定义了prompt_for_point_file_name函数，用于弹出文件对话框，让用户选择点云文件。
> 4. 内存分配：定义了allocate_memory函数，用于分配内存。
> 5. 数组输出：定义了write_double_array_to_listing_window函数，用于将数组输出到列表窗口。
> 6. 读取点云文件：定义了ask_points_from_file函数，用于读取用户选择的点云文件，并返回点的数量和坐标。
> 7. 曲面创建：在do_it函数中，先通过对话框获取点云文件，然后读取点云数据，最后调用UF_MODL_create_surf_from_cloud函数从点云创建曲面。
> 8. NX API初始化和终止：在ufusr函数中，初始化NX API，调用do_it函数，最后终止NX API。
> 9. 卸载函数：定义了ufusr_ask_unload函数，用于在插件卸载时调用。
>
> 总体来说，这段代码实现了从点云文件读取点数据，并利用NX的曲面建模功能创建曲面的二次开发功能，通过合理的模块化设计提高了代码的复用性和可维护性。
>
